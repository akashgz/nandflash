/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>  // For memset

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
NAND_HandleTypeDef hnand1;
/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_FSMC_Init(void);
/* USER CODE BEGIN PFP */
void NAND_Test();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_FSMC_Init();

  /* USER CODE BEGIN 2 */
  NAND_Test();  // Run test
  /* USER CODE END 2 */

  while (1)
  {
    /* USER CODE BEGIN WHILE */
    // Application loop
    /* USER CODE END WHILE */
  }
}

/* USER CODE BEGIN 4 */

void NAND_Test(void)
{
  uint8_t txBuffer[2048];  // NAND page size
  uint8_t rxBuffer[2048];
  NAND_AddressTypeDef nandAddress;

  // Fill transmit buffer with pattern
  for (uint16_t i = 0; i < sizeof(txBuffer); i++)
  {
    txBuffer[i] = i % 256;
  }

  // Address: Block 0, Page 0, Plane 0
  nandAddress.Plane = 0;
  nandAddress.Block = 0;
  nandAddress.Page = 0;

  // Erase block
  if (HAL_NAND_Erase_Block(&hnand1, &nandAddress) != HAL_OK)
  {
    Error_Handler();
  }

  // Write data
  if (HAL_NAND_Write_Page_8b(&hnand1, &nandAddress, txBuffer, 1) != HAL_OK)
  {
    Error_Handler();
  }

  // Clear RX buffer
  memset(rxBuffer, 0, sizeof(rxBuffer));

  // Read data
  if (HAL_NAND_Read_Page_8b(&hnand1, &nandAddress, rxBuffer, 1) != HAL_OK)
  {
    Error_Handler();
  }

  // Verify data
  for (uint16_t i = 0; i < sizeof(txBuffer); i++)
  {
    if (txBuffer[i] != rxBuffer[i])
    {
      Error_Handler();  // Mismatch
    }
  }

  // Optional: Success LED toggle or UART print
}

/* USER CODE END 4 */

/**
  * @brief System Clock Configuration
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                              | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
}

/**
  * @brief GPIO Initialization
  */
static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
}

/**
  * @brief FSMC Initialization
  */
static void MX_FSMC_Init(void)
{
  FSMC_NAND_PCC_TimingTypeDef ComSpaceTiming = {0};
  FSMC_NAND_PCC_TimingTypeDef AttSpaceTiming = {0};

  hnand1.Instance = FSMC_NAND_DEVICE;
  hnand1.Init.NandBank = FSMC_NAND_BANK2;
  hnand1.Init.Waitfeature = FSMC_NAND_PCC_WAIT_FEATURE_DISABLE;
  hnand1.Init.MemoryDataWidth = FSMC_NAND_PCC_MEM_BUS_WIDTH_8;
  hnand1.Init.EccComputation = FSMC_NAND_ECC_DISABLE;
  hnand1.Init.ECCPageSize = FSMC_NAND_ECC_PAGE_SIZE_512BYTE;
  hnand1.Init.TCLRSetupTime = 1;
  hnand1.Init.TARSetupTime = 1;

  hnand1.Config.PageSize = 2048;
  hnand1.Config.SpareAreaSize = 64;
  hnand1.Config.BlockSize = 64;
  hnand1.Config.BlockNbr = 2048;
  hnand1.Config.PlaneNbr = 1;
  hnand1.Config.PlaneSize = 1024;
  hnand1.Config.ExtraCommandEnable = DISABLE;

  ComSpaceTiming.SetupTime = 2;
  ComSpaceTiming.WaitSetupTime = 2;
  ComSpaceTiming.HoldSetupTime = 2;
  ComSpaceTiming.HiZSetupTime = 2;
  AttSpaceTiming = ComSpaceTiming;

  if (HAL_NAND_Init(&hnand1, &ComSpaceTiming, &AttSpaceTiming) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Error Handler
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
    // Optional: Blink LED to indicate error
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif
