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
    // CDC_Transmit_FS(NEO_GPS.data, strlen(NEO_GPS.data));
    char *start = strstr((char *)NEO_GPS.data, "GPGGA");
    if (start != NULL)
    {
        char *newline_position = strchr(start, '\n');
        if (newline_position != NULL)
        {
            size_t length = newline_position - start + 1; // calculate GPGAA substring length
            NEO_GPS.GPGGA = (uint8_t *)malloc(length * sizeof(uint8_t));
            strncpy((char *)NEO_GPS.GPGGA, start, length);

            sscanf((char *)NEO_GPS.GPGGA, "GPGGA,%lf,%lf,%c,%lf,%c,,,,%lf,,%lf,,,", &NEO_GPS.time,
                   &NEO_GPS.latitude, &NEO_GPS.N_OR_S, &NEO_GPS.longitude, &NEO_GPS.E_OR_W, &NEO_GPS.altitude_approx,
                   &NEO_GPS.altitude_correction);
            Gps_Data_Conversion();
            sprintf(NEO_GPS.result, "Time: %.5lf Latitude: %.5lf Longitude: %.5lf\r\n",
                    NEO_GPS.time, NEO_GPS.latitude, NEO_GPS.longitude);
            // CDC_Transmit_FS(NEO_GPS.GPGGA, length);
            CDC_Transmit_FS(NEO_GPS.result, strlen(NEO_GPS.result)); // transmit Latitude and Longitude
            // write result to SD card
            sd_write(NEO_GPS.result);
            free(NEO_GPS.GPGGA);

            // transmit radio data
            // HAL_UART_Transmit_DMA(huart, NEO_GPS.result, 100);
        }
        /*size_t offset = start - (char*) data;
        return data + offset;*/
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
