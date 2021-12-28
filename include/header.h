/*
 * header.h
 *
 * Georgi Korchakov
 * 23.12.2021
 * 
 * This file contains all global definitions 
 * and variables.
 */

#ifndef HEADER_H
#define HEADER_H

// GPIO definitions and functions

#define BUTTON1                 0
#define BUTTON2                 1
// Three more buttons are connected to this pin as a voltage devider 
// TODO: Add more information
#define ADDITIONAL_BUTTONS      A8

// These pins control mosfets that provide 
// power to the emiters of the proximity sensors.
// Before the value of the sensor is measured, 
// a digital HIGH should be provided to the coresponding emiter.
//
// WARNING: NEVER LIGHT UP ALL OF THE EMITERS SIMULTANIEUSLY, 
// BECAUSE THE VOLTAGE REGULATOR MAY FAIL TO SUPPLY THE REQUIRED 
// CURRENT AND THE MICROCONTROLLER MAY REBOOT
//
// WARNING: NEVER POWER THE EMITERS FOR MORE THAN 
// A COUPLE OF MICROSECONDS, BACAUSE DAMAGE OF THE SENSORS MAY OCCUR
#define FRONT_DIODES            13
#define SIDE_DIODES             2
#define DIAGONAL_DIODES         4

#define SENSOR_FRONT_LEFT       A7
#define SENSOR_FRONT_RIGHT      A0
#define SENSOR_SIDES_LEFT       A6
#define SENSOR_SIDES_RIGHT      A1
#define SENSOR_DIAGONALS_LEFT   A3
#define SENSOR_DIAGONALS_RIGHT  A2

#define BUZZER                  3

// B_ENABLE pin is connected to FlexPWM2.1
#define B_ENABLE                5
#define B_PHASE                 6
// A_ENABLE pin is connected to FlexPWM1.3, as pin 8 and 25
// When we change pwm configuration, it will change for all
// pins connected to the same timer
#define A_ENABLE                7
#define A_PHASE                 8

#define LEFT_MOTOR_ENCODER_A    10
#define LEFT_MOTOR_ENCODER_B    11
#define RIGHT_MOTOR_ENCODER_A   9
#define RIGHT_MOTOR_ENCODER_B   12

// A voltage devider is attached to this pin, that measures the battery voltage.
// R1 = 18k 1%, R2 = 13k 1%
// By the voltage devider formula, when the input voltage is 7.869V, 
// the voltage provided to the BATTERY_VOLTAGE_LEVEL pin will be 3.3V.
#define BATTERY_VOLTAGE_LEVEL   A9

// TODO: Provide more information
#define SCL                     19
#define SDA                     18

#define PWM_RESOLUTION 12
#define PWM_MAX_VALUE  4095
#define PWM_FREQUENCY  36621.09

#endif // HEADER_h