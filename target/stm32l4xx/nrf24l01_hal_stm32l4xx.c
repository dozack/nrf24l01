/* 
 * MIT License
 * -----------
 * 
 * Copyright (c) 2021 Technical university of Liberec (https://tul.cz)
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

#include "nrf24l01_hal_stm32l4xx.h"

struct stm32l4xx_driver {
    SPI_HandleTypeDef hspi;
    nrf24l01_callback_t callback;
    void *context;
};

static struct stm32l4xx_driver _stm32l4xx;

nrf24l01_hal_t nrf24l01_hal_stm32l4xx = {
    &nrf24l01_hal_stm32l4xx_initialize,
    &nrf24l01_hal_stm32l4xx_spiTransfer,
    &nrf24l01_hal_stm32l4xx_attachIrq,
    &nrf24l01_hal_stm32l4xx_powerControl,
    &nrf24l01_hal_stm32l4xx_selectControl,
    &nrf24l01_hal_stm32l4xx_clockControl,
    &nrf24l01_hal_stm32l4xx_irqState,
    &nrf24l01_hal_stm32l4xx_irqSource,
    &nrf24l01_hal_stm32l4xx_irqTrigger,
    &nrf24l01_hal_stm32l4xx_deinitialize, };

void nrf24l01_hal_stm32l4xx_initialize(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

#if 0 /* Power management not currently available */

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

#endif

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
    _stm32l4xx.hspi.Instance                 = (SPI_TypeDef*) NRF24L01_SPI_INSTANCE;
    _stm32l4xx.hspi.Init.Mode                = SPI_MODE_MASTER;
    _stm32l4xx.hspi.Init.Direction           = SPI_DIRECTION_2LINES;
    _stm32l4xx.hspi.Init.DataSize            = SPI_DATASIZE_8BIT;
    _stm32l4xx.hspi.Init.CLKPolarity         = SPI_POLARITY_LOW;
    _stm32l4xx.hspi.Init.CLKPhase            = SPI_PHASE_1EDGE;
    _stm32l4xx.hspi.Init.NSS                 = SPI_NSS_SOFT;
    /* This value gives us 6.25 Mbps in case of STM32F746IGTx on 200 Mhz core clock */
    _stm32l4xx.hspi.Init.BaudRatePrescaler   = SPI_BAUDRATEPRESCALER_16;
    _stm32l4xx.hspi.Init.FirstBit            = SPI_FIRSTBIT_MSB;
    _stm32l4xx.hspi.Init.TIMode              = SPI_TIMODE_DISABLE;
    _stm32l4xx.hspi.Init.CRCCalculation      = SPI_CRCCALCULATION_DISABLE;
    _stm32l4xx.hspi.Init.CRCPolynomial       = 7;
    _stm32l4xx.hspi.Init.CRCLength           = SPI_CRC_LENGTH_DATASIZE;
    _stm32l4xx.hspi.Init.NSSPMode            = SPI_NSS_PULSE_DISABLE;

    if (HAL_SPI_Init(&_stm32l4xx.hspi) != HAL_OK) {
        NRF24L01_LOG("[nrf24l01] Failed to initialize HAL SPI handler...\r\n");
        return;
    }

    /* Flush RX FIFO */
    HAL_SPIEx_FlushRxFifo(&_stm32l4xx.hspi);
}

void nrf24l01_hal_stm32l4xx_attachIrq(nrf24l01_callback_t callback, void *context) {

    nrfCallback = callback;
    nrfContext = context;

    if (callback == NULL) {
        HAL_NVIC_DisableIRQ(NRF24L01_IRQ_N);
    } else {
        HAL_NVIC_SetPriority(NRF24L01_IRQ_N, NRF24L01_IRQ_PRIO, 0);
        HAL_NVIC_EnableIRQ(NRF24L01_IRQ_N);
    }
}

void nrf24l01_hal_stm32l4xx_spiTransfer(uint8_t tx, uint8_t *rx) {
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

    if (HAL_SPI_TransmitReceive(&_stm32l4xx.hspi, &tx, prx, 1, 1000) != HAL_OK) {
        NRF24L01_LOG("[nrf24l01] SPI transmission has failed...\r\n");
        return;
    }
}

void nrf24l01_hal_stm32l4xx_powerControl(uint8_t state) {

#if 0 /* Power management not currently available */
    if (state) {
        HAL_GPIO_WritePin((GPIO_TypeDef*) NRF24L01_PWR_PORT, NRF24L01_PWR_PIN, GPIO_PIN_RESET);
        /* Wait for device voltage to stabilize */
        HAL_Delay(250);
    } else {
        HAL_GPIO_WritePin((GPIO_TypeDef*) NRF24L01_PWR_PORT, NRF24L01_PWR_PIN, GPIO_PIN_SET);
    }
#else
    (void) 0;
#endif
}

void nrf24l01_hal_stm32l4xx_selectControl(uint8_t state) {
    HAL_GPIO_WritePin((GPIO_TypeDef*) NRF24L01_CSN_PORT, NRF24L01_CSN_PIN, (GPIO_PinState) state);
}

void nrf24l01_hal_stm32l4xx_clockControl(uint8_t state) {
    HAL_GPIO_WritePin((GPIO_TypeDef*) NRF24L01_CE_PORT, NRF24L01_CE_PIN, (GPIO_PinState) state);
}

uint8_t nrf24l01_hal_stm32l4xx_irqState(void) {
    return (uint8_t) (HAL_GPIO_ReadPin((GPIO_TypeDef*) NRF24L01_IRQ_PORT, NRF24L01_IRQ_PIN) == GPIO_PIN_RESET);
}

uint8_t nrf24l01_hal_stm32l4xx_irqSource(void) {
    return (uint8_t) (EXTI->SWIER & NRF24L01_IRQ_PIN);
}

void nrf24l01_hal_stm32l4xx_irqTrigger(void) {
    __HAL_GPIO_EXTI_GENERATE_SWIT(NRF24L01_IRQ_PIN);
}

void nrf24l01_hal_stm32l4xx_deinitialize(void) {
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
    if (_stm32l4xx.callback != NULL) {
        _stm32l4xx.callback(_stm32l4xx.context);
    }

    HAL_GPIO_EXTI_IRQHandler(NRF24L01_IRQ_PIN);

    NRF24L01_TRACE_IRQ_EXIT();
}




