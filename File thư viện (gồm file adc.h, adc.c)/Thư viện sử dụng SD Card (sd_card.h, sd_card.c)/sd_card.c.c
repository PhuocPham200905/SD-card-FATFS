/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    sd_card.c
  * @brief   Trien khai cac ham xu ly SD Card
  ******************************************************************************
  */
/* USER CODE END Header */

#include "sd_card.h"
#include "usart.h" // De su dung UART in thong bao
#include <string.h>
#include <stdio.h>
#include "spi.h" // De cau hinh lai toc do SPI

// --- Bien toan cuc cho FatFs ---
FATFS fs;
FIL fil;
FRESULT fresult;
FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;
char sd_msg[100]; // Buffer de in thong bao

// --- Ham khoi tao va Mount the nho ---
void SD_Init_Mount(void) {
    HAL_UART_Transmit(&huart1, (uint8_t*)"Dang chuan bi mount the nho (Doi 1-5s)...\r\n", 43, 1000);
    
    // Reset chan CS truoc khi mount (tuy chon, giup on dinh)
    // Da xoa dong reset PC4 vi chan CS thuc te la PA4 (duoc quan ly boi fatfs_sd.c)
    HAL_Delay(100); // Cho mot chut de nguon on dinh

    fresult = f_mount(&fs, "/", 1);
    if (fresult != FR_OK) {
        snprintf(sd_msg, sizeof(sd_msg), "LOI Mount: %d. Thu Format...\r\n", fresult);
        HAL_UART_Transmit(&huart1, (uint8_t*)sd_msg, strlen(sd_msg), 1000);
        
        if (fresult == FR_NO_FILESYSTEM) {
            fresult = f_mkfs("", 1, 0); // Format
            if (fresult == FR_OK) {
                HAL_UART_Transmit(&huart1, (uint8_t*)"Format OK! Remounting...\r\n", 26, 1000);
                f_mount(NULL, "/", 1); // Unmount
                fresult = f_mount(&fs, "/", 1); // Remount
            }
        }
    }
    
    if (fresult == FR_OK) {
        HAL_UART_Transmit(&huart1, (uint8_t*)"-> Mount THANH CONG!\r\n", 22, 1000);
        // Tang toc do SPI sau khi khoi tao thanh cong
        // --- FIX: GIU NGUYEN TOC DO CHAM DE DAM BAO ON DINH ---
        // hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
        // HAL_SPI_Init(&hspi1);
    } else {
        HAL_UART_Transmit(&huart1, (uint8_t*)"-> Mount THAT BAI!\r\n", 20, 1000);
    }
}

// --- Ham ghi du lieu vao cuoi file (Append) ---
void SD_Write_File(char *filename, char *data) {
    fresult = f_open(&fil, filename, FA_OPEN_ALWAYS | FA_WRITE);
    if (fresult == FR_OK) {
        f_lseek(&fil, f_size(&fil)); // Di chuyen xuong cuoi file
        f_puts(data, &fil);
        f_puts("\n", &fil); // Xuong dong
        f_close(&fil);
        HAL_UART_Transmit(&huart1, (uint8_t*)"-> Ghi file OK!\r\n", 17, 100);
    } else {
        snprintf(sd_msg, sizeof(sd_msg), "-> LOI Ghi: %d\r\n", fresult);
        HAL_UART_Transmit(&huart1, (uint8_t*)sd_msg, strlen(sd_msg), 100);
    }
}

// --- Ham doc toan bo file ---
void SD_Read_File(char *filename) {
    char read_buf[100];
    HAL_UART_Transmit(&huart1, (uint8_t*)"--- Noi dung file ---\r\n", 23, 100);
    
    fresult = f_open(&fil, filename, FA_READ);
    if (fresult == FR_OK) {
        while (f_gets(read_buf, sizeof(read_buf), &fil)) {
            HAL_UART_Transmit(&huart1, (uint8_t*)read_buf, strlen(read_buf), 100);
        }
        f_close(&fil);
        HAL_UART_Transmit(&huart1, (uint8_t*)"\r\n--- Het ---\r\n", 13, 100);
    } else {
        HAL_UART_Transmit(&huart1, (uint8_t*)"-> Khong mo duoc file.\r\n", 24, 100);
    }
}

// --- Ham kiem tra dung luong ---
void SD_Check_Capacity(void) {
    fresult = f_getfree("", &fre_clust, &pfs);
    if (fresult == FR_OK) {
        total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
        free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
        
        snprintf(sd_msg, sizeof(sd_msg), "Tong: %lu KB, Trong: %lu KB\r\n", total, free_space);
        HAL_UART_Transmit(&huart1, (uint8_t*)sd_msg, strlen(sd_msg), 1000);
    } else {
        HAL_UART_Transmit(&huart1, (uint8_t*)"Loi doc dung luong.\r\n", 21, 1000);
    }
}