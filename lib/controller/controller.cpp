#include "controller.h"

IntervalTimer systick_interrupt;

proximity_sensors_t *contoller_proximity_sensors = NULL;

profile_t *speed_profile;
profile_t *angle_profile;

volatile uint8_t controller_output_enable = false;
volatile uint8_t systick_function_enable = false;

float speed_error = 0;
float rotation_error = 0;
float previous_speed_error = 0;
float previous_rotation_error = 0;

float min_e = 100;
float max_e = 0;

// Just to acknowledge. This function will probably violate 
// all of the recommendations of interrupt functions.
// We should pay attention, because many things may go wrong.
void systick()
{
    if (!systick_function_enable)
    {
        return;
    }

    // timestamp_t timestamp;

    read_proximity_sensors_side(&contoller_proximity_sensors);
    //read_proximity_sensors(&contoller_proximity_sensors);
    // log_proximity_sensors_to_timestap(&timestamp, &contoller_proximity_sensors);
    
    // Read the encoders
    // For now I will write this code like I will use encoders only.
    // Gyroscope and proximity sensors will be added after that.
    relative_position_t position_change = get_ecnoder_position_change_since_last_read();

    read_gyroscope();
    // log_gyroscope_z_to_timestamp(&timestamp, (-gyro_z)*0.001);

    profile_update(&speed_profile);
    profile_update(&angle_profile);

    float speed_output = speed_controller(&speed_profile, position_change.distance);
    float rotation_output = rotation_controller(&angle_profile, ((-gyro_z)*0.001) );

    float left_motor_pwm = voltage_to_pwm(speed_output + rotation_output);
    float right_motor_pwm = voltage_to_pwm(speed_output - rotation_output);

    if (controller_output_enable)
    {
        set_left_motor_pwm(left_motor_pwm);
        set_right_motor_pwm(right_motor_pwm);
    }

    // save_timestamp(timestamp);
}

float speed_controller(profile_t **__speed_profile, float real_distance_change)
{
    speed_error += 
        profile_get_distance_by_one_systick(__speed_profile) - real_distance_change;
    
    float output = KP_S * speed_error + KD_S * (speed_error - previous_speed_error);

    previous_speed_error = speed_error;

    return output;
}

