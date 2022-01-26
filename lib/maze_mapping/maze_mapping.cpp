#include "maze_mapping.h"

// From the start, the maze will be seen as:
//       NORTH
//         ^
//         |
// WEST <- . -> EAST
//         |
//         V
//       SOUTH
// NORTH | EAST | SOUTH | WEST

uint8_t maze[MAZE_SIZE_Y][MAZE_SIZE_X];

void init_maze()
{
    uint8_t maze_cell;
    for (uint8_t maze_y = 0; maze_y < MAZE_SIZE_Y; ++maze_y)
    {
        for (uint8_t maze_x = 0; maze_x < MAZE_SIZE_X; ++maze_x)
        {
            maze_cell = 0xF0;

            if (maze_y == 0)
            {
                maze_cell |= (1 << NORTH);
            }
            else if (maze_y == MAZE_SIZE_Y - 1)
            {
                maze_cell |= (1 << SOUTH);
            }

            if (maze_x == 0)
            {
                maze_cell |= (1 << WEST);
            }
            else if (maze_x == MAZE_SIZE_X - 1)
            {
                maze_cell |= (1 << EAST);
            }

            maze[maze_y][maze_x] = maze_cell;
        }
    }
}

// Normaly the 0:0 coordinate of the maze is the top left corner,
// but it would be better if 0:0 coordinate was in the bottom left corner
uint8_t get_maze_cell(uint8_t x, uint8_t y)
{
    return maze[MAZE_SIZE_Y - 1 - y][x];
}

void set_maze_cell(uint8_t x, uint8_t y, uint8_t cell)
{
    maze[MAZE_SIZE_Y - 1 - y][x] = cell;
}

void update_cell_single_wall(uint8_t x, uint8_t y, uint8_t add_wall, uint8_t direction)
{
    uint8_t maze_cell = get_maze_cell(x, y);
    if (add_wall)
    {
        maze_cell |= (((1 << direction) << 4) | (1 << direction));
    }
    else
    {
        maze_cell &= ~(((1 << direction) << 4) | (1 << direction));
    }
    set_maze_cell(x, y, maze_cell);
}

uint8_t add_maze_border(uint8_t x, uint8_t y)
{
    uint8_t walls = 0;

    if (x == 0)
    {
        walls |= (1 << WEST);
    }
    else if (x == MAZE_SIZE_X - 1)
    {
        walls |= (1 << EAST);
    }

    if (y == 0)
    {
        walls |= (1 << SOUTH);
    }
    else if (y == MAZE_SIZE_Y - 1)
    {
        walls |= (1 << NORTH);
    }

    return walls;
}

void update_cell_walls(uint8_t x, uint8_t y, uint8_t walls)
{
    walls |= add_maze_border(x, y);

    // Update the current cell
    uint8_t maze_cell = get_maze_cell(x, y);
    maze_cell = (walls << 4) | walls ;
    set_maze_cell(x, y, maze_cell);

    // Update adjacent cells
    // North cell
    if (y != MAZE_SIZE_Y - 1)
    {
        update_cell_single_wall(x, y + 1, walls & (1 << NORTH), SOUTH);
    }

    // East cell
    if (x != MAZE_SIZE_X - 1)
    {
        update_cell_single_wall(x + 1, y, walls & (1 << EAST), WEST);
    }

    // South cell
    if (y != 0)
    {
        update_cell_single_wall(x, y - 1, walls & (1 << SOUTH), NORTH);
    }

    // West cell
    if (x != 0)
    {
        update_cell_single_wall(x - 1, y, walls & (1 << WEST), EAST);
    }
}

uint8_t synchronize_walls_with_direction(uint8_t left_wall, uint8_t front_wall, uint8_t right_wall, uint8_t direction)
{
    uint8_t walls = 0;
    if (direction == NORTH)
    {
        if (front_wall)
        {
            walls |= NORTH_WALL;
        }
        if (left_wall)
        {
            walls |= WEST_WALL;
        }
        if (right_wall)
        {
            walls |= EAST_WALL;
        }
    }
    else if (direction == EAST)
    {
        if (front_wall)
        {
            walls |= EAST_WALL;
        }
        if (left_wall)
        {
            walls |= NORTH_WALL;
        }
        if (right_wall)
        {
            walls |= SOUTH_WALL;
        }  
    }
    else if (direction == SOUTH)
    {
        if (front_wall)
        {
            walls |= SOUTH_WALL;
        }
        if (left_wall)
        {
            walls |= EAST_WALL;
        }
        if (right_wall)
        {
            walls |= WEST_WALL;
        }
    }
    else if (direction == WEST)
    {
        if (front_wall)
        {
            walls |= WEST_WALL;
        }
        if (left_wall)
        {
            walls |= SOUTH_WALL;
        }
        if (right_wall)
        {
            walls |= NORTH_WALL;
        }
    }

    return walls;
}

int8_t two_bit(int8_t bits)
{
    if (bits < 0)
    {
        bits = 3;
    }
    else if (bits > 3)
    {
        bits = 0;
    }

    return bits;
}

uint8_t update_direction(uint8_t direction, uint8_t turn)
{
    if (turn == LEFT)
    {
        return two_bit(direction + 1);
    }
    else if (turn == RIGHT)
    {
        return two_bit(direction - 1);
    }
}

