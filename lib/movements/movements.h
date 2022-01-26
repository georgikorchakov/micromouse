#ifndef MOVEMENTS_H
#define MOVEMENTS_H

#include <Arduino.h>
#include "../../include/header.h"
#include "controller.h"

// Straight line
// Pivot turn
// Curve turn
// Diagonals

void one_cell_forward();
void forward_cells(int number_of_cells);
void left_pivot_turn();
void right_pivot_turn();
void move_forward_from_the_start();


#endif // MOVEMENTS_H