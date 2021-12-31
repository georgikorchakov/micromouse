#ifndef MAZE_SOLVING_H
#define MAZE_SOLVING_H

#define DIRECTION_FORWARD         0
#define DIRECTION_LEFT_FORWARD    1
#define DIRECTION_RIGHT_FORWARD   2
#define DIRECTION_LEFT            3
#define DIRECTION_RIGHT           4

/*
 * All of these algorithms are unable to solve the 
 * standard competition maze, but are very handy for testing the robot.
 */
uint8_t central_right_rule(uint8_t left_wall, uint8_t front_wall, uint8_t right_wall);
uint8_t central_left_rule(uint8_t left_wall, uint8_t front_wall, uint8_t right_wall);
uint8_t left_hand_rule(uint8_t left_wall, uint8_t front_wall, uint8_t right_wall);
uint8_t right_hand_rule(uint8_t left_wall, uint8_t front_wall, uint8_t right_wall);

#endif // MAZE_SOLVING_H