uint8_t update_y(uint8_t y, uint8_t direction)
{
    if (direction == NORTH)
    {
        return y + 1;
    }
    else if (direction == SOUTH)
    {
        return y - 1;
    }
    else
    {
        return y;
    }
}

uint8_t update_x(uint8_t x, uint8_t direction)
{
    if (direction == EAST)
    {
        return x + 1;
    }
    else if (direction == WEST)
    {
        return x - 1;
    }
    else
    {
        return x;
    }
}

void print_maze(uint8_t maze_select)
{
    for (uint8_t maze_y = 0; maze_y < MAZE_SIZE_Y; ++maze_y)
    {
        Serial.print("+");
        for (uint8_t maze_x = 0; maze_x < MAZE_SIZE_X; ++maze_x)
        {
           if ( (maze[maze_y][maze_x] >> maze_select) & (1 << NORTH) ) 
           {
               Serial.print("---+");
           }
           else
           {
               Serial.print("   +");
           }
        }

        Serial.print("\n");

        for (uint8_t maze_x = 0; maze_x < MAZE_SIZE_X; ++maze_x)
        {
           if ( (maze[maze_y][maze_x] >> maze_select) & (1 << WEST) ) 
           {
               Serial.print("|   ");
           }
           else
           {
               Serial.print("    ");
           }

           if (maze_x == MAZE_SIZE_X - 1)
           {
               if ( (maze[maze_y][maze_x] >> maze_select) & (1 << EAST) ) 
                {
                    Serial.print("|");
                }
                else
                {
                    Serial.print(" \n");
                }
           }
        }
        Serial.print(" \n");

        if (maze_y == MAZE_SIZE_Y - 1)
        {
            Serial.print("+");
            for (uint8_t maze_x = 0; maze_x < MAZE_SIZE_X; ++maze_x)
            {
                if ( (maze[maze_y][maze_x] >> maze_select) & (1 << SOUTH) ) 
                {
                    Serial.print("---+");
                }
                else
                {
                    Serial.print("   +");
                }
            }
            Serial.print("\n");
        }
    }
}

void print_maze_advanced(uint8_t maze_select)
{
    for (uint8_t maze_y = 0; maze_y < MAZE_SIZE_Y; ++maze_y)
    {
        Serial.print("  ");
        for (uint8_t maze_x = 0; maze_x < MAZE_SIZE_X; ++maze_x)
        {
           if ( (maze[maze_y][maze_x] >> maze_select) & (1 << NORTH) ) 
           {
               Serial.print("+---+");
           }
           else
           {
               Serial.print("+   +");
           }
        }

        char num[4];
        sprintf(num, "\n%2d", 15 - maze_y);
        Serial.print(num);

        for (uint8_t maze_x = 0; maze_x < MAZE_SIZE_X; ++maze_x)
        {
           if ( (maze[maze_y][maze_x] >> maze_select) & (1 << WEST) ) 
           {
               Serial.print("|");
           }
           else
           {
               Serial.print(" ");
           }

           if ( (maze[maze_y][maze_x] >> maze_select) & (1 << EAST) ) 
           {
               Serial.print("   |");
           }
           else
           {
               Serial.print("    ");
           }
        }

        Serial.print("\n  ");

        for (uint8_t maze_x = 0; maze_x < MAZE_SIZE_X; ++maze_x)
        {
           if ( (maze[maze_y][maze_x] >> maze_select) & (1 << SOUTH) ) 
           {
               Serial.print("+---+");
           }
           else
           {
               Serial.print("+   +");
           }
        }

        Serial.print("\n");
    }

    Serial.print(" ");
    for (uint8_t maze_x = 0; maze_x < MAZE_SIZE_X; ++maze_x)
    {
        Serial.print("  ");
        char num[4];
        sprintf(num, "%3d", maze_x);
        Serial.print(num);
    }

    Serial.print("\n");
}

void debug_maze_matrix()
{
    // for (uint8_t maze_y = 0; maze_y < MAZE_SIZE_Y; ++maze_y)
    // {
    //     for (uint8_t maze_x = 0; maze_x < MAZE_SIZE_X; ++maze_x)
    //     {
    //         printf("x: %d, y: %d | North: %d East: %d South: %d West: %d \n",
    //                 maze_x, maze_y,
    //                 (get_maze_cell(maze_x, maze_y) & (1 << NORTH)) >> NORTH,
    //                 (get_maze_cell(maze_x, maze_y) & (1 << EAST)) >> EAST,
    //                 (get_maze_cell(maze_x, maze_y) & (1 << SOUTH)) >> SOUTH,
    //                 (get_maze_cell(maze_x, maze_y) & (1 << WEST)) >> WEST);
    //     }
    // }
}

/*
int main()
{
    init_maze();

    update_cell_walls(7, 7, (1 << WEST) | (1 << SOUTH));
    update_cell_walls(7, 8, (1 << NORTH) | (1 << WEST));
    update_cell_walls(8, 7, (1 << SOUTH) | (1 << EAST));
    update_cell_walls(8, 8, (1 << NORTH) | (1 << EAST));

    update_cell_walls(0, 0, (1 << WEST) | (1 << EAST));
    update_cell_walls(0, 1, (1 << WEST) | (1 << EAST));
    update_cell_walls(0, 2, (1 << WEST) | (1 << EAST));

    //debug_maze_matrix();

    //print_maze_advanced(CLOSED_MAZE);
    print_maze(CLOSED_MAZE);

    return 0;
}
*/
