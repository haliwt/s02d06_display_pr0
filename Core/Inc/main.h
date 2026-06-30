/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

#include "stm32g0xx_ll_dma.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_exti.h"
#include "stm32g0xx_ll_cortex.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_tim.h"
#include "stm32g0xx_ll_usart.h"
#include "stm32g0xx_ll_gpio.h"
#include "stm32g0xx_ll_iwdg.h"

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define SMG_POWER_Pin LL_GPIO_PIN_1
#define SMG_POWER_GPIO_Port GPIOA
#define TM1639_STB_Pin LL_GPIO_PIN_4
#define TM1639_STB_GPIO_Port GPIOA
#define M1639_DIO_Pin LL_GPIO_PIN_5
#define M1639_DIO_GPIO_Port GPIOA
#define TM1639_SCLK_Pin LL_GPIO_PIN_6
#define TM1639_SCLK_GPIO_Port GPIOA
#define KEY_MOUSE_Pin LL_GPIO_PIN_0
#define KEY_MOUSE_GPIO_Port GPIOB
#define KEY_PLASMA_Pin LL_GPIO_PIN_1
#define KEY_PLASMA_GPIO_Port GPIOB
#define KEY_DRY_Pin LL_GPIO_PIN_2
#define KEY_DRY_GPIO_Port GPIOB
#define KEY_WIFI_Pin LL_GPIO_PIN_10
#define KEY_WIFI_GPIO_Port GPIOB
#define LED_POWER_Pin LL_GPIO_PIN_12
#define LED_POWER_GPIO_Port GPIOB
#define TIME_LED_EN_Pin LL_GPIO_PIN_14
#define TIME_LED_EN_GPIO_Port GPIOB
#define MOUSE_LED_EN_Pin LL_GPIO_PIN_15
#define MOUSE_LED_EN_GPIO_Port GPIOB
#define PLASMA_LED_EN_Pin LL_GPIO_PIN_8
#define PLASMA_LED_EN_GPIO_Port GPIOA
#define DRY_LED_EN_Pin LL_GPIO_PIN_9
#define DRY_LED_EN_GPIO_Port GPIOA
#define WIFI_LED_EN_Pin LL_GPIO_PIN_6
#define WIFI_LED_EN_GPIO_Port GPIOC
#define KEY_POWER_Pin LL_GPIO_PIN_7
#define KEY_POWER_GPIO_Port GPIOC
#define KEY_MODE_Pin LL_GPIO_PIN_10
#define KEY_MODE_GPIO_Port GPIOA
#define KEY_DEC_Pin LL_GPIO_PIN_11
#define KEY_DEC_GPIO_Port GPIOA
#define KEY_ADD_Pin LL_GPIO_PIN_12
#define KEY_ADD_GPIO_Port GPIOA


/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
