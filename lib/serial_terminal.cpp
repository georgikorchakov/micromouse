#include "../include/serial_terminal.h"

////////////////////////////////////////////////////////////
// Enable Command
////////////////////////////////////////////////////////////

int enable(char** args)
{
    printf("Enable command\n");
    return 1;
}


////////////////////////////////////////////////////////////
// Set Command
////////////////////////////////////////////////////////////

int set(char** args)
{
    printf("Set command\n");
    return 1;
}


////////////////////////////////////////////////////////////
// Inspect Command
////////////////////////////////////////////////////////////

int inspect(char** args)
{
    if (strcmp(args[0], "man") == 0)
    {
        inspect_man();
        return 1;
    }

    return 1;
}

void inspect_man()
{
    // time | cell | direction | speed profile | rotational_profile | speed_error | rotational_error | speed_output_v | rotational_output_v | battery_voltage | encoder_value | gyroscope_value
    char* inspect_manual = 
        "INSPECT\t\tMicromouse General Comands Manual\t\tINSPECT\n\n"
        "NAME\n\tinspect - This command is used to inspect the behaviour of the robot.\n\n"
        "SYNOPSIS\n\tinspect [system]\n\n"
        "DESCRIPTION"
        "\n\tThis is advanced inspection program, that uses statistics"
        "\n\tcollected from the systems of the robot performing in the maze."
        "\n\n\tAvailable systems for inspection:"
	    "\n\n\tcontroller"
        "\n\t\tShows the state of the variables in the controller for different timestamps."
        "\n\nAUTHOR"
        "\n\tGeorgi D. Korchakov"
        "\n\n\t\t\t\tDec. 28, 2021\n\0";

    printf("%s", inspect_manual);
}


////////////////////////////////////////////////////////////
// Show Command
////////////////////////////////////////////////////////////

int num_builtins_show()
{
    return sizeof(show_functions_str) / sizeof(char*);
}

int show_maze(char** args)
{
    if (args[2] == NULL)
    {
        print_maze(OPEN_MAZE);
    }
    else if (strcmp(args[3], "open") == 0)
    {
        print_maze(OPEN_MAZE);
    }
    else if (strcmp(args[3], "closed") == 0)
    {
        print_maze(CLOSED_MAZE);
    }
    else if (strcmp(args[3], "open_advanced") == 0)
    {
        print_maze_advanced(OPEN_MAZE);
    }
    else if (strcmp(args[3], "closed_advanced") == 0)
    {
        print_maze_advanced(CLOSED_MAZE);
    }
    else
    {
        printf("Paramethers after \"show maze\" are not recognized.\nTry \"man show\" for more information.\n");
    }

    return 1;
}

int show_best_path(char** args)
{
    return 1;
}

int show_optimization(char** args)
{
    return 1;
}

int show_speed_profile(char** args)
{
    return 1;
}

int show_rotational_profile(char** args)
{
    return 1;
}

int show_battery_voltage(char** args)
{
    return 1;
}

int show_proximity(char** args)
{
    return 1;
}

int show_gyroscope(char** args)
{
    return 1;
}

int show_accelerometer(char** args)
{
    return 1;
}

int show_encoders(char** args)
{
    return 1;
}

int show_stop(char** args)
{
    return 1;
}

int show(char** args)
{
    if (strcmp(args[0], "man") == 0)
    {
        show_man();
        return 1;
    }

    if (args[1] != NULL)
    {
        for (int i = 0; i < num_builtins_show(); ++i)
        {
            if (strcmp(args[1], show_functions_str[i]) == 0)
            {
                return (*show_functions[i])(args);
            }
        }
    }

    return 1;
}

void show_man()
{
    char* show_manual = 
        "SHOW\t\tMicromouse General Comands Manual\t\tSHOW\n\n"
        "NAME\n\tshow - This command can be used to show almost everything in the robot.\n\n"
        "SYNOPSIS\n\tshow [thing to be shown] -t [type] -o [optimization] -s [sensor] -a [axis] -e [encoder]\n\n"
        "DESCRIPTION"
        "\n\tThis command can be used to show:"
        "\n\n\tmaze"
        "\n\t\tShows the current open maze stored in the memory."
        "\n\t\tThe type of the maze shown can be selected by -t [open|closed|open_advanced|closed_advanced]."
        "\n\n\tbest path"
	    "\n\t\tShows the best path calculated with the current optimization."
        "\n\t\tDifferent optimization can be selected with -o [less turns|diagonals|shortest]."
        "\n\n\toptimization"
	    "\n\t\tShows the currently selected optimization."
	    "\n\n\tspeed profile"
	    "\n\t\tShows the current paramethers of the speed profile."
	    "\n\n\trotational profile"
	    "\n\t\tShows the current paramethers of the rotational profile."
	    "\n\n\tbattery voltage"
    	"\n\t\tShows the current battery voltage."
	    "\n\n\tproximity"
	    "\n\t\tMakes a reading from the proximity sensors and shows the result."
	    "\n\t\tSingle sensor can be selected by using"
	    "\n\t\t-s [side left|side right|front left|front right|diagonals left|diagonals right]."
        "\n\t\tContinuous readings can be achieved by useing -t [continuous]."
        "\n\n\tgyroscope"
        "\n\t\tMakes a reading from the gyroscope and shows the result."
        "\n\t\tSingle axis can be selected by using -a [x|y|z]."
        "\n\t\tContinuous readings can be achieved by useing -t [continuous]."
	    "\n\n\taccelerometer"
	    "\n\t\tMakes a reading from the accelerometer and shows the result."
        "\n\t\tSingle axis can be selected by using -a [x|y|z]."
        "\n\t\tContinuous readings can be achieved by useing -t [continuous]."
	    "\n\n\tencoders"
        "\n\t\tSingle encoder can be selected by using -e [left|right]."
	    "\n\t\tShows encoder count value."
        "\n\t\tContinuous readings can be achieved by useing -t [continuous]."
	    "\n\n\tstop"
        "\n\t\tStops the continuous reading."
        "\n\nAUTHOR"
        "\n\tGeorgi D. Korchakov"
        "\n\n\t\t\t\tDec. 28, 2021\n\0";

    printf("%s", show_manual);
}


