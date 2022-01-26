#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>
#include "../../include/header.h"
#include "sensors.h"

int voltage_to_pwm(float voltage);
void init_pwm_for_motors();
void set_left_motor_pwm(int speed);
void set_right_motor_pwm(int speed);

#endif // MOTOR_DRIVER_H


