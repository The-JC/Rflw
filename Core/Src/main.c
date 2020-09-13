/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

/* Private includes ----------------------------------------------------------*/
#include "config.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi2;

#ifdef ENABLE_WWDG
WWDG_HandleTypeDef hwwdg;
#endif /* ENABLE_WWDG */

TaskHandle_t systemMonitorHandle;
TaskHandle_t inputHandle;
TaskHandle_t menuHandle;
TaskHandle_t controlHandle;
TaskHandle_t LCDHandle;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI2_Init(void);
#ifdef ENABLE_WWDG
static void MX_WWDG_Init(void);
#endif /* ENABLE_WWDG */
void systemMonitorTask(void * argument);
extern void vInputTask(void * argument);
extern void vMenuTask(void * argument);
extern void vControlTask(void * argument);
extern void vLCDTask(void * argument);

/* Private user code ---------------------------------------------------------*/

void InitSystem(void) {
	SystemClock_Config();

	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_SPI2_Init();
	#ifdef ENABLE_WWDG
	MX_WWDG_Init();
	#endif /* ENABLE_WWDG */

//	HAL_SPI_RegisterCallback();
}

void InitOS(void) {
	xSystemMonitorTask =  xTaskCreateStatic(systemMonitorTask, SYSTEM_MONITOR_NAME, SYSTEM_MONITOR_STACK_SIZE, NULL, SYSTEM_MONITOR_PRIORITY, xSystemMonitorStack, &xSystemMonitorBuffer);
	
	xInputHandlerTask =  xTaskCreateStatic(vInputTask, INPUT_HANDLER_NAME, INPUT_HANDLER_STACK_SIZE, NULL, INPUT_HANDLER_PRIORITY, xInputHandlerStack, &xInputHandlerBuffer);

	xLCDTask =  xTaskCreateStatic(vLCDTask, LCD_NAME, LCD_STACK_SIZE, NULL, LCD_PRIORITY, xLCDStack, &xLCDBuffer);

	xMenuTask =  xTaskCreateStatic(vMenuTask, MENU_NAME, MENU_STACK_SIZE, NULL, MENU_PRIORITY, xMenuStack, &xMenuBuffer);

	xControlTask =  xTaskCreateStatic(vControlTask, CONTROL_NAME, CONTROL_STACK_SIZE, NULL, CONTROL_PRIORITY, xControlStack, &xControlBuffer);

	xI2CMutex = xSemaphoreCreateMutexStatic(&xI2CMutexBuffer);

	xSPIMutex = xSemaphoreCreateMutexStatic(&xSPIMutexBuffer);

	xLCDMutex = xSemaphoreCreateMutexStatic(&xLCDMutexBuffer);

	/* Start scheduler */
	vTaskStartScheduler();
}

/**
  * @brief  The application entry point.
  * @retval int
  */

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Initializes the CPU, AHB and APB busses clocks 
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks 
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
								|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
	/** Enables the Clock Security System 
	 */
	HAL_RCC_EnableCSS();
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void) {
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 400000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void) {
	/* SPI2 parameter configuration*/
	hspi2.Instance = SPI2;
	hspi2.Init.Mode = SPI_MODE_MASTER;
	hspi2.Init.Direction = SPI_DIRECTION_2LINES_RXONLY;
	hspi2.Init.DataSize = SPI_DATASIZE_16BIT;
	hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi2.Init.NSS = SPI_NSS_SOFT;
	hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi2.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi2) != HAL_OK) {
		Error_Handler();
	}

}

#ifdef ENABLE_WWDG
/**
  * @brief WWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_WWDG_Init(void) {
	hwwdg.Instance = WWDG;
	hwwdg.Init.Prescaler = WWDG_PRESCALER_8;
	hwwdg.Init.Window = 96;
	hwwdg.Init.Counter = 127;
	hwwdg.Init.EWIMode = WWDG_EWI_DISABLE;
	if (HAL_WWDG_Init(&hwwdg) != HAL_OK) {
		Error_Handler();
	}
}
#endif /* ENABLE_WWDG */

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LD_Power_GPIO_Port, LD_Power_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, CS2_Pin|CS_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin : HEATER_Pin */
	GPIO_InitStruct.Pin = HEATER_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(HEATER_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : LD_Power_Pin */
	GPIO_InitStruct.Pin = LD_Power_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LD_Power_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : LEFT_Pin RIGHT_Pin */
	GPIO_InitStruct.Pin = LEFT_Pin|RIGHT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : DOWN_Pin SELECT_Pin UP_Pin */
	GPIO_InitStruct.Pin = DOWN_Pin|SELECT_Pin|UP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : CS2_Pin CS_Pin */
	GPIO_InitStruct.Pin = CS2_Pin|CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/**
  * @brief  Function implementing the systemMonitor thread.
  * This function feeds the window watchdog, it won't be executed if a fault occuers.
  * 
  * @param  argument: Not used 
  * @retval None
  */
void systemMonitorTask(void * argument){
	// Every 48ms the window watchdog has the to refreshed, else the System will be restarted
	const TickType_t xDelay = 48 / portTICK_PERIOD_MS; // 48ms -  
  	/* Infinite loop */
	while(1) {
		vTaskDelay(xDelay);
	#ifdef ENABLE_WWDG
		HAL_WWDG_Refresh(&hwwdg);
	#endif /* ENABLE_WWDG */
	}
}

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM1) {
		HAL_IncTick();
	}
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	while(1) {
		HAL_GPIO_TogglePin(LD_Power_GPIO_Port, LD_Power_Pin);
		HAL_Delay(100);
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line) { 
  
  	/* User can add his own implementation to report the file name and line number,
	  tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
