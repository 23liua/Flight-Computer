/*
 * gps.c
 *
 *  Created on: Feb 20, 2024
 *      Author: 123ke
 */

#include "gps.h"
#include "sdCard.h"

Neo6M_GpsData NEO_GPS;
uint8_t radioData[700];

/*void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{                                                   // called automatically after DMA interrupt
    HAL_UART_Receive_DMA(huart, NEO_GPS.data, 700); // restarts DMA
}
*/
void Gps_Data_Parse(UART_HandleTypeDef *huart, uint32_t lastTransmitTime)
{
    uint8_t GPGGA_Buffer[256];

    char *start = strstr((char *)NEO_GPS.data, "GPGGA");
    if (start != NULL)
    {
        char *newline_position = strchr(start, '\n');
        if (newline_position != NULL)
        {
            size_t length = newline_position - start + 1;

            strncpy((char *)GPGGA_Buffer, start, length - 1);
            GPGGA_Buffer[length - 1] = '\0';

            sscanf((char *)GPGGA_Buffer, "GPGGA,%lf,%lf,%c,%lf,%c,,,,%lf,,%lf,,,",
                   &NEO_GPS.time, &NEO_GPS.latitude, &NEO_GPS.N_OR_S,
                   &NEO_GPS.longitude, &NEO_GPS.E_OR_W,
                   &NEO_GPS.altitude_approx, &NEO_GPS.altitude_correction);
            Gps_Data_Conversion();

            // need to add check on whether or not the values are outrageously wrong
            if (isnan(NEO_GPS.time) || isnan(NEO_GPS.latitude) || isnan(NEO_GPS.longitude))
            {
                NEO_GPS.time = 0.0;
                NEO_GPS.latitude = 0.0;
                NEO_GPS.longitude = 0.0;
            }

            // Format data for SD card logging
            uint8_t sd_data[256];
            sprintf((char *)sd_data, "%.lf, %.5lf, %.5lf, ",
                    NEO_GPS.time,
                    NEO_GPS.latitude,
                    NEO_GPS.longitude);
            sd_write(sd_data);

            // termianl debugging
            sprintf(NEO_GPS.result, "Time: %.lf Latitude: %.5lf Longitude: %.5lf\r\n",
                    NEO_GPS.time, NEO_GPS.latitude, NEO_GPS.longitude);
            CDC_Transmit_FS(NEO_GPS.result, strlen(NEO_GPS.result));

            // Transmit data via radio
            HAL_UART_Transmit_DMA(huart, NEO_GPS.result, strlen(NEO_GPS.result));
        }
    }
    else
    {
        sd_write("0.0, 0.0, 0.0, ");
    }
}

// convert long and lat to human readable
void Gps_Data_Conversion()
{
    double Deg_Val = 0.0, Min_Val = 0.0, Sec_Val = 0.0, lon = 0.0, lat = 0.0;

    lon = NEO_GPS.longitude;
    lat = NEO_GPS.latitude;
    Deg_Val = (int)(lon / 100);
    Min_Val = (int)(lon - (Deg_Val * 100));
    Sec_Val = ((lon - (Deg_Val * 100)) - Min_Val) * 100;
    NEO_GPS.longitude = (Deg_Val + (Min_Val * Min_To_Degree) + (Sec_Val * Sec_To_Degree)) * (NEO_GPS.E_OR_W == 'W' ? -1 : 1);

    Deg_Val = (int)((lat / 100));
    Min_Val = (int)(lat - (Deg_Val * 100));
    Sec_Val = ((lat - (Deg_Val * 100)) - Min_Val) * 10;
    NEO_GPS.latitude = (Deg_Val + (Min_Val * Min_To_Degree) + (Sec_Val * Sec_To_Degree)) * (NEO_GPS.N_OR_S == 'S' ? -1 : 1);
}
// radio transmission
