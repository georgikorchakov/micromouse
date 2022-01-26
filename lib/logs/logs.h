#ifndef LOGS_H
#define LOGS_H

#include <stdint.h>
#include <stdio.h>
#include "sensors.h"

#define MAX_TIMESTAMPS         35536
#define MAX_PROFILE_TIMESTAMPS 256
#define PROFILE_TYPE_SPEED     0
#define PROFILE_TYPE_ROTATION  1

typedef struct {
    uint64_t front_left_sensor      : 10;
    uint64_t front_right_sensor     : 10;
    uint64_t side_left_sensor       : 10;
    uint64_t side_right_sensor      : 10;
    uint64_t diagonal_left_sensor   : 10;
    uint64_t diagonal_right_sensor  : 10;  
    uint64_t cell_y                 : 4 ; // 8 bytes slot
    float gyroscope_z;                    // 4 bytes
    int8_t left_encoder_change;           // 1 byte
    int8_t right_encoder_change;          // 1 byte
    uint8_t cell_x                  : 4 ; // 4 bits
    uint8_t direction               : 2 ; // 2 bits
} timestamp_t;

typedef struct {
    float distance;     // 4 bytes
    float max_speed;    // 4 bytes
    float final_speed;  // 4 bytes
    float acceleration; // 4 bytes
    uint16_t time;      // 2 bytes
    uint8_t type : 1 ;  // 1 bit
    // 15 bits memory padding
} profile_timestamp_t;

extern timestamp_t timestamp_array[MAX_TIMESTAMPS];
extern profile_timestamp_t profile_timestamp_array[MAX_PROFILE_TIMESTAMPS];
extern uint16_t log_counter;
extern uint16_t log_profile_counter;

// This flag is set by the user
extern uint8_t logging_enabled;

// This flag is set by the user
extern uint8_t logging_profile_enabled;

// This flag is set when the robot start solving the maze
extern uint8_t start_logging;

void log_start();
void log_end();
void log_enable();
void log_disable();
void log_profile_enable();
void log_profile_disable();

void log_proximity_sensors_to_timestap(timestamp_t* timestamp, proximity_sensors_t **__proximity_sensors);
void log_encoders_change_to_timestap(timestamp_t* timestamp, long left_encoder_change, long right_encoder_change);
void log_position_in_the_maze_to_timestamp(timestamp_t* timestamp, uint8_t cell_x, uint8_t cell_y, uint8_t direction);
void log_gyroscope_z_to_timestamp(timestamp_t* timestamp, float gyroscope_z);
void log_profile(profile_timestamp_t* profile_timestamp, 
                 float distance, float max_speed, float final_speed, 
                 float acceleration, uint8_t profile_type);

// Be carefull that this is not a pointer.
void save_profile(profile_timestamp_t profile_timestamp);

// Be carefull that this is not a pointer.
void save_timestamp(timestamp_t timestamp);


#endif // LOGS_H