float rotation_controller(profile_t **__rotation_profile, float real_angle_change)
{
    rotation_error += 
        profile_get_distance_by_one_systick(__rotation_profile) - real_angle_change;

    float output = KP_R * rotation_error + KD_R * (rotation_error - previous_rotation_error);

    previous_rotation_error = rotation_error;

    return output;
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

void start_controller_output()
{
    controller_output_enable = true;
}

void stop_controller_output()
{
    controller_output_enable = false;
}

void enable_systick_function()
{
    systick_function_enable = true;
}

void disable_systick_function()
{
    systick_function_enable = false;
}

void init_control_system()
{
    speed_profile = init_profile_struct();
    angle_profile = init_profile_struct();

    if (speed_profile == NULL || angle_profile == NULL)
    {
        Serial.println("Speed profiles faild to initialize!");
        while(1); 
    }

    contoller_proximity_sensors = init_proximity_sensors_struct();
}


////////////////////////////////////////////////////////////
// Profiler
////////////////////////////////////////////////////////////

profile_t* init_profile_struct()
{
    profile_t *profile = (profile_t*)malloc(sizeof(profile_t));

    if (!profile)
    {
        // Serial.println("[Error] Faild to allocate profile_t*");
        return NULL;
    }

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

void profile_start(profile_t **__profile, float distance, float max_speed, float final_speed, float acceleration)
{
    if (distance < 0)
    {
        (*__profile)->sign = -1;
        distance = -distance;
    }
    else
    {
        (*__profile)->sign = 1;
    }

    if (distance < ACCEPTABLE_DISTANCE_FOR_FINISH)
    {
        (*__profile)->state = PROFILE_STATE_FINISHED;
        return;
    }

    if (final_speed > max_speed)
    {
        final_speed = max_speed;
    }

    // We don't set the profile.speed to 0, because
    // we may use profile_start() for second 
    // time with previous final_speed > 0
    (*__profile)->position = 0;
    (*__profile)->final_position = distance;

    // In the beginning we set the target speed, so the profiler will create acceleration.
    // Later in the process the target speed will become the final speed.
    (*__profile)->target_speed = (*__profile)->sign * fabsf(max_speed);
    (*__profile)->final_speed  = (*__profile)->sign * fabsf(final_speed);
    (*__profile)->acceleration = (*__profile)->sign * fabsf(acceleration);

    (*__profile)->state = PROFILE_STATE_ACCELERATING;
}

void profile_update(profile_t **__profile)
{
    if ((*__profile)->state == PROFILE_STATE_IDLE)
    {
        return;
    }

    // ---------- Update Speed ----------

    float remaining_distance = fabsf((*__profile)->final_position) - fabsf((*__profile)->position);

    // To be honest PROFILE_STATE_ACCELERATING state is used not only for acceleration.
    // This flag stays set when the robot is already achived the target speed
    // and is traveling with constant velocity.
    // It is changed to PROFILE_STATE_DECELERATING, when the robot starts to decelerate.
    // That allows us to start decelerating even when we are not achieved the required
    // max_speed (variable from profile_start()). 
    // For example when we use the controller for really smal distances.
    if ((*__profile)->state == PROFILE_STATE_ACCELERATING)
    {
        if (remaining_distance <= distance_required_for_deceleration(__profile))
        {
            (*__profile)->state = PROFILE_STATE_DECELERATING;
            (*__profile)->target_speed = (*__profile)->final_speed;
        }
    }

    // The change in speed for one systick period
    // By the first equation of motion
    float change_in_speed = fabsf((*__profile)->acceleration * SYSTICK_PERIOD);

    // Accelerating
    if ((*__profile)->speed < (*__profile)->target_speed)
    {
        (*__profile)->speed += change_in_speed;
        if ((*__profile)->speed > (*__profile)->target_speed)
        {
            (*__profile)->speed = (*__profile)->target_speed;
        }
    }
    // Decelerating
    else if ((*__profile)->speed > (*__profile)->target_speed)
    {
        (*__profile)->speed -= change_in_speed;
        if ((*__profile)->speed < (*__profile)->target_speed)
        {
            (*__profile)->speed = (*__profile)->target_speed;
        }
    }

    // ---------- Update Position ----------

    // By the second equation of motion
    (*__profile)->position += (*__profile)->speed * SYSTICK_PERIOD;

    if ((*__profile)->state != PROFILE_STATE_FINISHED &&
        remaining_distance < ACCEPTABLE_DISTANCE_FOR_FINISH)
    {
        (*__profile)->state = PROFILE_STATE_FINISHED;
        (*__profile)->target_speed = (*__profile)->final_speed; 
    }
}

float distance_required_for_deceleration(profile_t **__profile)
{
    // By the third equation of motion
    return fabsf( (  (*__profile)->final_speed * (*__profile)->final_speed 
                  - (*__profile)->speed * (*__profile)->speed ) / (2 * (*__profile)->acceleration) );
}

uint8_t profile_is_finished(profile_t **__profile)
{
    return (*__profile)->state == PROFILE_STATE_FINISHED;
}

void profile_stop(profile_t **__profile)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        (*__profile)->target_speed = 0;
    }

    profile_finish(__profile);
}

void profile_finish(profile_t **__profile)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        (*__profile)->speed = (*__profile)->target_speed;
        (*__profile)->state = PROFILE_STATE_FINISHED;
    }
}

float profile_get_position(profile_t **__profile)
{
    float position;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        position = (*__profile)->position;
    }
    return position;
}

float profile_get_speed(profile_t **__profile)
{
    float speed;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        speed = (*__profile)->speed;
    }
    return speed;
}

float profile_get_distance_by_one_systick(profile_t **__profile)
{
    float distance_by_one_systick;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        distance_by_one_systick = (*__profile)->speed * SYSTICK_PERIOD;
    }
    return distance_by_one_systick;
}

float profile_get_acceleration(profile_t **__profile)
{
    float acceleration;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        acceleration = (*__profile)->acceleration;
    }
    return acceleration;
}

void profile_set_state(profile_t **__profile, uint8_t state)
{
    (*__profile)->state = state;
}

void profile_set_speed(profile_t **__profile, float speed)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        (*__profile)->speed = speed;
    }
}

void profile_set_target_speed(profile_t **__profile, float speed)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        (*__profile)->target_speed = speed;
    }
}

void profile_set_position(profile_t **__profile, float position)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        (*__profile)->position += position;
    }
}

void profile_adjust_position(profile_t **__profile, float adjustment)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        (*__profile)->position += adjustment;
    }
}
