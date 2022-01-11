/* 
 * MIT License
 * -----------
 * 
 * Copyright (c) 2022 Technical university of Liberec (https://tul.cz)
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "nrf24l01_hal_stm32f7xx.h"

#if defined NRF24L01_TRACE_INC
#include NRF24L01_TRACE_INC
#endif

static SPI_HandleTypeDef nrfSpi;
static nrf24l01_callback_t nrfCallback;
static void *nrfContext;

nrf24l01_hal_t nrf24l01_hal_stm32f7xx = {
    &nrf24l01_hal_stm32f7xx_initialize,
    &nrf24l01_hal_stm32f7xx_spiTransfer,
    &nrf24l01_hal_stm32f7xx_powerControl,
    &nrf24l01_hal_stm32f7xx_selectControl,
    &nrf24l01_hal_stm32f7xx_clockControl,
    &nrf24l01_hal_stm32f7xx_irqState,
    &nrf24l01_hal_stm32f7xx_irqSource,
    &nrf24l01_hal_stm32f7xx_irqTrigger,
    &nrf24l01_hal_stm32f7xx_deinitialize, };

void nrf24l01_hal_stm32f7xx_initialize(nrf24l01_callback_t callback, void *context) {
    GPIO_InitTypeDef GPIO_InitStruct;

    NRF24L01_PWR_CLK_EN();

    GPIO_InitStruct.Pin             = NRF24L01_PWR_PIN;
    GPIO_InitStruct.Mode            = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull            = NRF24L01_PWR_PULL;
    GPIO_InitStruct.Speed           = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate       = 0;
    HAL_GPIO_Init((GPIO_TypeDef*) NRF24L01_PWR_PORT, &GPIO_InitStruct);
#if (NRF24L01_LOCK_PINS == 1)
    HAL_GPIO_LockPin((GPIO_TypeDef*) NRF24L01_PWR_PORT, NRF24L01_PWR_PIN);
#endif

    HAL_GPIO_WritePin((GPIO_TypeDef*) NRF24L01_PWR_PORT, NRF24L01_PWR_PIN, GPIO_PIN_SET);

    NRF24L01_MISO_CLK_EN();

    GPIO_InitStruct.Pin             = NRF24L01_MISO_PIN;
    GPIO_InitStruct.Mode            = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull            = GPIO_NOPULL;
    GPIO_InitStruct.Speed           = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate       = NRF24L01_MISO_AF;
    HAL_GPIO_Init((GPIO_TypeDef*) NRF24L01_MISO_PORT, &GPIO_InitStruct);
#if (NRF24L01_LOCK_PINS == 1)
    HAL_GPIO_LockPin((GPIO_TypeDef*) NRF24L01_MISO_PORT, NRF24L01_MISO_PIN);
#endif

    NRF24L01_MOSI_CLK_EN();

    GPIO_InitStruct.Pin             = NRF24L01_MOSI_PIN;
    GPIO_InitStruct.Mode            = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull            = GPIO_NOPULL;
    GPIO_InitStruct.Speed           = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate       = NRF24L01_MOSI_AF;
    HAL_GPIO_Init((GPIO_TypeDef*) NRF24L01_MOSI_PORT, &GPIO_InitStruct);
#if (NRF24L01_LOCK_PINS == 1)
    HAL_GPIO_LockPin((GPIO_TypeDef*) NRF24L01_MOSI_PORT, NRF24L01_MOSI_PIN);
#endif

    NRF24L01_SCK_CLK_EN();

    GPIO_InitStruct.Pin             = NRF24L01_SCK_PIN;
    GPIO_InitStruct.Mode            = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull            = GPIO_NOPULL;
    GPIO_InitStruct.Speed           = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate       = NRF24L01_SCK_AF;
    HAL_GPIO_Init((GPIO_TypeDef*) NRF24L01_SCK_PORT, &GPIO_InitStruct);
#if (NRF24L01_LOCK_PINS == 1)
    HAL_GPIO_LockPin((GPIO_TypeDef*) NRF24L01_SCK_PORT, NRF24L01_MOSI_PIN);
#endif

    NRF24L01_CSN_CLK_EN();

    GPIO_InitStruct.Pin             = NRF24L01_CSN_PIN;
    GPIO_InitStruct.Mode            = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull            = NRF24L01_CSN_PULL;
    GPIO_InitStruct.Speed           = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate       = 0;
    HAL_GPIO_Init((GPIO_TypeDef*) NRF24L01_CSN_PORT, &GPIO_InitStruct);
#if (NRF24L01_LOCK_PINS == 1)
    HAL_GPIO_LockPin((GPIO_TypeDef*) NRF24L01_CSN_PORT, NRF24L01_CSN_PIN);
#endif

    NRF24L01_CE_CLK_EN();

    GPIO_InitStruct.Pin             = NRF24L01_CE_PIN;
    GPIO_InitStruct.Mode            = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull            = NRF24L01_CE_PULL;
    GPIO_InitStruct.Speed           = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = 0;
    HAL_GPIO_Init((GPIO_TypeDef*) NRF24L01_CE_PORT, &GPIO_InitStruct);
#if (NRF24L01_LOCK_PINS == 1)
    HAL_GPIO_LockPin((GPIO_TypeDef*) NRF24L01_CE_PORT, NRF24L01_CE_PIN);
#endif

    NRF24L01_IRQ_CLK_EN();

    GPIO_InitStruct.Pin             = NRF24L01_IRQ_PIN;
    GPIO_InitStruct.Mode            = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull            = NRF24L01_IRQ_PULL;
    GPIO_InitStruct.Speed           = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate       = 0;
    HAL_GPIO_Init((GPIO_TypeDef*) NRF24L01_IRQ_PORT, &GPIO_InitStruct);
#if (NRF24L01_LOCK_PINS == 1)
    HAL_GPIO_LockPin((GPIO_TypeDef*) NRF24L01_IRQ_PORT, NRF24L01_IRQ_PIN);
#endif

    /* Enable peripheral clock */
    NRF24L01_SPI_CLK_EN();

    /* Initialize HAL handle */
    nrfSpi.Instance                 = (SPI_TypeDef*) NRF24L01_SPI_INSTANCE;
    nrfSpi.Init.Mode                = SPI_MODE_MASTER;
    nrfSpi.Init.Direction           = SPI_DIRECTION_2LINES;
    nrfSpi.Init.DataSize            = SPI_DATASIZE_8BIT;
    nrfSpi.Init.CLKPolarity         = SPI_POLARITY_LOW;
    nrfSpi.Init.CLKPhase            = SPI_PHASE_1EDGE;
    nrfSpi.Init.NSS                 = SPI_NSS_SOFT;
    /* This value gives us 6.25 Mbps in case of STM32F746IGTx on 200 Mhz core clock */
    nrfSpi.Init.BaudRatePrescaler   = SPI_BAUDRATEPRESCALER_16;
    nrfSpi.Init.FirstBit            = SPI_FIRSTBIT_MSB;
    nrfSpi.Init.TIMode              = SPI_TIMODE_DISABLE;
    nrfSpi.Init.CRCCalculation      = SPI_CRCCALCULATION_DISABLE;
    nrfSpi.Init.CRCPolynomial       = 7;
    nrfSpi.Init.CRCLength           = SPI_CRC_LENGTH_DATASIZE;
    nrfSpi.Init.NSSPMode            = SPI_NSS_PULSE_DISABLE;

    if (HAL_SPI_Init(&nrfSpi) != HAL_OK) {
        NRF24L01_LOG("[nrf24l01] Failed to initialize HAL SPI handler...\r\n");
        return;
    }

    nrfCallback = callback;
    nrfContext = context;

    /* Flush RX FIFO */
    HAL_SPIEx_FlushRxFifo(&nrfSpi);
    HAL_NVIC_SetPriority(NRF24L01_IRQ_N, NRF24L01_IRQ_PRIO, 0);
    HAL_NVIC_EnableIRQ(NRF24L01_IRQ_N);
}

