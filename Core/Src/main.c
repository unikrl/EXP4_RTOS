/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <includes.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define START_TASK_PRIO   3
#define LED0_TASK_PRIO    4
#define MSG_TASK_PRIO     5
#define LED1_TASK_PRIO    6

#define START_STK_SIZE    96
#define LED0_STK_SIZE     64
#define MSG_STK_SIZE      128
#define LED1_STK_SIZE     64
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
CPU_STK START_TASK_STK[START_STK_SIZE];
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
CPU_STK MSG_TASK_STK[MSG_STK_SIZE];
CPU_STK LED1_TASK_STK[LED1_STK_SIZE];

OS_TCB StartTaskTCB;
OS_TCB Led0TaskTCB;
OS_TCB MsgTaskTCB;
OS_TCB Led1TaskTCB;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void start_task(void *p_arg);
static void led0_task(void *p_arg);
static void led1_task(void *p_arg);
static void msg_task(void *p_arg);
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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  OS_ERR err;
  OSInit(&err);

  OSTaskCreate((OS_TCB *)&StartTaskTCB,
             (CPU_CHAR *)"start_task",
             (OS_TASK_PTR)start_task,
             (void *)0,
             (OS_PRIO)START_TASK_PRIO,
             (CPU_STK *)&START_TASK_STK[0],
             (CPU_STK_SIZE)START_STK_SIZE / 10,
             (CPU_STK_SIZE)START_STK_SIZE,
             (OS_MSG_QTY)0,
             (OS_TICK)0,
             (void *)0,
             (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
             (OS_ERR *)&err);

  OSStart(&err);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void start_task(void *p_arg) {
    OS_ERR err;


    OSTaskCreate((OS_TCB *)&Led0TaskTCB, (CPU_CHAR *)"led0_task",
                 (OS_TASK_PTR)led0_task, (void *)0, (OS_PRIO)LED0_TASK_PRIO,
                 (CPU_STK *)&LED0_TASK_STK[0], (CPU_STK_SIZE)LED0_STK_SIZE / 10,
                 (CPU_STK_SIZE)LED0_STK_SIZE, (OS_MSG_QTY)0, (OS_TICK)0,
                 (void *)0, (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
                 (OS_ERR *)&err);

    OSTaskCreate((OS_TCB *)&Led1TaskTCB, (CPU_CHAR *)"led1_task",
                 (OS_TASK_PTR)led1_task, (void *)0, (OS_PRIO)LED1_TASK_PRIO,
                 (CPU_STK *)&LED1_TASK_STK[0], (CPU_STK_SIZE)LED1_STK_SIZE / 10,
                 (CPU_STK_SIZE)LED1_STK_SIZE, (OS_MSG_QTY)0, (OS_TICK)0,
                 (void *)0, (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
                 (OS_ERR *)&err);

    OSTaskCreate((OS_TCB *)&MsgTaskTCB, (CPU_CHAR *)"msg_task",
                 (OS_TASK_PTR)msg_task, (void *)0, (OS_PRIO)MSG_TASK_PRIO,
                 (CPU_STK *)&MSG_TASK_STK[0], (CPU_STK_SIZE)MSG_STK_SIZE / 10,
                 (CPU_STK_SIZE)MSG_STK_SIZE, (OS_MSG_QTY)0, (OS_TICK)0,
                 (void *)0, (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
                 (OS_ERR *)&err);

                 OSTaskDel((OS_TCB *)0, &err);
}

static void led0_task(void *p_arg) {
    OS_ERR err;
    (void)p_arg;
    while (1) {
        /* ???LED0_Pin?LED0_GPIO_Port?CubeMX???????????? */
        HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
        OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_HMSM_STRICT, &err); // 1?
    }
}

static void led1_task(void *p_arg) {
    OS_ERR err;
    (void)p_arg;
    while (1) {
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        OSTimeDlyHMSM(0, 0, 3, 0, OS_OPT_TIME_HMSM_STRICT, &err); // 3?
    }
}

static void msg_task(void *p_arg) {
    OS_ERR err;
    (void)p_arg;
    while (1) {
        printf("hello uC/OS! 欢迎来到RTOS多任务环境!\r\n");
        OSTimeDlyHMSM(0, 0, 2, 0, OS_OPT_TIME_HMSM_STRICT, &err); // 2?
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
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
