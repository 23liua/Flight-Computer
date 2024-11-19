#include "sdCard.h"

FATFS fs;          // file system
FIL fil;           // file
FRESULT fresult;   // store results
char buffer[1024]; // buffer to store data

UINT br, bw;

FATFS *pfs;
DWORD fre_clust;
uint32_t free_space;
uint32_t total;

int buffer_Size(char *buff)
{
    int i = 0;
    while (*buff++ != '\0')
        i++;
    return i;
}
// clear buffer
void buffer_Clear(char *buff)
{
    for (int i = 0; i < 1024; i++)
    {
        buffer[i] = '\0';
    }
}
void cdc_Transmit(char *buff)
{
    CDC_Transmit_FS((uint8_t *)buff, strlen(buff));
}

void sd_Start_Up()
{
    fresult = f_mount(&fs, "", 1);
    if (fresult == FR_NOT_READY)
    {
        cdc_Transmit("SD card failed to mount!\r\n");
    }
    else
    {
        cdc_Transmit("SD card mounting successful!\r\n");
    }
    // cdc_Transmit("fresult stuck");
    fresult = f_getfree("", &fre_clust, &pfs);
    if (fresult != FR_OK)
    {
        cdc_Transmit("FR_NOT_READY Error!\r\n");
    }
    else
    {
        cdc_Transmit("f_getfree is OK!\r\n");
        total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
        sprintf(buffer, "SD CARD Total Size: %lu\r\n", total);
        cdc_Transmit(buffer);
        buffer_Clear(buffer);
        free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
        sprintf(buffer, "SD CARD Free Space: %lu\r\n", free_space);
        cdc_Transmit(buffer);
        buffer_Clear(buffer);
    }
    // need to add confimration of SD card mounting that can be transmitted to Ground station

    file_naming();
}

// file naming for now just forcibly overwrites the file
void file_naming()
{
    fresult = f_open(&fil, "flight_log.txt", FA_CREATE_ALWAYS | FA_WRITE);
    const char *title = "time, latitude, longitude, pressure, temperature, altitude\n";
    fresult = f_puts(title, &fil);
    fresult = f_close(&fil);
    // cdc_Transmit("flight_log1.txt created and headers written\r\n");
}

void sd_write(char *input)
{
    fresult = f_open(&fil, "flight_log.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_OPEN_APPEND);
    // f_lseek(&fil, f_size(&fil));
    fresult = f_puts(input, &fil);
    fresult = f_close(&fil);
    // cdc_Transmit("written\r\n");
    //  sd_read(input);
}

// reads back what's being written onto terminal
void sd_read(char *input)
{
    fresult = f_open(&fil, "flight_log1.txt", FA_READ);
    f_gets(input, strlen(input), &fil);
    strcpy(buffer, input);
    strcat(buffer, "\r");
    cdc_Transmit(buffer);
    fresult = f_close(&fil);
    buffer_Clear(buffer);
}