////////////////////////////////////////////////////////////
// Help Command
////////////////////////////////////////////////////////////

int help(char** args)
{
    if (strcmp(args[0], "man") == 0)
    {
        printf("Are you really so stupid, that you need\na manual for this command!\n");
        return 1;
    }

    if (args[1] == NULL)
    {
        printf("Welcome to Micromouse shell terminal!\n");
        printf("List of available commands:\n");
        for (int i = 0; i < num_builtins(); ++i)
        {
            printf("    %s\n", builtin_str[i]);
        }
        printf("Use: \"man [command name]\"\n");
    }

    return 1;
}


////////////////////////////////////////////////////////////
// Man Command
////////////////////////////////////////////////////////////

int man(char** args)
{
    if (args[1] == NULL)
    {
        printf("man: expected argument\n");
        printf("Try \"man [command]\"\n");
        printf("Or try \"help\"\n");
        return 1;
    }
    
    for (int i = 0; i < num_builtins(); ++i)
    {
        if (strcmp(args[1], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
    }

    return 1;
}


////////////////////////////////////////////////////////////
// Code for creating the Shell interface
////////////////////////////////////////////////////////////

int num_builtins()
{
    return sizeof(builtin_str) / sizeof(char*);
}

int execute(char** args)
{
    if (args[0] == NULL)
    {
        return 1;
    }

    for (int i = 0; i < num_builtins(); ++i)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
    }

    printf("The command is not found!\n");
    printf("Try: \"help\"\n");
    return 0;
}

char* read_line()
{
    int bufsize = SERIAL_TERMINAL_BUFSIZE;
    int position = 0;
    char* buffer = (char*)malloc(sizeof(char) * bufsize);
    int c = 0;

    if (!buffer)
    {
        // An error occured
        printf("Buffer alocation failed!\n");
        return NULL;
    }

    while (1)
    {
        c = getchar();

        if (c == EOF || c == '\n')
        {
            buffer[position] = '\0';
            return buffer;
        }
        else if (c == -1)
        {
            return buffer;
        }
        else
        {
            buffer[position] = c;
        }
        ++position;

        // If we have exceededthe buffer, reallocate
        if (position >= bufsize)
        {
            bufsize += SERIAL_TERMINAL_BUFSIZE;
            buffer = (char*)realloc(buffer, bufsize);

            if (!buffer)
            {
                // An error occured
                printf("Buffer alocation failed!\n");
                return NULL;
            }
        }
    }
}

char** split_line(char* line)
{
    int bufsize = SERIAL_TERMINAL_TOKEN_BUFSIZE;
    int position = 0;
    char** tokens = (char**)malloc(bufsize * sizeof(char*));
    char* token;

    if (!tokens)
    {
        printf("Buffer alocation failed!\n");
        return NULL;
    }

    token = strtok(line, SERIAL_TERMINAL_TOK_DELIM);
    while (token != NULL)
    {
        tokens[position] = token;
        ++position;

        if (position >= bufsize)
        {
            bufsize += SERIAL_TERMINAL_TOKEN_BUFSIZE;
            tokens = (char**)realloc(tokens, bufsize * sizeof(char*));

            if (!tokens)
            {
                printf("Buffer alocation failed!\n");
                return NULL;
            }
        }

        token = strtok(NULL, SERIAL_TERMINAL_TOK_DELIM);
    }

    tokens[position] = NULL;
    return tokens;
}

void serial_terminal()
{
    char* line;
    char** args;

    printf("[Micromouse] > ");
    line = read_line();
    args = split_line(line);
    execute(args);

    free(line);
    free(args);
}

int main()
{
    init_maze();
    while (1)
    {
        serial_terminal();
    }

    return 0;
}
