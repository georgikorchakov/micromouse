// Straight line
// Pivot turn
// Curve turn
// Diagonals

#include "movements.h"


void one_cell_forward()
{
    //enable_steering();
    profile_start(&speed_profile, 180, 700, 0, 1000); //500 /// 1200
    //profile_start(&speed_profile, 180, 1000, 0, 1600); //500
    profile_start(&angle_profile, 0, 200, 0, 500);

    enable_systick_function();
    while (!profile_is_finished(&speed_profile) || !profile_is_finished(&angle_profile))
    {
        delay(1);
    }
    disable_systick_function();
    //disable_steering();
}

void forward_cells(int number_of_cells)
{
    //enable_steering();
    profile_start(&speed_profile, 180*number_of_cells, 700, 0, 1000);
    // profile_start(&speed_profile, 180*number_of_cells, 1000, 0, 1600); //500
    profile_start(&angle_profile, 0, 200, 0, 500);

    enable_systick_function();
    while (!profile_is_finished(&speed_profile) || !profile_is_finished(&angle_profile))
    {
        delay(1);
    }
    disable_systick_function();
    //disable_steering();
}

void left_pivot_turn()
{
    profile_start(&speed_profile, 0, 500, 0, 1000);
    profile_start(&angle_profile, 90, 200, 0, 500);

    enable_systick_function();
    while (!profile_is_finished(&speed_profile) || !profile_is_finished(&angle_profile))
    {
        delay(1);
    }
    disable_systick_function();
}

void right_pivot_turn()
{
    profile_start(&speed_profile, 0, 500, 0, 1000);
    profile_start(&angle_profile, -91, 200, 0, 500);

    enable_systick_function();
    while (!profile_is_finished(&speed_profile) || !profile_is_finished(&angle_profile))
    {
        delay(1);
    }
    disable_systick_function();
}


// Back to robot rotational center - (38.62mm)
// Distance to the center of the cell - (90 - 38.62 = 51.38)
void move_forward_from_the_start()
{
    profile_start(&speed_profile, 51.38, 700, 0, 1000);
    //profile_start(&speed_profile, 51.38, 1000, 0, 1600); //500
    profile_start(&angle_profile, 0, 200, 0, 500);

    enable_systick_function();
    while (!profile_is_finished(&speed_profile) || !profile_is_finished(&angle_profile))
    {
        delay(1);
    }
    disable_systick_function();
}


