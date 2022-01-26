#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include "../../include/header.h"

// More information about the Teensy ADC Library
// https://forum.pjrc.com/threads/25532-ADC-library-update-now-with-support-for-Teensy-3-1
#include <ADC.h>

// More information about the Accelerometer and the Gyroscope 
// and the library that we are useing.
// https://learn.adafruit.com/lsm6ds33-6-dof-imu=accelerometer-gyro
// More information about Gyroscope calibration
// https://learn.adafruit.com/adafruit-sensorlab-gyroscope-calibration
//#include <Adafruit_LSM6DS33.h>

// Faster library for reading the gyroscope
#include <Wire.h>
#include <LSM6.h>

// Pin definitions for the Adafruit_LSM6DS33.h library.
#define LSM_SCK SCL
#define LSM_MOSI SDA

// These four lines are used to correct an error in the encoder library
// More information: https://github.com/PaulStoffregen/Encoder/issues/44
#define IO_REG_TYPE			            uint32_t
#define PIN_TO_BASEREG(pin)             (portInputRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define DIRECT_PIN_READ(base, mask)     (((*(base)) & (mask)) ? 1 : 0)

// More information about the Encoders library
// https://www.pjrc.com/teensy/td_libs_Encoder.html
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

// For better understanding of the calculations:
// https://github.com/ukmars/ukmarsbot-examples/blob/master/11-distance-moved/README.md
#define PI 3.14159
#define ENCODER_COUNTS_PER_ROTATION 12.0
#define MOTOR_GEAR_RATIO            29.86 // 29.86 : 1
#define WHEEL_DIAMETER              32 //32.0 // mm
// TODO: To be measured when the robot arrives
#define DISTANCE_BETWEEN_THE_WHEELS   71 //71.5 // 65.0

#define COUNTS_PER_ROTATION ( ENCODER_COUNTS_PER_ROTATION *  MOTOR_GEAR_RATIO )
// We devide the MM_PER_COUNT by 2, because we will multiply 
// the sum of the two encoder values by MM_PER_COUNT and we want the average
#define MM_PER_COUNT        ( ( PI * WHEEL_DIAMETER ) / ( COUNTS_PER_ROTATION * 2 ) )
#define DEG_PER_COUNT       ( ( MM_PER_COUNT * 360) / ( PI * DISTANCE_BETWEEN_THE_WHEELS ) )

typedef struct {
    float distance;
    float angle;
} relative_position_t;

extern ADC *adc;
//extern Adafruit_LSM6DS33 lsm6ds33;
extern LSM6 imu;
extern Encoder *left_encoder;
extern Encoder *right_encoder;

// It should probably be less than that. 
// TODO: Read the emitter datasheet
#define SENSOR_EMITTER_ON_TIME 100 // Microseconds 120

// Calibration of the sensors
#define LEFT_WALL_THRESHOLD  450
#define RIGHT_WALL_THRESHOLD 450
#define FRONT_WALL_THRESHOLD 450

// Global Variables
typedef struct {
    volatile uint16_t front_left_sensor;
    volatile uint16_t front_right_sensor;
    volatile uint16_t side_left_sensor;
    volatile uint16_t side_right_sensor;
    volatile uint16_t diagonal_left_sensor;
    volatile uint16_t diagonal_right_sensor;
} proximity_sensors_t;

struct push_buttons_t {
    uint8_t up     : 1;
    uint8_t down   : 1;
    uint8_t center : 1;
    uint8_t left   : 1;
    uint8_t right  : 1;
};

extern float acc_x;
extern float acc_y;
extern float acc_z;

extern float gyro_x;
extern float gyro_y;
extern float gyro_z;

extern ADC::Sync_result emitter_on;
extern ADC::Sync_result emitter_off;

extern long previous_encoder_count_left;
extern long previous_encoder_count_right;

// Functions that take use of the ADC
void init_adc();
void init_proximity_sensors();
proximity_sensors_t* init_proximity_sensors_struct();

void read_proximity_sensors(proximity_sensors_t **__proximity_sensors);
void read_proximity_sensors_front(proximity_sensors_t **__proximity_sensors);
void read_proximity_sensors_diagonal(proximity_sensors_t **__proximity_sensors);
void read_proximity_sensors_side(proximity_sensors_t **__proximity_sensors);
uint8_t get_left_wall(proximity_sensors_t **__proximity_sensors);
uint8_t get_right_wall(proximity_sensors_t **__proximity_sensors);
uint8_t get_front_wall(proximity_sensors_t **__proximity_sensors);
float read_battery_voltage();
void init_buttons();
struct push_buttons_t read_push_buttons();

// Functions that take use of the 
// Accelerometer and/or the Gyroscope
void init_gyro_accel();
void read_gyroscope();
void read_accelerometer();

// Functions that take use of the Encoders
void init_encoders();
void clear_encoders();
relative_position_t get_ecnoder_position_change_since_last_read();
relative_position_t get_encoder_relative_position();


#endif // SENSORS_H