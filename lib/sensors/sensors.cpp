#include "sensors.h"

////////////////////////////////////////////////////////////
// Proximity Sensors
////////////////////////////////////////////////////////////

/*
 * FRONT_DIODES, SIDE_DIODES and DIAGONAL_DIODES 
 * control mosfets that provide power to the emiters of the proximity
 * sensors.  Before the value of the sensor is measured, a digital HIGH should
 * be provided to the coresponding emiter.
 * 
 * WARNING: NEVER LIGHT UP ALL OF THE EMITERS SIMULTANIEUSLY, BECAUSE THE
 * VOLTAGE REGULATOR MAY FAIL TO SUPPLY THE REQUIRED CURRENT AND THE
 * MICROCONTROLLER MAY REBOOT
 * 
 * WARNING: NEVER POWER THE EMITERS FOR MORE THAN A COUPLE OF MICROSECONDS,
 * BACAUSE DAMAGE OF THE SENSORS MAY OCCUR
 * 
 * ---------------------------------------------------------------------------
 * 
 * When we are reading the sensor values,
 * the surounding light could affect the
 * values read from the phototransistor.
 * In order to eliminate that effect 
 * as mutch as possible, we are making 
 * two reading. One before lighting the emitter
 * and one after that. Then we substract the dark value
 * from the light value.
 * 
 * sensor_value = sensor_emitter_on - sensor_emitter_off;
 * 
 */

ADC *adc = NULL;

ADC::Sync_result emitter_on;
ADC::Sync_result emitter_off;

Encoder *left_encoder = NULL;
Encoder *right_encoder = NULL;

long previous_encoder_count_left = 0;
long previous_encoder_count_right = 0;

//Adafruit_LSM6DS33 lsm6ds33;
LSM6 imu;

float acc_x;
float acc_y;
float acc_z;

float gyro_x;
float gyro_y;
float gyro_z;


void init_adc()
{  
    pinMode(SENSOR_FRONT_LEFT, INPUT);
    pinMode(SENSOR_FRONT_RIGHT, INPUT);
    pinMode(SENSOR_SIDES_LEFT, INPUT);
    pinMode(SENSOR_SIDES_RIGHT, INPUT);
    pinMode(SENSOR_DIAGONALS_LEFT, INPUT);
    pinMode(SENSOR_DIAGONALS_RIGHT, INPUT);
    pinMode(ADDITIONAL_BUTTONS, INPUT);
    pinMode(BATTERY_VOLTAGE_LEVEL, INPUT);

    adc = new ADC();

    if (adc == NULL)
    {
        Serial.println("adc = new ADC() failed!");
        while(1);
    }

    adc->adc0->setAveraging(0);
    adc->adc0->setResolution(10);
    adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::MED_SPEED);
    adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);

    adc->adc1->setAveraging(0);
    adc->adc1->setResolution(10);
    adc->adc1->setSamplingSpeed(ADC_SAMPLING_SPEED::MED_SPEED);
    adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);
}

void init_proximity_sensors()
{
    pinMode(FRONT_DIODES, OUTPUT);
    pinMode(SIDE_DIODES, OUTPUT);
    pinMode(DIAGONAL_DIODES, OUTPUT);
    pinMode(BUZZER, OUTPUT);

    digitalWrite(FRONT_DIODES, LOW);
    digitalWrite(SIDE_DIODES, LOW);
    digitalWrite(DIAGONAL_DIODES, LOW);
    digitalWrite(BUZZER, LOW);
}

proximity_sensors_t* init_proximity_sensors_struct()
{
    proximity_sensors_t *__proximity_sensors;
    if ( ( __proximity_sensors = (proximity_sensors_t*)calloc(1, sizeof(proximity_sensors_t)) ) == NULL )
    {
        Serial.println("Malloc Fail!");
        Serial.flush();
        return NULL;
    }

    return __proximity_sensors;
}

void read_proximity_sensors(proximity_sensors_t **__proximity_sensors)
{
    read_proximity_sensors_front(__proximity_sensors);
    delayMicroseconds(80);
    read_proximity_sensors_diagonal(__proximity_sensors);
    delayMicroseconds(80);
    read_proximity_sensors_side(__proximity_sensors);
}

