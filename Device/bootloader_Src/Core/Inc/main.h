/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void deInitializer(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ESP_EN_Pin GPIO_PIN_0
#define ESP_EN_GPIO_Port GPIOC
#define ESP_IO9_Pin GPIO_PIN_1
#define ESP_IO9_GPIO_Port GPIOC
#define CONF_DONE_Pin GPIO_PIN_2
#define CONF_DONE_GPIO_Port GPIOC
#define nCONFIG_Pin GPIO_PIN_3
#define nCONFIG_GPIO_Port GPIOC
#define DATA0_Pin GPIO_PIN_0
#define DATA0_GPIO_Port GPIOA
#define DCLK_Pin GPIO_PIN_1
#define DCLK_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define nSTATUS_Pin GPIO_PIN_4
#define nSTATUS_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define OK_LED_Pin GPIO_PIN_3
#define OK_LED_GPIO_Port GPIOB
#define BUSY_LED_Pin GPIO_PIN_4
#define BUSY_LED_GPIO_Port GPIOB
#define ERROR_LED_Pin GPIO_PIN_5
#define ERROR_LED_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */