/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32h7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA2D_HandleTypeDef hdma2d;
extern LTDC_HandleTypeDef hltdc;
extern TIM_HandleTypeDef htim7;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */

  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(CT_INT_Pin);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */

  /* USER CODE END TIM7_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_IRQn 1 */

  /* USER CODE END TIM7_IRQn 1 */
}

/**
  * @brief This function handles LTDC global interrupt.
  */
void LTDC_IRQHandler(void)
{
  /* USER CODE BEGIN LTDC_IRQn 0 */

  /* USER CODE END LTDC_IRQn 0 */
  HAL_LTDC_IRQHandler(&hltdc);
  /* USER CODE BEGIN LTDC_IRQn 1 */

  /* USER CODE END LTDC_IRQn 1 */
}

/**
  * @brief This function handles DMA2D global interrupt.
  */
void DMA2D_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2D_IRQn 0 */

  /* USER CODE END DMA2D_IRQn 0 */
  HAL_DMA2D_IRQHandler(&hdma2d);
  /* USER CODE BEGIN DMA2D_IRQn 1 */

  /* USER CODE END DMA2D_IRQn 1 */
}

/* USER CODE BEGIN 1 */
#include "gt911.h"
#include "atk_ncr.h"
#define INT_LOG_EN	0
#if INT_LOG_EN
	#include "dbger.h"
	#define INT_DBG(fmt, ...)		LOG_DBG(fmt, ##__VA_ARGS__)
#else
	#define INT_DBG(fmt, ...)
#endif

extern osMessageQueueId_t coordinateHandle;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	static COORDINATE_t last_pos[NUM_TOUCH_SUPPORT] = {0};
	static uint32_t last_tick[NUM_TOUCH_SUPPORT] = {0};
	osStatus_t osSta;
	
	if(GPIO_Pin == CT_INT_Pin) {
		gt911_get_touch((COORDINATE_t*)touch_coordinate, (uint8_t*)&num_touched);
		if(num_touched) {
			INT_DBG(" detect %d touch(s):\n", num_touched);
			for(uint8_t i = 0; i < num_touched; i++) {
				// INT_DBG("\tP%d(%d, %d)\n", i, touch_coordinate[i].x, touch_coordinate[i].y);
				
				#if 1		// process the handwrite event
					if(touch_coordinate[i].x > 150 && touch_coordinate[i].x < 450 &&
						 touch_coordinate[i].y > 150 && touch_coordinate[i].y < 450 &&
						 last_pos[i].x > 150 && last_pos[i].x < 450 &&
						 last_pos[i].y > 150 && last_pos[i].y < 450)
					{
						if(HAL_GetTick() - last_tick[i] > 20) {
							last_tick[i] = HAL_GetTick();
							last_pos[i].x = touch_coordinate[i].x;
							last_pos[i].y = touch_coordinate[i].y;
							continue;
						}
						last_tick[i] = HAL_GetTick();
						if(last_pos[i].x != touch_coordinate[i].x || last_pos[i].y != touch_coordinate[i].y) {
							#if 0		// draw point
							for(int16_t m = touch_coordinate[i].x - LINE_WIDTH; m <= touch_coordinate[i].x + LINE_WIDTH; m++) {
								for(int16_t n = touch_coordinate[i].y - LINE_WIDTH; n <= touch_coordinate[i].y + LINE_WIDTH; n++) {
									addr = (LTDC_L1_ADDR + (m - LTDC_L1_START_X) * 2 + (n - LTDC_L1_START_Y) * LTDC_L1_WIDTH * 2);
									INT_DBG("P%d(%d, %d) 0x%08x\n", i, touch_coordinate[i].x, touch_coordinate[i].y, addr);
									if(addr >= LTDC_L1_ADDR && addr < LTDC_L1_ADDR + LTDC_L1_WIDTH * LTDC_L1_HEIGHT * 2) {
										*(uint16_t*)addr = 0;
									}
								}
							}
							#else		// draw line
							LOG_DBG("A(%d, %d) --> A(%d, %d)\n", last_pos[i].x, last_pos[i].y, touch_coordinate[i].x, touch_coordinate[i].y);
							//LTDC_draw_line(last_pos[i].x, last_pos[i].y, touch_coordinate[i].x, touch_coordinate[i].y);
							COORDINATE_t coordinate[2] = {{.x = last_pos[i].x, .y = last_pos[i].y}, {.x = touch_coordinate[i].x, .y = touch_coordinate[i].y}};
							if(point_num == 0) {
								draw_coor[0].x = last_pos[i].x;
								draw_coor[0].y = last_pos[i].y;
								draw_coor[1].x = touch_coordinate[i].x;
								draw_coor[1].y = touch_coordinate[i].y;
								point_num = 2;
							} else {
								draw_coor[point_num].x = touch_coordinate[i].x;
								draw_coor[point_num++].y = touch_coordinate[i].y;
							}
							osSta = osMessageQueuePut(coordinateHandle, coordinate, NULL, 0);
							if(osSta != osOK) {
								LOG_ERR("queue put err[%d]\n", osSta);
							}
							#endif
						}
					}
					last_pos[i].x = touch_coordinate[i].x;
					last_pos[i].y = touch_coordinate[i].y;
					#endif
				
			}
		}
	}
}
/* USER CODE END 1 */
