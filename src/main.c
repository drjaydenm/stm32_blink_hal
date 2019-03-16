#include "main.h"

static GPIO_InitTypeDef GPIOB_OutputLedConfig;
static GPIO_InitTypeDef GPIOF_OutputLedConfig;
static GPIO_InitTypeDef GPIOA_InputButtonConfig;

int main ()
{
    HAL_Init();

    SystemClockConfig();

    SetupGPIO();

    int flashDelay = 0;
    while (1)
    {
        if (flashDelay >= 100000)
        {
            flashDelay = 0;
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
        }

        if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))
        {
            HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, GPIO_PIN_RESET);
        }

        flashDelay += 1;
    }
}

void SetupGPIO()
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();

    // Configure the onboard LED
    GPIOB_OutputLedConfig.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOB_OutputLedConfig.Pull = GPIO_PULLUP;
    GPIOB_OutputLedConfig.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIOB_OutputLedConfig.Pin = GPIO_PIN_3;

    HAL_GPIO_Init(GPIOB, &GPIOB_OutputLedConfig);

    // Configure the output LEDs
    GPIOF_OutputLedConfig.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOF_OutputLedConfig.Pull = GPIO_PULLUP;
    GPIOF_OutputLedConfig.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIOF_OutputLedConfig.Pin = GPIO_PIN_0;

    HAL_GPIO_Init(GPIOF, &GPIOF_OutputLedConfig);

    GPIOA_InputButtonConfig.Mode = GPIO_MODE_INPUT;
    GPIOA_InputButtonConfig.Pull = GPIO_PULLUP;
    GPIOA_InputButtonConfig.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIOA_InputButtonConfig.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_4;

    HAL_GPIO_Init(GPIOA, &GPIOA_InputButtonConfig);
}

void SystemClockConfig(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    // No HSE Oscillator on Nucleo, Activate PLL with HSI as source
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
    {
        // Initialization Error
        while(1);
    }

    // Select PLL as system clock source and configure the HCLK, PCLK1 clocks dividers
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
        // Initialization Error
        while(1);
    }
}