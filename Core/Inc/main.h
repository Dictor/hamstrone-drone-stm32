/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
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
#include "hamster_tongue.h"
#include "value_store.h"
#include "config.h"
#include "cmsis_os2.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define HAMSTRONE_I2C_HANDLE_TYPE I2C_HandleTypeDef
#define HAMSTRONE_SPI_HANDLE_TYPE SPI_HandleTypeDef
#define HAMSTRONE_TIM_HANDLE_TYPE TIM_HandleTypeDef
#include "hamstrone_task.h" // this header file refer above 3 macros.
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern osSemaphoreId_t HAMSTRONE_GLOBAL_TELEMETRY_SEMAPHORE;
extern UART_HandleTypeDef *HAMSTRONE_GLOBAL_TELEMETRY_PORT, *HAMSTRONE_GLOBAL_SERIAL_PORT;
extern HAMSTRONE_I2C_HANDLE_TYPE *HAMSTRONE_GLOBAL_I2C_PORT;
extern HAMSTRONE_SPI_HANDLE_TYPE *HAMSTRONE_GLOBAL_SPI_PORT;
extern HAMSTRONE_TIM_HANDLE_TYPE *HAMSTRONE_GLOBAL_MOTOR_PWM;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define HAMSTERTONGUE_InitFailf(...)                    \
    HAMSTERTONGUE_WriteAndFreeMessage(                  \
        HAMSTRONE_GLOBAL_TELEMETRY_PORT,                \
        HAMSTERTONGUE_NewFormatStringMessage(           \
            HAMSTERTONGUE_MESSAGE_VERB_SIGNAL,          \
            HAMSTERTONGUE_MESSAGE_NOUN_SIGNAL_INITFAIL, \
            48, __VA_ARGS__));
/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define VCP_TX_Pin GPIO_PIN_2
#define VCP_TX_GPIO_Port GPIOA
#define VCP_RX_Pin GPIO_PIN_3
#define VCP_RX_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define LD3_Pin GPIO_PIN_3
#define LD3_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
