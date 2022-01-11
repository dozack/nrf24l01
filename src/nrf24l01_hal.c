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

#include "nrf24l01.h"

void nrf24l01_hal_attach(struct nrf24l01 *nrf, nrf24l01_hal_t *hal) {
    if ((nrf == NULL) || (hal == NULL)) {
        NRF24L01_LOG("[nrf24l01] Failed to attach HAL due to NULL argument... \r\n");
        return;
    }

    nrf->hal = hal;
}

int nrf24l01_hal_validate(struct nrf24l01 *nrf) {
    nrf24l01_hal_t *hal;
    int res;

    if (nrf == NULL) {
        NRF24L01_LOG("[nrf24l01] Failed to validate HAL due to NULL argument... \r\n");
        return -1;
    }

    hal = nrf->hal;
    res = 0;

    if ((hal->initialize) == NULL) {
        NRF24L01_LOG("[nrf24l01] HAL->initialize is missing... \r\n");
        res = -1;
    }
    if ((hal->spiTransfer) == NULL) {
        NRF24L01_LOG("[nrf24l01] HAL->spiTransfer is missing... \r\n");
        res = -1;
    }
    if ((hal->powerControl) == NULL) {
        NRF24L01_LOG("[nrf24l01] HAL->powerControl is missing... \r\n");
        res = -1;
    }
    if ((hal->selectControl) == NULL) {
        NRF24L01_LOG("[nrf24l01] HAL->selectControl is missing... \r\n");
        res = -1;
    }
    if ((hal->clockControl) == NULL) {
        NRF24L01_LOG("[nrf24l01] HAL->clockControl is missing... \r\n");
        res = -1;
    }
    if ((hal->irqState) == NULL) {
        NRF24L01_LOG("[nrf24l01] HAL->irqState is missing... \r\n");
        res = -1;
    }
    if ((hal->irqTrigger) == NULL) {
        NRF24L01_LOG("[nrf24l01] HAL->irqTrigger is missing... \r\n");
        res = -1;
    }
    if ((hal->deinitialize) == NULL) {
        NRF24L01_LOG("[nrf24l01] HAL->deinitialize is missing... \r\n");
        res = -1;
    }

    return res;
}

void nrf24l01_hal_detach(struct nrf24l01 *nrf) {
    if (nrf == NULL) {
        NRF24L01_LOG("[nrf24l01] Failed to detach HAL due to NULL argument... \r\n");
        return;
    }

    nrf->hal = NULL;
}
