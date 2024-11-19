/*
 * gps.c
 *
 *  Created on: April 11, 2024
 *      Author: 123ke
 */
#include "bmp_sensor.h"

uint8_t sensor_Data[256];
void Bmp_Init(I2C_HandleTypeDef *hi2c1, BMP280_HandleTypedef *bmp280)
{
    bmp280_init_default_params(&bmp280->params);
    bmp280->addr = BMP280_I2C_ADDRESS_0;
    bmp280->i2c = hi2c1;
    if (!bmp280_init(bmp280, &bmp280->params))
    {
        cdc_Transmit("BMP280 initialization failed! \r\n");
    }
    else
    {
        cdc_Transmit("BMP280 initialization successful! \r\n");
    }
}

void Bmp_Read_Data(BMP280_HandleTypedef *bmp280, float *pressure, float *temperature, float *humidity)
{
    if (!bmp280_read_float(bmp280, temperature, pressure, humidity))
    {
        sprintf((char *)sensor_Data, "Temperature and pressure reading failed!\r\n");
    }
    else
    {
        float altitude = 0;
        altitude = altitude_calc(pressure, temperature);
        sprintf((char *)sensor_Data, "Pressure: %.2f Pa, Temperature: %.2f C, Altitude: %.f ft\r\n", *pressure, *temperature, altitude);

        // format for SD card
        uint8_t sd_data[256];
        sprintf((char *)sd_data, "%.2f, %.2f, %.f\r\n", *pressure, *temperature, altitude);
        sd_write(sd_data);
    }
    cdc_Transmit(sensor_Data);
}

float altitude_calc(float *pressure, float *temperature)
{
    float altitude;
    altitude = 145366.45 * (1 - pow(*pressure / 101325, 1 / 5.255));
    return altitude;
}
