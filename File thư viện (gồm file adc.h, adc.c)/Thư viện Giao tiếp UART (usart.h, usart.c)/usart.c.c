/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */
void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/* Ham nhan chuoi tu UART */
void UART_RxString(char *buf, uint16_t max_len) {
    uint16_t i = 0;
    uint8_t ch;
    while (i < max_len - 1) {
        // Cho nhan 1 ky tu
        if (HAL_UART_Receive(&huart1, &ch, 1, HAL_MAX_DELAY) == HAL_OK) {
            // Neu la phim Enter (\r hoac \n) thi ket thuc
            if (ch == '\r' || ch == '\n') {
                HAL_UART_Transmit(&huart1, (uint8_t*)"\r\n", 2, 100); // Xuong dong
                break;
            }
            // Neu la phim Backspace (xoa)
            else if (ch == '\b' || ch == 0x7F) {
                 if(i > 0) {
                     i--;
                     HAL_UART_Transmit(&huart1, (uint8_t*)"\b \b", 3, 100); // Xoa ky tu tren man hinh
                 }
            }
            else {
                buf[i++] = ch; // Luu vao buffer
                HAL_UART_Transmit(&huart1, &ch, 1, 100); // Hien thi lai ky tu vua go (Echo)
            }
        }
    }
    buf[i] = '\0'; // Ket thuc chuoi
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/