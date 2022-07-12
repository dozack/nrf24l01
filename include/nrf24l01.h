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

#ifndef NRF24L01_H_
#define NRF24L01_H_

#if defined __cplusplus
extern "C"{
#endif

#include "nrf24l01_def.h"
#include "nrf24l01_hal.h"

typedef enum {
    NRF_UNINIT                  = 0,
    NRF_POWERDOWN               = 1,
    NRF_STANDBY                 = 2,
    NRF_TRANSMIT                = 3,
    NRF_RECEIVE                 = 4,
} nrf24l01_state_t;

typedef void (*nrf24l01_event_callback_t)(void *context);

typedef struct nrf24l01_config {
    uint8_t                     channel;
    uint64_t                    address;
    uint8_t                     retr_count;
    uint32_t                    retr_delay;
} nrf24l01_config_t;

typedef struct nrf24l01 {
    nrf24l01_hal_t             *hal;

    nrf24l01_state_t            state;
    nrf24l01_event_callback_t   callback;
    void *                      context;

    nrf24l01_config_t           config;
} nrf24l01_t;

extern void nrf24l01_initialize(nrf24l01_t *nrf);

extern void nrf24l01_notify(nrf24l01_t *nrf, nrf24l01_event_callback_t callback, void *context);

extern int nrf24l01_probe(nrf24l01_t *nrf);

extern int nrf24l01_configure(nrf24l01_t *nrf, nrf24l01_config_t *config);

extern int nrf24l01_open(nrf24l01_t *nrf);

extern void nrf24l01_trigger_irq(nrf24l01_t *nrf);

extern bool nrf24l01_irq_pending(nrf24l01_t *nrf);

extern void nrf24l01_flush_tx(nrf24l01_t *nrf);

extern void nrf24l01_flush_rx(nrf24l01_t *nrf);

extern void nrf24l01_standby(nrf24l01_t *nrf);

extern void nrf24l01_power_up(nrf24l01_t *nrf);

extern void nrf24l01_power_down(nrf24l01_t *nrf);

extern void nrf24l01_listen(nrf24l01_t *nrf);

extern void nrf24l01_stop_listening(nrf24l01_t *nrf);

extern uint8_t nrf24l01_read_status(nrf24l01_t *nrf);

extern uint8_t nrf24l01_clear_status(nrf24l01_t *nrf);

extern uint8_t nrf24l01_tx_pending(nrf24l01_t *nrf);

extern bool nrf24l01_channel_available(nrf24l01_t *nrf);

extern int nrf24l01_write(nrf24l01_t *nrf, uint8_t *data, uint8_t size);

extern int nrf24l01_write_ack(nrf24l01_t *nrf, uint8_t *data, uint8_t size);

extern uint8_t nrf24l01_rx_pending(nrf24l01_t *nrf);

extern int nrf24l01_read(nrf24l01_t *nrf, uint8_t *data, uint8_t *size);

extern void nrf24l01_close(nrf24l01_t *nrf);

extern void nrf24l01_deinitialize(nrf24l01_t *nrf);

#if defined __cplusplus
}
#endif

#endif /* NRF24L01_H_ */
