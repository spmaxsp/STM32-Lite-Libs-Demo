/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include <stdio.h>

#include "sl2_gpio.h"
#include "sl2_spi.h"
#include "sl2_max6675.h"


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    /* SysTick_IRQn interrupt configuration */
    // NVIC_SetPriority(SysTick_IRQn, 3);   // SysTick_ISR not used in this project

    /** Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral */
    LL_SYSCFG_DisableDBATT(LL_SYSCFG_UCPD1_STROBE | LL_SYSCFG_UCPD2_STROBE);

    /* Configure the system clock */
    SystemClock_Config();

    LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_PCLK1);
    LL_RCC_SetTIMClockSource(LL_RCC_TIM1_CLKSOURCE_PCLK1);

    /* Peripherals clock enable */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM14);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOD);

    /* Configure SPI2 */
    SL2_SPI_t spi1;
    SL2_SPI_Init(&spi1, SPI2, SL2_SPI2_SCK_PA0, SL2_SPI2_MISO_PD3, SL2_SPI2_MOSI_PB15);
    //SL2_SPI_SetMode(&spi1, SL2_SPI_MODE_MASTER);
    //SL2_SPI_SetBitOrder(&spi1, SL2_SPI_MSB_FIRST);
    //SL2_SPI_SetPolPhase(&spi1, SL2_SPI_POLARITY_LOW, SL2_SPI_PHASE_1EDGE);
    //SL2_SPI_SetClockPrescaler(&spi1, LL_SPI_BAUDRATEPRESCALER_DIV8);


    SL2_MAX6675_t max6675;
    SL2_MAX6675_Init(&max6675, &spi1, SL2_GPIO_PB6);

    while (1)
    {
        SL2_MAX6675_ForceConversion(&max6675);
        uint16_t temp = SL2_MAX6675_ReadRAW(&max6675);
        printf("Temperature: %d\n", temp);
    }


    /* Infinite loop */
    while (1)
    {
    }
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  /* HSI configuration and activation */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* Sysclk activation on the HSI */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  }

  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_Init1msTick(16000000);
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(16000000);
}
