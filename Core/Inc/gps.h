/*
 * gps.h
 *
 *  Created on: Feb 20, 2024
 *      Author: 123ke
 */

#ifndef INC_GPS_H_
#define INC_GPS_H_

#include "stm32f4xx_hal.h"
#include "string.h"
#include "usb_device.h"
#include "usbd_cdc.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define Min_To_Degree 0.01666666667
#define Sec_To_Degree 0.000277777778

typedef struct Neo6M_GpsData
{
    double time;
    double latitude;
    double longitude;
    double altitude_approx;
    double altitude_correction;
    char N_OR_S;
    char E_OR_W;
    uint8_t data[700];
    uint8_t result[100];

    UART_HandleTypeDef UartPort;
} Neo6M_GpsData;
extern Neo6M_GpsData NEO_GPS;
extern uint8_t radioData[700];

void CDC_Transmit_FS(uint8_t *data, uint16_t length);
void Gps_Data_Parse(UART_HandleTypeDef *huart, uint32_t lastTransmitTime);
void Gps_Data_Conversion();

#endif /* INC_GPS_H_ */
