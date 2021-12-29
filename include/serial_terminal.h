#ifndef SERIAL_TERMINAL_H
#define SERIAL_TERMINAL_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "maze_mapping.h"

#define SERIAL_TERMINAL_BUFSIZE 64
#define SERIAL_TERMINAL_TOKEN_BUFSIZE 16
#define SERIAL_TERMINAL_TOK_DELIM " \t\r\n\a"

// Really good tutorial of how to implement linux shell
// https://brennan.io/2015/01/16/write-a-shell-in-c/

////////////////////////////////////////////////////////////
// Enable Command
////////////////////////////////////////////////////////////

int enable(char** args);


////////////////////////////////////////////////////////////
// Set Command
////////////////////////////////////////////////////////////

int set(char** args);


////////////////////////////////////////////////////////////
// Show Command
////////////////////////////////////////////////////////////

int num_builtins_show();
int show(char** args);
void show_man();
int show_maze(char** args);
int show_best_path(char** args);
int show_optimization(char** args);
int show_speed_profile(char** args);
int show_rotational_profile(char** args);
int show_battery_voltage(char** args);
int show_proximity(char** args);
int show_gyroscope(char** args);
int show_accelerometer(char** args);
int show_encoders(char** args);
int show_stop(char** args);

char* show_functions_str[] = {
    "maze",
    "best", // best path
    "optimization",
    "speed", // speed profile
    "rotational", // rotational profile
    "battery", // battery voltage
    "proximity",
    "gyroscope",
    "accelerometer",
    "encoders",
    "stop"
};

int (*show_functions[]) (char**) = {
    &show_maze,
    &show_best_path,
    &show_optimization,
    &show_speed_profile,
    &show_rotational_profile,
    &show_battery_voltage,
    &show_proximity,
    &show_gyroscope,
    &show_accelerometer,
    &show_encoders,
    &show_stop
};

////////////////////////////////////////////////////////////
// Help Command
////////////////////////////////////////////////////////////

int help(char** args);


////////////////////////////////////////////////////////////
// Man Command
////////////////////////////////////////////////////////////

int man(char** args);


////////////////////////////////////////////////////////////
// Inspect Command
////////////////////////////////////////////////////////////

int inspect(char** args);
void inspect_man();

////////////////////////////////////////////////////////////
// Shell interface
////////////////////////////////////////////////////////////

char* builtin_str[] = {
    "help",
    "show",
    "enable",
    "set",
    "inspect",
    "man"
};

int (*builtin_func[]) (char**) = {
    &help,
    &show,
    &enable,
    &set,
    &inspect,
    &man
};

int num_builtins();
int execute(char** args);
char* read_line();
char** split_line(char* line);
void serial_terminal();

/*
 *  List of functions to be implemented.
 *  help
 *  show - help
 *  show maze - open maze
 *  show maze -t open - open maze
 *  show maze -t closed - closed maze
 *  show best path - current best path
 *  show best path -o les turns
 *  show best path -o diagonals
 *  show best path -o shortest
 *  show current optimization
 *  show current speed profile
 *  show current rotational profile
 *  show current battery voltage
 *  show proximity -s [sensor] -t continuous 
 *  show gyroscope -a [axis] -t continuous
 *  show accelerometer -a [axis] -t continuous
 *  show encoders -e [encoder] -t continuous
 *  show stop
 * 
 *  enable loging -t full 
 *  enable loging -t PD controller 
 *  enable loging -t acceleration
 *  enable loging -t gyroscope
 *  enable loging -t encoder
 *  
 *  set speed P [value]
 *  set speed D [value]
 *  set rotation P [value]
 *  set rotation D [value]
 *  set max speed [value]
 *  set max rotation speed [value]
 *  set optimization [les turns | diagonals | shortest]
 * 
 *  inspect controller
 *  
 */


#endif // SERIAL_TERMINAL_H