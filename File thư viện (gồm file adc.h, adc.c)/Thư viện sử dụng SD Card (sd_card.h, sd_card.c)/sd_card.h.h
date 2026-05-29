/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    sd_card.h
  * @brief   Thu vien quan ly SD Card (High Level)
  ******************************************************************************
  */
/* USER CODE END Header */
#ifndef __SD_CARD_H
#define __SD_CARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "fatfs.h"

void SD_Init_Mount(void);
void SD_Write_File(char *filename, char *data);
void SD_Read_File(char *filename);
void SD_Check_Capacity(void);

#ifdef __cplusplus
}
#endif

#endif /* __SD_CARD_H */