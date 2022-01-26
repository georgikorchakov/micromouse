#ifndef SIMPLE_LOGGER_H
#define SIMPLE_LOGGER_H

typedef struct {
    uint64_t front_left_sensor      : 10;
    uint64_t front_right_sensor     : 10;
    uint64_t side_left_sensor       : 10;
    uint64_t side_right_sensor      : 10;
    uint64_t diagonal_left_sensor   : 10;
    uint64_t diagonal_right_sensor  : 10;  
    uint64_t cell_y                 : 4 ; // 8 bytes slot
    float gyroscope_x;                    // 4 bytes
    int8_t left_encoder_change;           // 1 byte
    int8_t right_encoder_change;          // 1 byte
    uint8_t cell_x                  : 4 ; // 4 bits
    uint8_t direction               : 2 ; // 2 bits
} cell_timestamp_t;


#endif // SIMPLE_LOGGER_H