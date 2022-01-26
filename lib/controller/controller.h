#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include "../../include/header.h"
#include "sensors.h"
#include "motor_driver.h"
#include <util/atomic.h>

// This library provides us a regular interupt capabilities
#include <IntervalTimer.h>

// In order to keep track of the changes and
//#include "logs.h"

#define PROFILE_STATE_IDLE         0
#define PROFILE_STATE_ACCELERATING 1
#define PROFILE_STATE_DECELERATING 2
#define PROFILE_STATE_FINISHED     3

#define SYSTICK_PERIOD 0.001//f // 1ms
#define SYSTICK_INTERRUPT_TIME_MICRO_SECONDS 1000
#define SYSTICK_INTERRUPT_PRIORITY 130
#define ACCEPTABLE_DISTANCE_FOR_FINISH 0.3 // mm

#define KP_S 2.5//5//7.0 // 2.0
#define KD_S 3.0
#define KP_R  0.3 //0.5
#define KD_R 3//3//2 //2.0

extern IntervalTimer systick_interrupt;

extern float speed_error;
extern float rotation_error;
extern float previous_speed_error;
extern float previous_rotation_error;
extern volatile uint8_t controller_output_enable;
extern volatile uint8_t systick_function_enable;

extern float min_e;
extern float max_e;

// Profiler struct
typedef struct {
    volatile float speed;
    volatile float position;
    float final_position;
    float target_speed;
    float final_speed;
    float acceleration;
    volatile uint8_t state;
    int8_t sign;
} profile_t;

extern profile_t *speed_profile;
extern profile_t *angle_profile;

// This function will be executed with the interupts 
// from the IntervalTimer.h library on every 1 milisecond
void systick();

float speed_controller(profile_t **__speed_profile, float real_distance_change);
float rotation_controller(profile_t **__rotation_profile, float real_angle_change);

uint8_t start_systick_interrupt();
void stop_systick_interrupt();

void start_controller_output();
void stop_controller_output();

void enable_systick_function();
void disable_systick_function();

////////////////////////////////////////////////////////////
// Profiler
////////////////////////////////////////////////////////////

void init_control_system();

profile_t* init_profile_struct();
void profile_start(profile_t **__profile, float distance, float max_speed, float final_speed, float acceleration);
void profile_stop(profile_t **__profile);
void profile_finish(profile_t **__profile);
void profile_set_state(profile_t **__profile, uint8_t state);
float profile_get_position(profile_t **__profile);
float profile_get_speed(profile_t **__profile);
float profile_get_distance_by_one_systick(profile_t **__profile);
uint8_t profile_is_finished(profile_t **__profile);
float profile_get_acceleration(profile_t **__profile);
void profile_set_speed(profile_t **__profile, float speed);
void profile_set_target_speed(profile_t **__profile, float speed);
void profile_adjust_position(profile_t **__profile, float adjustment);
void profile_set_position(profile_t **__profile, float position);
float distance_required_for_deceleration(profile_t **__profile);
void profile_update(profile_t **__profile);


#endif // CONTROLLER_H