void read_proximity_sensors_front(proximity_sensors_t **__proximity_sensors)
{
    emitter_off = adc->analogSyncRead(SENSOR_FRONT_LEFT, SENSOR_FRONT_RIGHT);

    digitalWriteFast(FRONT_DIODES, HIGH);
    delayMicroseconds(SENSOR_EMITTER_ON_TIME);

    emitter_on = adc->analogSyncRead(SENSOR_FRONT_LEFT, SENSOR_FRONT_RIGHT);

    digitalWriteFast(FRONT_DIODES, LOW);

    (*__proximity_sensors)->front_left_sensor = (uint16_t)emitter_on.result_adc0 - (uint16_t)emitter_off.result_adc0;
    (*__proximity_sensors)->front_right_sensor = (uint16_t)emitter_on.result_adc1 - (uint16_t)emitter_off.result_adc1;
}

void read_proximity_sensors_diagonal(proximity_sensors_t **__proximity_sensors)
{
    emitter_off = adc->analogSyncRead(SENSOR_DIAGONALS_LEFT, SENSOR_DIAGONALS_RIGHT);

    digitalWriteFast(DIAGONAL_DIODES, HIGH);
    delayMicroseconds(SENSOR_EMITTER_ON_TIME);

    emitter_on = adc->analogSyncRead(SENSOR_DIAGONALS_LEFT, SENSOR_DIAGONALS_RIGHT);
    
    digitalWriteFast(DIAGONAL_DIODES, LOW);

    (*__proximity_sensors)->diagonal_left_sensor = (uint16_t)emitter_on.result_adc0 - (uint16_t)emitter_off.result_adc0;
    (*__proximity_sensors)->diagonal_right_sensor = (uint16_t)emitter_on.result_adc1 - (uint16_t)emitter_off.result_adc1;
}

void read_proximity_sensors_side(proximity_sensors_t **__proximity_sensors)
{
    emitter_off = adc->analogSyncRead(SENSOR_SIDES_LEFT, SENSOR_SIDES_RIGHT);

    digitalWriteFast(SIDE_DIODES, HIGH);
    delayMicroseconds(SENSOR_EMITTER_ON_TIME);

    emitter_on = adc->analogSyncRead(SENSOR_SIDES_LEFT, SENSOR_SIDES_RIGHT);
    
    digitalWriteFast(SIDE_DIODES, LOW);

    (*__proximity_sensors)->side_left_sensor = (uint16_t)emitter_on.result_adc0 - (uint16_t)emitter_off.result_adc0;
    (*__proximity_sensors)->side_right_sensor = (uint16_t)emitter_on.result_adc1 - (uint16_t)emitter_off.result_adc1;
}

uint8_t get_left_wall(proximity_sensors_t **__proximity_sensors)
{
    return (*__proximity_sensors)->side_left_sensor >= LEFT_WALL_THRESHOLD;
}

uint8_t get_right_wall(proximity_sensors_t **__proximity_sensors)
{
    return (*__proximity_sensors)->side_right_sensor >= RIGHT_WALL_THRESHOLD;
}

uint8_t get_front_wall(proximity_sensors_t **__proximity_sensors)
{
    return ( (*__proximity_sensors)->front_left_sensor + 
             (*__proximity_sensors)->front_right_sensor ) / 2 >= FRONT_WALL_THRESHOLD;
}

////////////////////////////////////////////////////////////
// Battery Voltage
////////////////////////////////////////////////////////////

float adc_reading_to_voltage(int adc_value)
{
    return ( (float)adc_value * 3.3 ) / adc->adc0->getMaxValue();
}

/* A voltage divider is attached to this pin, that measures the battery voltage.
 * R1 = 18k 1%, R2 = 13k 1%
 * By the voltage divider formula, when the input voltage is 7.869V, 
 * the voltage provided to the BATTERY_VOLTAGE_LEVEL pin will be 3.3V.
 */
float read_battery_voltage()
{
    int adc_value = adc->adc0->analogRead(BATTERY_VOLTAGE_LEVEL);
    return (adc_reading_to_voltage(adc_value) * 31) / 13;
}

////////////////////////////////////////////////////////////
// Buttons
////////////////////////////////////////////////////////////
void init_buttons()
{
    pinMode(BUTTON1, INPUT);
    pinMode(BUTTON2, INPUT);
}

struct push_buttons_t read_push_buttons()
{
    struct push_buttons_t buttons;
    buttons.up = digitalReadFast(BUTTON1);
    buttons.down = digitalReadFast(BUTTON2);