void nrf24l01_hal_stm32f7xx_spiTransfer(uint8_t tx, uint8_t *rx) {
    uint8_t _rx, *prx;

    /*
     * HAL expects valid pointer as input rx buffer arg. In case of user does not care about rx value, it sets arg as NULL.
     * Pointer pRx holds reference to rx buffer. If driver function is called with rx arg = NULL, set reference to local var.
     */
    if (rx == NULL) {
        prx = &_rx;
    } else {
        prx = rx;
    }

    if (HAL_SPI_TransmitReceive(&nrfSpi, &tx, prx, 1, 1000) != HAL_OK) {
        NRF24L01_LOG("[nrf24l01] SPI transmission has failed...\r\n");
        return;
    }
}

void nrf24l01_hal_stm32f7xx_powerControl(uint8_t state) {
    if (state) {
        HAL_GPIO_WritePin((GPIO_TypeDef*) NRF24L01_PWR_PORT, NRF24L01_PWR_PIN, GPIO_PIN_RESET);
        /* Wait for device voltage to stabilize */
        HAL_Delay(250);
    } else {
        HAL_GPIO_WritePin((GPIO_TypeDef*) NRF24L01_PWR_PORT, NRF24L01_PWR_PIN, GPIO_PIN_SET);
    }
}

void nrf24l01_hal_stm32f7xx_selectControl(uint8_t state) {
    HAL_GPIO_WritePin((GPIO_TypeDef*) NRF24L01_CSN_PORT, NRF24L01_CSN_PIN, (GPIO_PinState) state);
}

