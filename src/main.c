#include "main.h"

void SystemClock_Config(void);

int main ()
{
    HAL_Init();

    SystemClock_Config();
    
    // Enable GPIOB peripheral
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOFEN;

    // Setup pin B3 for output and push-pull
    GPIOB->MODER &= ~GPIO_MODER_MODER3;
    GPIOB->MODER |= GPIO_MODER_MODER3_0;
    GPIOB->OTYPER &= ~GPIO_OTYPER_OT_3;

    // Setup pin F4 for output and push-pull
    GPIOF->MODER &= ~GPIO_MODER_MODER0;
    GPIOF->MODER |= GPIO_MODER_MODER0_0;
    GPIOF->OTYPER &= ~GPIO_OTYPER_OT_0;

    // Setup pin B1 for input and pull up
    GPIOA->MODER &= ~GPIO_MODER_MODER0;
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR0;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_0;

    int flashDelay = 0;
    while (1)
    {
        if (flashDelay >= 100000)
        {
            flashDelay = 0;
            GPIOB->ODR ^= (1 << 3);
        }

        if (~GPIOA->IDR & (1 << 0))
        {
            GPIOF->BSRR = (1 << 0);
        }
        else
        {
            GPIOF->BRR = (1 << 0);
        }

        flashDelay += 1;
    }
}

void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    /* No HSE Oscillator on Nucleo, Activate PLL with HSI as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
    {
        /* Initialization Error */
        while(1); 
    }

    /* Select PLL as system clock source and configure the HCLK, PCLK1 clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1)!= HAL_OK)
    {
        /* Initialization Error */
        while(1); 
    }
}