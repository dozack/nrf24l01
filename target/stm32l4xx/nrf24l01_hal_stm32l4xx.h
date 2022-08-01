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

#ifndef NRF24L01_HAL_STM32L4XX_H_
#define NRF24L01_HAL_STM32L4XX_H_

#if defined __cplusplus
extern "C"{
#endif

#include "nrf24l01_hal.h"
#include "stm32l4xx.h"

#define NRF24L01_TRACE_INC              "FreeRTOS.h"
#define NRF24L01_TRACE_IRQ_ENTER()      traceISR_ENTER()
#define NRF24L01_TRACE_IRQ_EXIT()       traceISR_EXIT()

#undef NRF24L01_LOG
#define NRF24L01_LOG(...)               printf(__VA_ARGS__)


#define NRF24L01_LOCK_PINS              (1)

#define NRF24L01_SPI_INSTANCE           (SPI2_BASE)
#define NRF24L01_SPI_CLK_EN()           do { __HAL_RCC_SPI2_CLK_ENABLE();  } while(0)
#define NRF24L01_SPI_CLK_DIS()          do { __HAL_RCC_SPI2_CLK_DISABLE(); } while(0)

#define NRF24L01_MISO_CLK_EN()          do { __HAL_RCC_GPIOB_CLK_ENABLE(); } while(0)
#define NRF24L01_MISO_PORT              (GPIOB_BASE)
#define NRF24L01_MISO_PIN               (GPIO_PIN_14)
#define NRF24L01_MISO_AF                (GPIO_AF5_SPI2)

#define NRF24L01_MOSI_CLK_EN()          do { __HAL_RCC_GPIOB_CLK_ENABLE(); } while(0)
#define NRF24L01_MOSI_PORT              (GPIOB_BASE)
#define NRF24L01_MOSI_PIN               (GPIO_PIN_15)
#define NRF24L01_MOSI_AF                (GPIO_AF5_SPI2)

#define NRF24L01_SCK_CLK_EN()           do { __HAL_RCC_GPIOB_CLK_ENABLE(); } while(0)
#define NRF24L01_SCK_PORT               (GPIOB_BASE)
#define NRF24L01_SCK_PIN                (GPIO_PIN_13)
#define NRF24L01_SCK_AF                 (GPIO_AF5_SPI2)

#define NRF24L01_CSN_CLK_EN()           do { __HAL_RCC_GPIOB_CLK_ENABLE(); } while(0)
#define NRF24L01_CSN_PORT               (GPIOB_BASE)
#define NRF24L01_CSN_PIN                (GPIO_PIN_12)
#define NRF24L01_CSN_PULL               (GPIO_NOPULL)

#define NRF24L01_PWR_CLK_EN()           do { __HAL_RCC_GPIOC_CLK_ENABLE(); } while(0)
#define NRF24L01_PWR_PORT               (GPIOC_BASE)
#define NRF24L01_PWR_PIN                (GPIO_PIN_12)
#define NRF24L01_PWR_PULL               (GPIO_NOPULL)

#define NRF24L01_CE_CLK_EN()            do { __HAL_RCC_GPIOB_CLK_ENABLE(); } while(0)
#define NRF24L01_CE_PORT                (GPIOB_BASE)
#define NRF24L01_CE_PIN                 (GPIO_PIN_6)
#define NRF24L01_CE_PULL                (GPIO_NOPULL)

#define NRF24L01_IRQ_CLK_EN()           do { __HAL_RCC_GPIOA_CLK_ENABLE(); } while(0)
#define NRF24L01_IRQ_PORT               (GPIOA_BASE)
#define NRF24L01_IRQ_PIN                (GPIO_PIN_15)
#define NRF24L01_IRQ_PULL               (GPIO_NOPULL)
#define NRF24L01_IRQ_N                  (EXTI15_10_IRQn)
#define NRF24L01_IRQ_HANDLER            (EXTI15_10_IRQHandler)
#define NRF24L01_IRQ_PRIO               (5)

extern void nrf24l01_hal_stm32l4xx_initialize(void);

extern void nrf24l01_hal_stm32l4xx_attachIrq(nrf24l01_callback_t callback, void *context);

extern void nrf24l01_hal_stm32l4xx_spiTransfer(uint8_t tx, uint8_t *rx);

extern void nrf24l01_hal_stm32l4xx_powerControl(uint8_t state);

extern void nrf24l01_hal_stm32l4xx_selectControl(uint8_t state);

extern void nrf24l01_hal_stm32l4xx_clockControl(uint8_t state);

extern uint8_t nrf24l01_hal_stm32l4xx_irqState(void);

extern uint8_t nrf24l01_hal_stm32l4xx_irqSource(void);

extern void nrf24l01_hal_stm32l4xx_irqTrigger(void);

extern void nrf24l01_hal_stm32l4xx_deinitialize(void);

extern nrf24l01_hal_t nrf24l01_hal_stm32l4xx;

#if defined __cplusplus
}
#endif

#endif /* NRF24L01_HAL_STM32L4XX_H_ */