void nrf24l01_hal_stm32f7xx_clockControl(uint8_t state) {
    HAL_GPIO_WritePin((GPIO_TypeDef*) NRF24L01_CE_PORT, NRF24L01_CE_PIN, (GPIO_PinState) state);
}

uint8_t nrf24l01_hal_stm32f7xx_irqState(void) {
    return (uint8_t) (HAL_GPIO_ReadPin((GPIO_TypeDef*) NRF24L01_IRQ_PORT, NRF24L01_IRQ_PIN) == GPIO_PIN_RESET);
}

uint8_t nrf24l01_hal_stm32f7xx_irqSource(void) {
    return (uint8_t) (EXTI->SWIER & NRF24L01_IRQ_PIN);
}

void nrf24l01_hal_stm32f7xx_irqTrigger(void) {
    __HAL_GPIO_EXTI_GENERATE_SWIT(NRF24L01_IRQ_PIN);
}

void nrf24l01_hal_stm32f7xx_deinitialize(void) {
    HAL_NVIC_DisableIRQ(NRF24L01_IRQ_N);
    HAL_SPI_DeInit(&nrfSpi);

    NRF24L01_SPI_CLK_DIS();

#if (NRF24L01_LOCK_PINS == 0) /* Pins cannot be deinitialized if they are locked */
    HAL_GPIO_DeInit((GPIO_TypeDef*) NRF24L01_PWR_PORT, NRF24L01_PWR_PIN);
    HAL_GPIO_DeInit((GPIO_TypeDef*) NRF24L01_CSN_PORT, NRF24L01_CSN_PIN);
    HAL_GPIO_DeInit((GPIO_TypeDef*) NRF24L01_CE_PORT, NRF24L01_CE_PIN);
    HAL_GPIO_DeInit((GPIO_TypeDef*) NRF24L01_IRQ_PORT, NRF24L01_IRQ_PIN);

    HAL_GPIO_DeInit((GPIO_TypeDef*) NRF24L01_MISO_PORT, NRF24L01_MISO_PIN);
    HAL_GPIO_DeInit((GPIO_TypeDef*) NRF24L01_MOSI_PORT, NRF24L01_MOSI_PIN);
    HAL_GPIO_DeInit((GPIO_TypeDef*) NRF24L01_SCK_PORT, NRF24L01_SCK_PIN);
#endif
}

void NRF24L01_IRQ_HANDLER(void) {

    NRF24L01_TRACE_IRQ_ENTER();

    /*
     * HAL_GPIO_EXTI_IRQHandler call clears the pending interrupt pending flag.
     * This prevents us from software interrupt trigger detection.
     *
     * Execute callback first and then proceed with clearing irq flags using HAL API call.
     */
    if (nrfCallback != NULL) {
        nrfCallback(nrfContext);
    }

    HAL_GPIO_EXTI_IRQHandler(NRF24L01_IRQ_PIN);

    NRF24L01_TRACE_IRQ_EXIT();
}