    int additional_buttons = adc->adc0->analogRead(ADDITIONAL_BUTTONS);
    float additional_buttons_voltage = adc_reading_to_voltage(additional_buttons);

    buttons.left = 0;
    buttons.center = 0;
    buttons.right = 0;

    if (additional_buttons_voltage < 1.395 && additional_buttons_voltage > 1.185)
    {
        buttons.right = 1;
    }
    else if (additional_buttons_voltage <= 1.185 && additional_buttons_voltage > 1.01)
    {
        buttons.center = 1;
    }
    else if (additional_buttons_voltage <= 1.01 && additional_buttons_voltage > 0.88)
    {
        buttons.left = 1;
    }

    return buttons;
}   


////////////////////////////////////////////////////////////
// Accelerometer and Gyroscope
////////////////////////////////////////////////////////////

// We have two approches to the Gyroscope, because
// they are noisy little creacures!
// First one, if the running speed does affect accuracy,
// then we will set it to 1.66KHz and we will be fine.
// The second option is to use higher sampling speed (6.66KHz)
// and in the range of one ms to make 4 averages of the signal.

void init_gyro_accel()
{
    Wire.begin();
    Wire.setClock(400000);

    if (!imu.init())
    {
        Serial.println("Failed to detect and initialize IMU!");
        while (1);
    }
    imu.enableDefault();

    // lsm6ds33.begin_I2C();

    // // LSM6DS_ACCEL_RANGE_4_G
    // lsm6ds33.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);

    // // LSM6DS_GYRO_RANGE_125_DPS
    // lsm6ds33.setGyroRange(LSM6DS_GYRO_RANGE_125_DPS);

    // // LSM6DS_RATE_833_HZ
    // lsm6ds33.setAccelDataRate(LSM6DS_RATE_1_66K_HZ);

    // // LSM6DS_RATE_833_HZ
    // lsm6ds33.setGyroDataRate(LSM6DS_RATE_1_66K_HZ);
}

void read_gyroscope()
{
    imu.readGyro();

    //imu.g.z*8.75*0.017453293/1000

    // An ugly formula stolen from Adafruit LSM6DS Library -/+ Zero Level Offset
    gyro_x = (( imu.g.x * 8.75 * 0.017453293 ) / 1000 - 0.0391) * 57.2958;
    gyro_y = (( imu.g.y * 8.75 * 0.017453293 ) / 1000 + 0.1478) * 57.2958;
    gyro_z = (( imu.g.z * 8.75 * 0.017453293 ) / 1000 + 0.0586) * 57.2958;

    //lsm6ds33.readGyroscope(gyro_x, gyro_y, gyro_z);
}

// void read_accelerometer()
// {
//     lsm6ds33.readAcceleration(acc_x, acc_y, acc_z);
// }


////////////////////////////////////////////////////////////
// Encoders
////////////////////////////////////////////////////////////

void init_encoders()
{
    left_encoder = new Encoder(LEFT_MOTOR_ENCODER_A, LEFT_MOTOR_ENCODER_B);
    right_encoder = new Encoder(RIGHT_MOTOR_ENCODER_A, RIGHT_MOTOR_ENCODER_B);
}

void clear_encoders()
{
    left_encoder->write(0);
    right_encoder->write(0);
}

relative_position_t get_ecnoder_position_change_since_last_read()
{
    relative_position_t relative_position;

    long left_encoder_count = left_encoder->read();
    long right_encoder_count = right_encoder->read();

    long left_encoder_change = left_encoder_count - previous_encoder_count_left;
    long right_encoder_change = right_encoder_count - previous_encoder_count_right;

    previous_encoder_count_left = left_encoder_count;
    previous_encoder_count_right = right_encoder_count;

    relative_position.distance = 
        (float)(right_encoder_change + left_encoder_change) * MM_PER_COUNT;
    relative_position.angle = 
        (float)(right_encoder_change - left_encoder_change) * DEG_PER_COUNT;

    return relative_position;
}

relative_position_t get_encoder_relative_position()
{
    relative_position_t relative_position;
    long left_encoder_count = left_encoder->read();
    long right_encoder_count = right_encoder->read();

    relative_position.distance = 
        (float)(right_encoder_count + left_encoder_count) * MM_PER_COUNT;
    relative_position.angle = 
        (float)(right_encoder_count - left_encoder_count) * DEG_PER_COUNT;

    return relative_position;
}
