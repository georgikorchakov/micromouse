#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include "header.h"
#include "sensors.h"
#include "motor_driver.h"
#include <util/atomic.h>

// This library provides us a regular interupt capabilities
#include <IntervalTimer.h>

#define PROFILE_STATE_IDLE         0
#define PROFILE_STATE_ACCELERATING 1
#define PROFILE_STATE_DECELERATING 2
#define PROFILE_STATE_FINISHED     3

#define SYSTICK_PERIOD 0.001 // 1ms
#define ACCEPTABLE_DISTANCE_FOR_FINISH 0.125 // mm

#define KP_S 1f
#define KD_S 0f
#define KP_R 1f
#define KD_R 0f

IntervalTimer myTimer;

extern float speed_error;
extern float rotation_error;
extern float previous_speed_error;
extern float previous_rotation_error;
extern volatile uint8_t controller_output_enable;

// This function will be executed with the interupts 
// from the IntervalTimer.h library on every 1 milisecond
void systick();

float speed_controller(profile_t* speed_profile, float real_distance_change);
float rotation_controller(profile_t* rotation_profile, float real_angle_change);
void start_controller_output();
void stop_controller_output();

////////////////////////////////////////////////////////////
// Profiler
////////////////////////////////////////////////////////////

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

extern profile_t* speed_profile;
extern profile_t* angle_profile;

profile_t* init_profile();
void profile_start(profile_t* profile, float distance, float max_speed, float final_speed, float acceleration);
void profile_stop(profile_t* profile);
void profile_finish(profile_t* profile);
void profile_set_state(profile_t* profile, uint8_t state);
float profile_get_position(profile_t* profile);
float profile_get_speed(profile_t* profile);
float profile_get_distance_by_one_systick(profile_t* profile);
float profile_get_acceleration(profile_t* profile)
void profile_set_speed(profile_t* profile, float speed);
void profile_set_target_speed(profile_t* profile, float speed);
void profile_adjust_position(profile_t* profile, float adjustment);
void profile_set_position(profile_t* profile, float position);
float distance_required_for_deceleration(profile_t* profile);
void profile_update(profile_t* profile);


#endif // CONTROLLER_H