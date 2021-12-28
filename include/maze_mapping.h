#ifndef MAZE_MAPPING_H
#define MAZE_MAPPING_H

#include <stdio.h>
#include <stdint.h>

#define MAZE_SIZE_X 16
#define MAZE_SIZE_Y 16

#define WEST    0
#define SOUTH   1
#define EAST    2
#define NORTH   3

#define WEST_WALL    (1 << 0)
#define SOUTH_WALL   (1 << 1)
#define EAST_WALL    (1 << 2)
#define NORTH_WALL   (1 << 3)

#define OPEN_MAZE   0
#define CLOSED_MAZE 4

uint8_t maze[MAZE_SIZE_Y][MAZE_SIZE_X] = {{0}};

void init_maze();
void print_maze(uint8_t maze_select);
uint8_t get_maze_cell(uint8_t x, uint8_t y);
void set_maze_cell(uint8_t x, uint8_t y, uint8_t cell);
void print_maze_advanced(uint8_t maze_select);
void update_cell_single_wall(uint8_t x, uint8_t y, uint8_t add_wall, uint8_t direction);
void update_cell_walls(uint8_t x, uint8_t y, uint8_t walls);
void debug_maze_matrix();

#endif // MAZE_MAPPING_H