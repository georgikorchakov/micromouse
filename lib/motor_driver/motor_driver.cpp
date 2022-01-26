#include "motor_driver.h"

int voltage_to_pwm(float voltage)
{
    int sign_multiplier = 1;
    if (voltage < 0)
    {
        sign_multiplier = -1;
        voltage = -voltage;
    }

    float battery_voltage = read_battery_voltage();
    if (voltage > battery_voltage)
    {
        return PWM_MAX_VALUE * sign_multiplier;
    }

    float multiplier = PWM_MAX_VALUE / battery_voltage;

    return (int)( voltage * multiplier * sign_multiplier );
}

void init_pwm_for_motors()
{
    pinMode(B_ENABLE, OUTPUT);
    pinMode(B_PHASE, OUTPUT);
    pinMode(A_ENABLE, OUTPUT);
    pinMode(A_PHASE, OUTPUT);

    // We are aiming for higher resolution, 
    // so we will have higher precision
    analogWriteResolution(PWM_RESOLUTION);

    // We are aiming for higher pwm frequency,
    // because that will create smoother reactions
    analogWriteFrequency(A_ENABLE, PWM_FREQUENCY);
    analogWriteFrequency(B_ENABLE, PWM_FREQUENCY);
}

void set_left_motor_pwm(int speed)
{
    if (speed < 0)
    {
        digitalWriteFast(A_PHASE, LOW);
        speed = abs(speed);
    }
    else
    {
        digitalWriteFast(A_PHASE, HIGH);
    }

    analogWrite(A_ENABLE, speed);
}

void set_right_motor_pwm(int speed)
{
    if (speed < 0)
    {
        digitalWriteFast(B_PHASE, HIGH);
        speed = abs(speed);
    }
    else
    {
        digitalWriteFast(B_PHASE, LOW);
    }

    analogWrite(B_ENABLE, speed);
}
