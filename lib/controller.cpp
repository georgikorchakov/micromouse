#include "controller.h"

profile_t* speed_profile = init_profile();
profile_t* angle_profile = init_profile();

volatile uint8_t controller_output_enable = false;

float speed_error = 0;
float rotation_error = 0;
float previous_speed_error = 0;
float previous_rotation_error = 0;

// Just to acknowledge. This function will probably violate 
// all of the recommendations of interrupt functions. 
// We should pay attention, because many things may go wrong.
void systick()
{
    // Read the encoders
    // For now I will write this code like I will use encoders only.
    // Gyroscope and proximity sensors will be added after that.
    relative_position_t position_change = get_ecnoder_position_change_since_last_read();

    profile_update(speed_profile);
    profile_update(angle_profile);

    float speed_output = speed_controller(speed_profile, position_change.distance);
    float rotation_output = rotation_controller(angle_profile, position_change.angle);

    float left_motor_pwm = voltage_to_pwm(speed_output - rotation_output);
    float right_motor_pwm = voltage_to_pwm(speed_output + rotation_output);

    if (controller_output_enable)
    {
        set_left_motor_pwm(left_motor_pwm);
        set_right_motor_pwm(right_motor_pwm);
    }
}

uint8_t start_systick_interrupt()
{
    systick_interrupt.priority(SYSTICK_INTERRUPT_PRIORITY);
    uint8_t ok = systick_interrupt.begin(systick, SYSTICK_INTERRUPT_TIME_MICRO_SECONDS);

    if (!ok)
    {
        // Serial.println("[Error] Failed to start systick interupt");
    }

    return ok;
}

void stop_systick_interrupt()
{
    systick_interrupt.end();
}

float speed_controller(profile_t* speed_profile, float real_distance_change)
{
    speed_error += 
        profile_get_distance_by_one_systick(speed_profile) - real_distance_change;
    
    float output = KP_S * speed_error + KD_S * (speed_error - previous_speed_error);

    previous_speed_error = speed_error;

    return output;
}

float rotation_controller(profile_t* rotation_profile, float real_angle_change)
{
    rotation_error += 
        profile_get_distance_by_one_systick(rotation_profile) - real_angle_change;

    float output = KP_R * rotation_error + KD_R * (rotation_error - previous_rotation_error);

    previous_rotation_error = rotation_error;

    return output;
}

void start_controller_output()
{
    controller_output_enable = true;
}

void stop_controller_output()
{
    controller_output_enable = false;
}

////////////////////////////////////////////////////////////
// Profiler
////////////////////////////////////////////////////////////

profile_t* init_profile()
{
    profile_t* profile = (profile_t*)malloc(sizeof(profile_t));

    if (!profile)
    {
        // Serial.println("[Error] Faild to allocate profile_t*");
        return NULL;
    }

    // TODO: check malloc
    profile->speed = 0;
    profile->position = 0;
    profile->final_position = 0;
    profile->target_speed = 0;
    profile->final_speed = 0;
    profile->acceleration = 0;
    profile->state = PROFILE_STATE_IDLE;
    profile->sign = 1;

    return profile;
}

void profile_start(profile_t* profile, float distance, float max_speed, float final_speed, float acceleration)
{
    if (distance < 0)
    {
        profile->sign = -1;
        distance = -distance;
    }
    else
    {
        profile->sign = 1;
    }

    if (distance < ACCEPTABLE_DISTANCE_FOR_FINISH)
    {
        profile->state = PROFILE_STATE_FINISHED;
        return;
    }

    if (final_speed > max_speed)
    {
        final_speed = max_speed;
    }

    // We don't set the profile.speed to 0, because
    // we may use profile_start() for second 
    // time with previous final_speed > 0
    profile->position = 0;
    profile->final_position = distance;

    // In the beginning we set the target speed, so the profiler will create acceleration.
    // Later in the process the target speed will become the final speed.
    profile->target_speed = profile->sign * fabsf(max_speed);
    profile->final_speed = profile->sign * fabsf(final_speed);
    profile->acceleration = profile->sign * fabsf(acceleration);

    profile->state = PROFILE_STATE_ACCELERATING;
}

void profile_update(profile_t* profile)
{
    if (profile->state == PROFILE_STATE_IDLE)
    {
        return;
    }

    // ---------- Update Speed ----------

    float remaining_distance = fabsf(profile->final_position) - fabsf(profile->position);

    // To be honest PROFILE_STATE_ACCELERATING state is used not only for acceleration.
    // This flag stays set when the robot is already achived the target speed
    // and is traveling with constant velocity.
    // It is changed to PROFILE_STATE_DECELERATING, when the robot starts to decelerate.
    // That allows us to start decelerating even when we are not achieved the required
    // max_speed (variable from profile_start()). 
    // For example when we use the controller for really smal distances.
    if (profile->state == PROFILE_STATE_ACCELERATING)
    {
        if (remaining_distance <= distance_required_for_deceleration(profile))
        {
            profile->state = PROFILE_STATE_DECELERATING;
            profile->target_speed = profile->final_speed;
        }
    }

    // The change in speed for one systick period
    // By the first equation of motion
    float change_in_speed = profile->acceleration * SYSTICK_PERIOD;

    // Accelerating
    if (profile->speed < profile->target_speed)
    {
        profile->speed += change_in_speed;
        if (profile->speed > profile->target_speed)
        {
            profile->speed = profile->target_speed;
        }
    }
    // Decelerating
    else if (profile->speed > profile->target_speed)
    {
        profile->speed -= change_in_speed;
        if (profile->speed < profile->target_speed)
        {
            profile->speed = profile->target_speed;
        }
    }

    // ---------- Update Position ----------

    // By the second equation of motion
    profile->position += profile->speed * SYSTICK_PERIOD;

    if (profile->state != PROFILE_STATE_FINISHED &&
        remaining_distance < ACCEPTABLE_DISTANCE_FOR_FINISH)
    {
        profile->state = PROFILE_STATE_FINISHED;
        profile->target_speed = profile->final_speed; 
    }
}

float distance_required_for_deceleration(profile_t* profile)
{
    // By the third equation of motion
    return fabsf( profile->final_speed * profile->final_speed 
                  - profile->speed * profile->speed ) / (2 * profile->acceleration);
}

void profile_stop(profile_t* profile)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        profile->target_speed = 0;
    }

    profile_finish(profile);
}

void profile_finish(profile_t* profile)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        profile->speed = profile->target_speed;
        profile->state = PROFILE_STATE_FINISHED;
    }
}

float profile_get_position(profile_t* profile)
{
    float position;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        position = profile->position;
    }
    return position;
}

float profile_get_speed(profile_t* profile)
{
    float speed;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        speed = profile->speed;
    }
    return speed;
}

float profile_get_distance_by_one_systick(profile_t* profile)
{
    float distance_by_one_systick;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        distance_by_one_systick = profile->speed * SYSTICK_PERIOD;
    }
    return distance_by_one_systick;
}

float profile_get_acceleration(profile_t* profile)
{
    float acceleration;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        acceleration = profile->acceleration;
    }
    return acceleration;
}

void profile_set_state(profile_t* profile, uint8_t state)
{
    profile->state = state;
}

void profile_set_speed(profile_t* profile, float speed)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        profile->speed = speed;
    }
}

void profile_set_target_speed(profile_t* profile, float speed)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        profile->target_speed = speed;
    }
}

void profile_set_position(profile_t* profile, float position)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        profile->position += position;
    }
}

void profile_adjust_position(profile_t* profile, float adjustment)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        profile->position += adjustment;
    }
}
