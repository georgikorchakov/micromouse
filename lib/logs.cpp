/*
 * This file is used to collect run time data from the system.
 * After that we can use the data to make statistics.
 *
 * The logging program is kinda tricky.
 * If we log the status of the sensors and
 * profilers we can easily run out of memory.
 * Because of that a couple of memory optimization tecnuques should be used.
 * 
 * Really good tutorial of how to optimize memory footprint of c structs
 * http://www.catb.org/esr/structure-packing/
 * First of all:
 * By this tutorial, the members of the timestamp_t structs are orderd 
 * by specific rules. It looks ugly, but it saves aloooot of memory.
 * 
 * Second of all:
 * Because we will take timestamp of the system on every 1ms (the systick interupt),
 * we don't need to store the time of the timestamp in the struct, because we
 * can use instead, the index of the array containing the timestamps.
 * So basicly we erase 2 bytes of space on every timestamp.
 * 
 * Third of all:
 * Every proximity sensor value is 10bit, but is stored in 16bit variable, because
 * there is no 10bit variable. That waste 4bits in every sensor.
 * So we will use bit fields (https://www.tutorialspoint.com/cprogramming/c_bit_fields.htm),
 * 
 * After that:
 * We will store the profile updates in separate struct 
 * and there would be vatiable for the time, so we can match it with timestamp_t.
 * 
 * For the Gyroscope value, we use only the Z axis, so se only need to store that.
 * 
 * For the encoder values, there is no point of storing the position.
 * Instead we will store only the difference in the encoder counts between the interrupts.
 * 
 * With this information stored, we can calculate everything else in the system. 
 * 
 */

#include "logs.h"

timestamp_t timestamp_array[MAX_TIMESTAMPS];
profile_timestamp_t profile_timestamp_array[MAX_PROFILE_TIMESTAMPS];
uint16_t log_counter = 0;
uint8_t log_profile_counter = 0;

// This flag is set by the user
uint8_t logging_enabled = false;

// This flag is set by the user
uint8_t logging_profile_enabled = false;

// This flag is set when the robot start solving the maze
uint8_t start_logging = false;

void log_start() { start_logging = true; }
void log_end() { start_logging = false; }
void log_enable() { logging_enabled = true; }
void log_disable() { logging_enabled = false; }
void log_profile_enable() { logging_profile_enabled = true; }
void log_profile_disable() { logging_profile_enabled = false; }

void log_proximity_sensors_to_timestap(timestamp_t* timestamp, proximity_sensors_t* proximity_sensors)
{
    if (logging_enabled && start_logging)
    {
        timestamp->front_left_sensor = proximity_sensors->front_left_sensor;
        timestamp->front_right_sensor = proximity_sensors->front_right_sensor;
        timestamp->side_left_sensor = proximity_sensors->side_left_sensor;
        timestamp->side_right_sensor = proximity_sensors->side_right_sensor;
        timestamp->diagonal_left_sensor = proximity_sensors->diagonal_left_sensor;
        timestamp->diagonal_right_sensor = proximity_sensors->diagonal_right_sensor;
    }
}

void log_encoders_change_to_timestap(timestamp_t* timestamp, long left_encoder_change, long right_encoder_change)
{
    if (logging_enabled && start_logging)
    {
        timestamp->left_encoder_change = (int8_t)left_encoder_change;
        timestamp->right_encoder_change = (int8_t)right_encoder_change;
    }
}

void log_position_in_the_maze_to_timestamp(timestamp_t* timestamp, uint8_t cell_x, uint8_t cell_y, uint8_t direction)
{
    if (logging_enabled && start_logging)
    {
        timestamp->cell_x = cell_x;
        timestamp->cell_y = cell_y;
        timestamp->direction = direction;
    }
}

void log_gyroscope_z_to_timestamp(timestamp_t* timestamp, float gyroscope_x)
{
    if (logging_enabled && start_logging)
    {
        timestamp->gyroscope_x = gyroscope_x;
    }
}

void log_profile(profile_timestamp_t* profile_timestamp, 
                 float distance, float max_speed, float final_speed, 
                 float acceleration, uint8_t profile_type)
{
    if (logging_profile_enabled && start_logging)
    {
        profile_timestamp->distance = distance;
        profile_timestamp->max_speed = max_speed;
        profile_timestamp->final_speed = final_speed;
        profile_timestamp->acceleration = acceleration;
        profile_timestamp->time = log_counter;
        profile_timestamp->type = profile_type;
    }
}

// Be carefull that this is not a pointer.
void save_profile(profile_timestamp_t profile_timestamp)
{
    if (logging_profile_enabled && start_logging)
    {
        profile_timestamp_array[log_profile_counter++] = profile_timestamp;
    }
}

// Be carefull that this is not a pointer.
void save_timestamp(timestamp_t timestamp)
{
    if (logging_enabled && start_logging)
    {
        timestamp_array[log_counter++] = timestamp;
    }
}

// int main()
// {
//     // sizeof evaluates the size of a variable
//     printf("Size of my struct: %zu bytes\n", sizeof(timestamp_t)*35536);
//     printf("Size of float: %zu bytes\n", sizeof(float));

//     return 0;
// }
