#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include "header.h"

// More information about the Teensy ADC Library
// https://forum.pjrc.com/threads/25532-ADC-library-update-now-with-support-for-Teensy-3-1
#include <ADC.h>

// More information about the Accelerometer and the Gyroscope 
// and the library that we are useing.
// https://learn.adafruit.com/lsm6ds33-6-dof-imu=accelerometer-gyro
// More information about Gyroscope calibration
// https://learn.adafruit.com/adafruit-sensorlab-gyroscope-calibration
#include <Adafruit_LSM6DS33.h>

// Pin definitions for the Adafruit_LSM6DS33.h library.
#define LSM_SCK SCL
#define LSM_MOSI SDA

// More information about the Encoders library
// https://www.pjrc.com/teensy/td_libs_Encoder.html
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

// For better understanding of the calculations:
// https://github.com/ukmars/ukmarsbot-examples/blob/master/11-distance-moved/README.md
#define PI 3.14159
#define ENCODER_COUNTS_PER_ROTATION 12
#define MOTOR_GEAR_RATIO            29.86 // 29.86 : 1
#define WHEEL_DIAMETER              32 // mm
// TODO: To be measured when the robot arrives
#define DISTANCE_BETWEEN_THE_WHEELS    65

#define COUNTS_PER_ROTATION ( ENCODER_COUNTS_PER_ROTATION *  MOTOR_GEAR_RATIO )
// We devide the MM_PER_COUNT by 2, because we will multiply 
// the sum of the two encoder values by MM_PER_COUNT and we want the average
#define MM_PER_COUNT        ( ( PI * WHEEL_DIAMETER ) / ( COUNTS_PER_ROTATION * 2 ) )
#define DEG_PER_COUNT       ( ( MM_PER_COUNT * 360) / ( PI * DISTANCE_BETWEEN_THE_WHEELS ) )

typedef struct {
    float distance;
    float angle;
} relative_position_t;

ADC *adc = new ADC();
Adafruit_LSM6DS33 lsm6ds33;
Encoder left_encoder(LEFT_MOTOR_ENCODER_A, LEFT_MOTOR_ENCODER_B);
Encoder right_encoder(RIGHT_MOTOR_ENCODER_A, RIGHT_MOTOR_ENCODER_B);

// It should probably be less than that. 
// TODO: Read the emitter datasheet
#define SENSOR_EMITTER_ON_TIME 320 // Microseconds

// Global Variables
uint16_t front_left_sensor = 0;
uint16_t front_right_sensor = 0;
uint16_t side_left_sensor = 0;
uint16_t side_right_sensor = 0;
uint16_t diagonal_left_sensor = 0;
uint16_t diagonal_right_sensor = 0;

float acc_x;
float acc_y;
float acc_z;

float gyro_x;
float gyro_y;
float gyro_z;

ADC::Sync_result emitter_on;
ADC::Sync_result emitter_off;

long previous_encoder_count_left = 0;
long previous_encoder_count_right = 0;

// Functions that take use of the ADC
void init_adc();
void read_proximity_sensors();
void read_proximity_sensors_front();
void read_proximity_sensors_diagonal();
void read_proximity_sensors_side();
float read_battery_voltage();

// Functions that take use of the 
// Accelerometer and/or the Gyroscope
void init_gyro_accel();
void read_gyroscope();
void read_accelerometer();

// Functions that take use of the Encoders
void clear_encoders();
relative_position_t get_ecnoder_position_change_since_last_read();
relative_position_t get_encoder_relative_position();


#endif // SENSORS_H