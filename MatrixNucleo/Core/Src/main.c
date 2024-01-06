/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "disp1color.h"
#include "font.h"
#include "f6x8m.h"
#include "morse.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define TIME_REF 500
#define DEBOUNCE 3

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int16_t X = 32;
uint16_t Period = 70;
uint16_t TickNum = 1; //150
uint8_t data1[16], data2[16], data3[16], data4[16];
uint16_t strSize;

char CurrentCode[50];
char Text[50];
char *letter;

extern volatile uint16_t Flag;

uint16_t IsNewLetter = 1;
uint16_t IsNewWord = 1;

uint32_t clicktime;
extern volatile uint32_t HighTime;
extern volatile uint32_t LowTime;
extern volatile uint64_t TimeStamp;

uint16_t trash;

void disp_row(int row) {
	if (row == 0) {
		HAL_SPI_Transmit(&hspi1, data1, 16, 10);
		while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY) {
		}

		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);

		HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_RESET);
	}
	if (row == 1) {
		HAL_SPI_Transmit(&hspi1, data2, 16, 10);
		while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY) {
		}

		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);

		HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_RESET);
	}

	if (row == 2) {
		HAL_SPI_Transmit(&hspi1, data3, 16, 10);
		while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY) {
		}

		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);

		HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_SET);
	}

	if (row == 3) {
		HAL_SPI_Transmit(&hspi1, data4, 16, 10);
		while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY) {
		}

		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_SET);
	}
	HAL_GPIO_WritePin(nOE_GPIO_Port, nOE_Pin, GPIO_PIN_SET);
//	for (uint32_t x = 0; x <= 3000; x++) {
//	};
//	HAL_GPIO_WritePin(nOE_GPIO_Port, nOE_Pin, GPIO_PIN_RESET);

//	HAL_GPIO_WritePin(nOE_GPIO_Port, nOE_Pin, GPIO_PIN_RESET);
}

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_GPIO_WritePin(nOE_GPIO_Port, nOE_Pin, GPIO_PIN_RESET);
	char pMyStr[] = { 0xCF, 0xF0, 0xE8, 0xE2, 0xE5, 0xF2 };  //Строка на вывод

	uint8_t symbolDelay = 2;

  /* USER CODE END 2 */
абы
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	while (1) {
		if (Flag) {
			if (HighTime >= 5 * TIME_REF) {
				memset(CurrentCode, 0, strlen(CurrentCode));
				if (HighTime >= 7 * TIME_REF) {
					memset(Text, 0, strlen(Text));
					IsNewWord = 1;
				}
			}
		} else {
			if (!IsNewLetter) {
				if ((LowTime > 3 * TIME_REF) && (clicktime < 7 * TIME_REF)) {
					letter = codeToLetter(CurrentCode);
					strcat(Text, letter);
					memset(CurrentCode, 0, strlen(CurrentCode));
					IsNewLetter = 1;
				}
			}
			if (IsNewLetter && !IsNewWord && (LowTime >= 7 * TIME_REF)) {
				memset(CurrentCode, 0, strlen(CurrentCode));
				strcat(Text, space);
				IsNewWord = 1;
			}
		}

		disp1color_FillScreenbuff(0);
		disp1color_printf(2, 4, FONTID_6X8M, Text); //Расчет данных для вывода
		disp1color_UpdateFromBuff(); //Перевод рассчитанных данных в массив
		prepare_data(); //Разбиение массива на массивы под каждую строку

		disp_row(0);
		disp_row(1);
		disp_row(2);
		disp_row(3);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) { //EXTI interrupt callback
	if (GPIO_Pin == GPIO_PIN_13) { // check interrupt for specific pin
		clicktime = HAL_GetTick() - TimeStamp;

		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)) { //check pin state
			Flag = 1; // Pin is pulled up, so state is reversed
		}

		if (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)) {
			Flag = 0;
		}

		IsNewWord = 0;
		IsNewLetter = 0;
		if (!Flag) {
			LowTime = 0;
			if ((clicktime >= DEBOUNCE) && (clicktime <= TIME_REF)) {
				strcat(CurrentCode, dot);
			} else if ((clicktime >= TIME_REF) && (clicktime <= 3 * TIME_REF)) {
				strcat(CurrentCode, dash);

			}
		}
		TimeStamp = HAL_GetTick();
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
