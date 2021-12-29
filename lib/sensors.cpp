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

void init_adc()
{
    adc->adc0->setAveraging(0);
    adc->adc0->setResolution(10);
    adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::MED_SPEED);
    adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);

    adc->adc1->setAveraging(0);
    adc->adc1->setResolution(10);
    adc->adc1->setSamplingSpeed(ADC_SAMPLING_SPEED::MED_SPEED);
    adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);
}

void read_proximity_sensors()
{
    read_proximity_sensors_front();
    read_proximity_sensors_diagonal();
    read_proximity_sensors_side();
}

void read_proximity_sensors_front()
{
    emitter_off = adc->analogSyncRead(SENSOR_FRONT_LEFT, SENSOR_FRONT_RIGHT);

    digitalWriteFast(FRONT_DIODES, HIGH);
    delayMicroseconds(SENSOR_EMITTER_ON_TIME);

    emitter_on = adc->analogSyncRead(SENSOR_FRONT_LEFT, SENSOR_FRONT_RIGHT);

    digitalWriteFast(FRONT_DIODES, LOW);

    front_left_sensor = (uint16_t)emitter_on->result_adc0 - (uint16_t)emitter_off->result_adc0;
    front_right_sensor = (uint16_t)emitter_on->result_adc1 - (uint16_t)emitter_off->result_adc1;
}

void read_proximity_sensors_diagonal()
{
    emitter_off = adc->analogSyncRead(SENSOR_DIAGONALS_LEFT, SENSOR_DIAGONALS_RIGHT);

    digitalWriteFast(DIAGONAL_DIODES, HIGH);
    delayMicroseconds(SENSOR_EMITTER_ON_TIME);

    emitter_on = adc->analogSyncRead(SENSOR_DIAGONALS_LEFT, SENSOR_DIAGONALS_RIGHT);
    
    digitalWriteFast(DIAGONAL_DIODES, LOW);

    diagonal_left_sensor = (uint16_t)emitter_on->result_adc0 - (uint16_t)emitter_off->result_adc0;
    diagonal_right_sensor = (uint16_t)emitter_on->result_adc1 - (uint16_t)emitter_off->result_adc1;
}

void read_proximity_sensors_side()
{
    emitter_off = adc->analogSyncRead(SENSOR_SIDES_LEFT, SENSOR_SIDES_RIGHT);

    digitalWriteFast(SIDE_DIODES, HIGH);
    delayMicroseconds(SENSOR_EMITTER_ON_TIME);

    emitter_on = adc->analogSyncRead(SENSOR_SIDES_LEFT, SENSOR_SIDES_RIGHT);
    
    digitalWriteFast(SIDE_DIODES, LOW);

    side_left_sensor = (uint16_t)emitter_on->result_adc0 - (uint16_t)emitter_off->result_adc0;
    side_right_sensor = (uint16_t)emitter_on->result_adc1 - (uint16_t)emitter_off->result_adc1;
}

////////////////////////////////////////////////////////////
// Battery Voltage
////////////////////////////////////////////////////////////

/* A voltage divider is attached to this pin, that measures the battery voltage.
 * R1 = 18k 1%, R2 = 13k 1%
 * By the voltage divider formula, when the input voltage is 7.869V, 
 * the voltage provided to the BATTERY_VOLTAGE_LEVEL pin will be 3.3V.
 */
float read_battery_voltage()
{
    // Is it integer?   TODO: Check!
    int adc_value = adc->adc0->analogRead(BATTERY_VOLTAGE_LEVEL);
    float pin_voltage = ( (float)adc_value * 3.3 ) / adc->adc0->getMaxValue();

    return (pin_voltage * 31) / 13;
}

////////////////////////////////////////////////////////////
// Buttons
////////////////////////////////////////////////////////////

// TODO: Button functions

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
    lsm6ds33.begin_I2C();

    // LSM6DS_ACCEL_RANGE_4_G
    lsm6ds33.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);

    // LSM6DS_GYRO_RANGE_125_DPS
    lsm6ds33.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS);

    // LSM6DS_RATE_833_HZ
    lsm6ds33.setAccelDataRate(LSM6DS_RATE_1_66K_HZ);

    // LSM6DS_RATE_833_HZ
    lsm6ds33.setGyroDataRate(LSM6DS_RATE_1_66K_HZ);
}

void read_gyroscope()
{
    lsm6ds33.readGyroscope(gyro_x, gyro_y, gyro_z);
}

void read_accelerometer()
{
    lsm6ds33.readAcceleration(acc_x, acc_y, acc_z);
}


////////////////////////////////////////////////////////////
// Encoders
////////////////////////////////////////////////////////////

void clear_encoders()
{
    left_encoder.write(0);
    right_encoder.write(0);
}

relative_position_t get_encoder_relative_position()
{
    relative_position_t relative_position;
    left_encoder_count = left_encoder.read();
    right_encoder_count = left_encoder.read();

    relative_position.distance = 
        (right_encoder_count + left_encoder_count) * MM_PER_COUNT;
    relative_position.angle = 
        (right_encoder_count - left_encoder_count) * DEG_PER_COUNT;

    return relative_position;
}
