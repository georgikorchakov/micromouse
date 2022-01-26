#ifndef MAZE_MAPPING_H
#define MAZE_MAPPING_H

#include <Arduino.h>
#include <stdint.h>
#include <stdlib.h>

#define MAZE_SIZE_X 16
#define MAZE_SIZE_Y 16

#define WEST    0
#define SOUTH   1
#define EAST    2
#define NORTH   3

#define LEFT    0
#define RIGHT   1

#define WEST_WALL    (1 << 0)
#define SOUTH_WALL   (1 << 1)
#define EAST_WALL    (1 << 2)
#define NORTH_WALL   (1 << 3)

#define OPEN_MAZE   0
#define CLOSED_MAZE 4

extern uint8_t maze[MAZE_SIZE_Y][MAZE_SIZE_X];

void init_maze();
void print_maze(uint8_t maze_select);
uint8_t get_maze_cell(uint8_t x, uint8_t y);
void set_maze_cell(uint8_t x, uint8_t y, uint8_t cell);
uint8_t synchronize_walls_with_direction(uint8_t left_wall, uint8_t front_wall, uint8_t right_wall, uint8_t direction);
int8_t two_bit(int8_t bits);
uint8_t update_direction(uint8_t direction, uint8_t turn);
uint8_t update_y(uint8_t y, uint8_t direction);
uint8_t update_x(uint8_t x, uint8_t direction);

void print_maze_advanced(uint8_t maze_select);
void update_cell_single_wall(uint8_t x, uint8_t y, uint8_t add_wall, uint8_t direction);
uint8_t add_maze_border(uint8_t x, uint8_t y);
void update_cell_walls(uint8_t x, uint8_t y, uint8_t walls);
void debug_maze_matrix();

#endif // MAZE_MAPPING_H