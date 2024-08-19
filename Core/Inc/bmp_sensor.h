#ifndef INC_bmp_sensor_H_
#define INC_bmp_sensor_H_

#include "stm32f4xx_hal.h"
#include "string.h"
#include "usb_device.h"
#include "usbd_cdc.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp280.h"
#include "sdCard.h"

extern BMP280_HandleTypedef bmp280;

extern float pressure, temperature, humidity;

extern uint8_t sensor_Data[256];

void Bmp_Init(I2C_HandleTypeDef *hi2c1, BMP280_HandleTypedef *bmp280);

void Bmp_Read_Data(BMP280_HandleTypedef *bmp280, float *pressure, float *temperature, float *humidity);

float altitude_calc(float *pressure, float *temperature);

#endif
