#include "maze_solving.h"


//  left | front | right | output
//  0    | 0     | 0     | forward
//  0    | 0     | 1     | forward
//  0    | 1     | 0     | right_forward
//  0    | 1     | 1     | left_forward
//  1    | 0     | 0     | forward
//  1    | 0     | 1     | forward
//  1    | 1     | 0     | right_forward
//  1    | 1     | 1     | right
uint8_t central_right_rule(uint8_t left_wall, uint8_t front_wall, uint8_t right_wall)
{
    if (!left_wall && !front_wall && !right_wall)
    {
        return DIRECTION_FORWARD;
    }
    else if (!left_wall && !front_wall && right_wall)
    {
        return DIRECTION_FORWARD;
    }
    else if (!left_wall && front_wall && !right_wall)
    {
        return DIRECTION_RIGHT_FORWARD;
    }
    else if (!left_wall && front_wall && right_wall)
    {
        return DIRECTION_LEFT_FORWARD;
    }
    else if (left_wall && !front_wall && !right_wall)
    {
        return DIRECTION_FORWARD;
    }
    else if (left_wall && !front_wall && right_wall)
    {
        return DIRECTION_FORWARD;
    }
    else if (left_wall && front_wall && !right_wall)
    {
        return DIRECTION_RIGHT_FORWARD;
    }
    else if (left_wall && front_wall && right_wall)
    {
        return DIRECTION_RIGHT;
    }
}


//  left | front | right | output
//  0    | 0     | 0     | forward
//  0    | 0     | 1     | forward
//  0    | 1     | 0     | left_forward
//  0    | 1     | 1     | left_forward
//  1    | 0     | 0     | forward
//  1    | 0     | 1     | forward
//  1    | 1     | 0     | right_forward
//  1    | 1     | 1     | left
uint8_t central_left_rule(uint8_t left_wall, uint8_t front_wall, uint8_t right_wall)
{
    if (!left_wall && !front_wall && !right_wall)
    {
        return DIRECTION_FORWARD;
    }
    else if (!left_wall && !front_wall && right_wall)
    {
        return DIRECTION_FORWARD;
    }
    else if (!left_wall && front_wall && !right_wall)
    {
        return DIRECTION_LEFT_FORWARD;
    }
    else if (!left_wall && front_wall && right_wall)
    {
        return DIRECTION_LEFT_FORWARD;
    }
    else if (left_wall && !front_wall && !right_wall)
    {
        return DIRECTION_FORWARD;
    }
    else if (left_wall && !front_wall && right_wall)
    {
        return DIRECTION_FORWARD;
    }
    else if (left_wall && front_wall && !right_wall)
    {
        return DIRECTION_RIGHT_FORWARD;
    }
    else if (left_wall && front_wall && right_wall)
    {
        return DIRECTION_LEFT;
    }
}

//  left | front | right | output
//  0    | 0     | 0     | left_forward
//  0    | 0     | 1     | left_forward
//  0    | 1     | 0     | left_forward
//  0    | 1     | 1     | left_forward
//  1    | 0     | 0     | forward
//  1    | 0     | 1     | forward
//  1    | 1     | 0     | right_forward
//  1    | 1     | 1     | right
uint8_t left_hand_rule(uint8_t left_wall, uint8_t front_wall, uint8_t right_wall)
{
    if (!front_wall && left_wall && right_wall)
    {
        return DIRECTION_FORWARD;
    }
    else if (!left_wall && front_wall && right_wall)
    {
        return DIRECTION_LEFT_FORWARD;
    }
    else if (!left_wall && !right_wall && front_wall)
    {
        return DIRECTION_LEFT_FORWARD;
    }
    else if (!left_wall && !front_wall && !right_wall)
    {
        return DIRECTION_LEFT_FORWARD;
    }
    else if (!right_wall && left_wall && front_wall)
    {
        return DIRECTION_RIGHT_FORWARD;
    }
    else if (!left_wall && !front_wall && right_wall)
    {
        return DIRECTION_LEFT_FORWARD;
    }
    else if (!front_wall && !right_wall && left_wall)
    {
        return DIRECTION_FORWARD;
    }
    else if (left_wall && front_wall && right_wall)
    {
        return DIRECTION_RIGHT;
    }
}

//  left | front | right | output
//  0    | 0     | 0     | right_forward
//  0    | 0     | 1     | forward
//  0    | 1     | 0     | right_forward
//  0    | 1     | 1     | left_forward
//  1    | 0     | 0     | right_forward
//  1    | 0     | 1     | forward
//  1    | 1     | 0     | right_forward
//  1    | 1     | 1     | left
uint8_t right_hand_rule(uint8_t left_wall, uint8_t front_wall, uint8_t right_wall)
{
    if (!front_wall && right_wall && left_wall)
    {
        return DIRECTION_FORWARD;
    }
    else if (!front_wall && !right_wall && left_wall)
    {
        return DIRECTION_RIGHT_FORWARD;
    }
    else if (!right_wall && !left_wall && front_wall)
    {
        return DIRECTION_RIGHT_FORWARD;
    }
    else if (!front_wall && !left_wall && !right_wall)
    {
        return DIRECTION_RIGHT_FORWARD;
    }
    else if (!left_wall && front_wall && right_wall)
    {
        return DIRECTION_LEFT_FORWARD;
    }
    else if (front_wall && left_wall && right_wall)
    {
        return DIRECTION_LEFT;
    }
    else if (!left_wall && !front_wall && right_wall)
    {
        return DIRECTION_FORWARD;
    }
    else if (!right_wall && left_wall && front_wall)
    {
        return DIRECTION_RIGHT_FORWARD;
    }

}
