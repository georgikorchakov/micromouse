/*
 * File: main.cpp
 * Author: Georgi Korchakov
 * Date: 20.01.2022
 */

#include <Arduino.h>
#include "header.h"
#include "maze_mapping.h"
#include "serial_terminal.h"
#include "sensors.h"
#include "motor_driver.h"
#include "maze_solving.h"
#include "controller.h"
#include "movements.h"

#define USER_MODE             0
#define SEARCH_MODE           1
#define RETURN_SEARCH_MODE    2
#define SEARCH_ENDED_MODE     3
#define FAST_SOLVE_MODE       4
#define FAST_RETURN_MODE      5
#define FAST_SOLVE_ENDED_MODE 6

uint8_t robot_mode = USER_MODE;

proximity_sensors_t* proximity_sensors = NULL;

// Position in the maze
uint8_t x = 0;
uint8_t y = 0;
uint8_t direction = NORTH;
uint8_t next_step = DIRECTION_FORWARD;

uint8_t walls;
uint8_t front_wall;
uint8_t left_wall;
uint8_t right_wall;


void setup() 
{  
    Wire.begin();

    imu.enableDefault();

    init_maze();
    init_serial_terminal();

    init_adc();
    init_proximity_sensors();
    proximity_sensors = init_proximity_sensors_struct();

    init_buttons();
    init_encoders();
    init_gyro_accel();

    init_pwm_for_motors();
    set_left_motor_pwm(0);
    set_right_motor_pwm(0);

    // Controll Loop Initialization
    init_control_system();
    stop_controller_output();
    disable_systick_function();

    pinMode(BUZZER, OUTPUT);

    delay(3000);
}

void user_mode()
{
    struct push_buttons_t push_buttons;
    push_buttons = read_push_buttons();

    if (push_buttons.up == 1)
    {   
        if (robot_mode == USER_MODE)
        {
            robot_mode = SEARCH_MODE;
        }
        else if (robot_mode == SEARCH_ENDED_MODE)
        {
            robot_mode = FAST_SOLVE_MODE;
        }
        else if (robot_mode == FAST_SOLVE_ENDED_MODE)
        {
            robot_mode = FAST_SOLVE_ENDED_MODE;
        }
    }
    else
    {
        serial_terminal();
    }
}

// int8_t two_bit(int8_t bits)
// {
//     if (bits < 0)
//     {
//         bits = 3;
//     }
//     else if (bits > 3)
//     {
//         bits = 0;
//     }

//     return bits;
// }

void search_mode()
{
    // Prepare the user
    delay(3000);
    tone(BUZZER, 1000);
    delay(1000);
    noTone(BUZZER);

    // Start the controller
    start_systick_interrupt();
    start_controller_output();
    //enable_systick_function();

    // Position in the center of the start cell
    move_forward_from_the_start();

    while (1)
    {
        // Read Sensors (Later that will be executed in the systick)
        read_proximity_sensors(&proximity_sensors);

        // Extrect the individual Walls
        left_wall = get_left_wall(&proximity_sensors);
        right_wall = get_right_wall(&proximity_sensors);
        front_wall = get_front_wall(&proximity_sensors);

        // Normalize the walls direction
        walls = synchronize_walls_with_direction(
            left_wall, front_wall, right_wall, 
            direction
        );

        // Update the maze matrix
        update_cell_walls(x, y, walls);

        // Use algorithm to find the next step
        next_step = right_hand_rule(left_wall, front_wall, right_wall);

        if (next_step == DIRECTION_FORWARD)
        {
            one_cell_forward();
            x = update_x(x, direction);
            y = update_y(y, direction);
        }
        else if (next_step == DIRECTION_LEFT_FORWARD)
        {
            left_pivot_turn();
            one_cell_forward();

            direction = update_direction(direction, LEFT);
            x = update_x(x, direction);
            y = update_y(y, direction);
        }
        else if (next_step == DIRECTION_RIGHT_FORWARD)
        {
            right_pivot_turn();
            one_cell_forward();

            direction = update_direction(direction, RIGHT);
            x = update_x(x, direction);
            y = update_y(y, direction);
            
        }
        else if (next_step == DIRECTION_LEFT)
        {
            left_pivot_turn();

            direction = update_direction(direction, LEFT);
        }
        else if (next_step == DIRECTION_RIGHT)
        {
            right_pivot_turn();

            direction = update_direction(direction, RIGHT);
        }

        struct push_buttons_t push_buttons;
        push_buttons = read_push_buttons();

        if (x == 0 && y == 0)
        {
            break;
        } 
        else if (push_buttons.up == 1)
        {   
            break;
        }
    }

    // Exit search mode

    //disable_systick_function();
    stop_controller_output();
    stop_systick_interrupt();

    set_left_motor_pwm(0);
    set_right_motor_pwm(0); 

    robot_mode = USER_MODE;

    tone(BUZZER, 1000);
    delay(2000);
    noTone(BUZZER);
}

void loop()
{   
    switch (robot_mode)
    {
        case USER_MODE:
        case SEARCH_ENDED_MODE:
        case FAST_SOLVE_ENDED_MODE:
            user_mode();
            break;
        case SEARCH_MODE:
            search_mode();
            //test_imaginary_maze();
            //search_mode();
            break;
    }
}
