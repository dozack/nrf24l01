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

#ifndef NRF24L01_HAL_H_
#define NRF24L01_HAL_H_

#if defined __cplusplus
extern "C"{
#endif

#include "nrf24l01_def.h"

#define NRF24L01_LOG(...)

struct nrf24l01;

typedef void (*nrf24l01_callback_t)(void *context);

typedef struct {
    void        (*initialize    )(nrf24l01_callback_t callback, void *context);
    void        (*spiTransfer   )(uint8_t tx, uint8_t *rx);
    void        (*powerControl  )(uint8_t state);
    void        (*selectControl )(uint8_t state);
    void        (*clockControl  )(uint8_t state);
    uint8_t     (*irqState      )(void);
    void        (*irqTrigger    )(void);
    void        (*deinitialize  )(void);
} nrf24l01_hal_t;

extern void nrf24l01_hal_attach(struct nrf24l01 *nrf, nrf24l01_hal_t *hal);

extern int nrf24l01_hal_validate(struct nrf24l01 *nrf);

extern void nrf24l01_hal_detach(struct nrf24l01 *nrf);

#if defined __cplusplus
}
#endif

#endif /* NRF24L01_HAL_H_ */
