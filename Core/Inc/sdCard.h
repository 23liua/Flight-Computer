
#ifndef INC_sdCard_H_
#define INC_sdCard_H_

//#include "fatfs_sd.h"
#include "fatfs.h"
#include "stm32f4xx_hal.h"
#include "string.h"
#include "usb_device.h"
#include "usbd_cdc.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern FATFS fs; //file system
extern FIL fil; //file
extern FRESULT fresult; //store results
extern char buffer[1024]; //buffer to store data

extern UINT br, bw;

extern FATFS *pfs;
extern DWORD fre_clust;
extern uint32_t free_space;
extern uint32_t total;

void CDC_Transmit_FS(uint8_t* data, uint16_t length);
int bufferSize (char *buff);
void bufferClear (char *buff);
void cdcTransmit(char *buff);
void sd_Start_Up();
void sd_write();
void sd_read();

#endif
