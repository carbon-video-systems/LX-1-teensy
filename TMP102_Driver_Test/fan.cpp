
/*
 * Fan Source
 *
 * @file    fan.cpp
 * @author  Carbon Video Systems 2019
 * @description   Cooling Fan Driver.
 *
 * @section LICENSE
 * Redistribution and use in source and binary forms, with or without
 * modification, is permitted in accordance with the BSD 3-Clause License.
 *
 * Distributed as-is; in accordance with the BSD 3-Clause License.
 */

/* Includes-------------------------------------------------------------*/
#include "fan.h"

/* Constants -----------------------------------------------------------*/
#define TEMP_SENSOR_1_ADDRESS     0x48
#define TEMP_SENSOR_2_ADDRESS     0x49

#define FAN1_PIN            29
#define FAN2_PIN            30

#define MIN_PWM_THRESHOLD   14
#define MAX_PWM_THRESHOLD   1024
#define PWM_FREQ            29296.875
#define ANALOG_RESOLUTION   10

#define MIN_FAN_TEMP        25
#define MAX_FAN_TEMP        40
#define pwmNum(temperature)  int(map(temperature, MIN_FAN_TEMP, MAX_FAN_TEMP, MIN_PWM_THRESHOLD, MAX_PWM_THRESHOLD))

/* Variables  ----------------------------------------------------------*/
TMP102 temp_sensor_1(TEMP_SENSOR_1_ADDRESS);
TMP102 temp_sensor_2(TEMP_SENSOR_2_ADDRESS);

/* Functions------------------------------------------------------------*/
/**
  * @brief  Initializes cooling fan GPIO control and the
  *     TMP102 temperature sensor.
  * @param  void
  * @return void
  */
void initFans(void)
{
    #ifdef TESTING
        SerialUSB.println("Initializing temperature sensors");
    #endif

    temp_sensor_1.begin();

    pinMode(FAN1_PIN, OUTPUT);
    pinMode(FAN2_PIN, OUTPUT);
    analogWriteResolution(ANALOG_RESOLUTION);
    analogWriteFrequency(FAN1_PIN, PWM_FREQ);
    analogWriteFrequency(FAN2_PIN, PWM_FREQ);

    analogWrite(FAN1_PIN, 0);
    analogWrite(FAN2_PIN, 0);
}

/**
  * @brief  Reads temperature sensor #1 and runs fan #1 accordingly.
  * @param  void
  * @return void
  */
void runFan1(void)
{
    static bool fanStartup1 = true;
    float temp_sensor_data_1 = constrain(temp_sensor_1.readTempC(), MIN_FAN_TEMP, MAX_FAN_TEMP);

    if (temp_sensor_data_1 <= MIN_FAN_TEMP){
            analogWrite(FAN1_PIN, 0);
            fanStartup1 = true;

            #ifdef TESTING
                SerialUSB.println("Fan 1 off");
            #endif
        }
        else{
            if (fanStartup1)
                analogWrite(FAN1_PIN, MAX_PWM_THRESHOLD);
            else
                analogWrite(FAN1_PIN, pwmNum(temp_sensor_data_1));

            #ifdef TESTING
                SerialUSB.print("FAN 1 ON - constrained temperature 1:  ");
                SerialUSB.println(temp_sensor_data_1);
            #endif

            fanStartup1 = false;
        }
}

/**
  * @brief  Reads temperature sensor #2 and runs fan #2 accordingly.
  * @param  void
  * @return void
  */
void runFan2(void)
{
    static bool fanStartup2 = true;
    float temp_sensor_data_2 = constrain(temp_sensor_2.readTempC(), MIN_FAN_TEMP, MAX_FAN_TEMP);

    if (temp_sensor_data_2 <= MIN_FAN_TEMP){
        analogWrite(FAN2_PIN, 0);
        fanStartup2 = true;

        #ifdef TESTING
            SerialUSB.println("Fan 2 off");
        #endif
    }
    else{
        if(fanStartup2)
            analogWrite(FAN2_PIN, MAX_PWM_THRESHOLD);
        else
            analogWrite(FAN2_PIN, pwmNum(temp_sensor_data_2));

        #ifdef TESTING
            SerialUSB.print("FAN 2 ON - constrained temperature 2: ");
            SerialUSB.println(temp_sensor_data_2);
        #endif

        fanStartup2 = false;
    }
}
