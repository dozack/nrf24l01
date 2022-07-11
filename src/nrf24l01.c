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

static void nrf24l01_irq_handler(void *context);

static void nrf24l01_spi_write(nrf24l01_t *nrf, uint8_t address, uint8_t *data, uint8_t size);

static void nrf24l01_spi_read(nrf24l01_t *nrf, uint8_t address, uint8_t *data, uint8_t size);

static void nrf24l01_command(nrf24l01_t *nrf, uint8_t command);

static void nrf24l01_register_write(nrf24l01_t *nrf, uint8_t address, uint8_t value);

static uint8_t nrf24l01_register_read(nrf24l01_t *nrf, uint8_t address);

static uint8_t nrf24l01_read_payload_size(nrf24l01_t *nrf);

void nrf24l01_initialize(nrf24l01_t *nrf) {
    nrf24l01_hal_t *hal;

    if (nrf == NULL) {
        NRF24L01_LOG("[nrf24l01] Failed to initialize handle due to NULL argument... \r\n");
    }

    if (nrf24l01_hal_validate(nrf) < 0) {
        NRF24L01_LOG("[nrf24l01] HAL validation failed, is HAL attached?... \r\n");
    }

    nrf->state = NRF_UNINIT;

    hal = nrf->hal;
    hal->initialize();
    hal->powerControl(1);
    hal->clockControl(0);
    hal->selectControl(1);

    nrf->callback = NULL;
    nrf->context = NULL;

    nrf->config.address = NRF24L01_DEFAULT_ADDRESS;
    nrf->config.channel = NRF24L01_DEFAULT_CHANNEL;
    nrf->config.retr_count = NRF24L01_DEFAULT_RETR_COUNT;
    nrf->config.retr_delay = NRF24L01_DEFAULT_RETR_DELAY;

    nrf->state = NRF_POWERDOWN;
}

void nrf24l01_notify(nrf24l01_t *nrf, nrf24l01_event_callback_t callback, void *context) {
    if (nrf == NULL) {
        NRF24L01_LOG("[nrf24l01] Failed to attach notification due to NULL argument... \r\n");
    }

    nrf->callback = callback;
    nrf->context = context;
}

int nrf24l01_probe(nrf24l01_t *nrf) {
    uint8_t value;

    value = nrf24l01_register_read(nrf, NRF24L01_REG_SETUP_AW);

    if ((value < 1) || (value > 3)) {
        return (-1);
    }

    return (0);
}

int nrf24l01_configure(nrf24l01_t *nrf, nrf24l01_config_t *config) {
    if ((nrf == NULL) || (config) == NULL) {
        return (-1);
    }
    if (config->channel > 125) {
        return (-1);
    }
    if (config->retr_count > 0xf) {
        /*
         * TODO: return invalid config instead of contents manipulation?
         */
        config->retr_count = 0xf;
    }
    if (config->retr_delay > 4000) {
        config->retr_delay = 4000;
    }

    nrf->config.address = config->address;
    nrf->config.channel = config->channel;
    nrf->config.retr_count = config->retr_count;
    nrf->config.retr_delay = config->retr_delay;

    return (0);
}

int nrf24l01_open(nrf24l01_t *nrf) {
    uint8_t value;

    if (nrf == NULL) {
        NRF24L01_LOG("[nrf24l01] Failed to connect due to NULL argument... \r\n");
        return -1;
    }

    if (nrf->state != NRF_POWERDOWN) {
        NRF24L01_LOG("[nrf24l01] Device is not initialized or is in use... \r\n");
        return -1;
    }

    value = (NRF24L01_CONFIG_EN_CRC | NRF24L01_CONFIG_CRCO);
    nrf24l01_register_write(nrf, NRF24L01_REG_CONFIG, value);

    value = nrf->config.channel;
    nrf24l01_register_write(nrf, NRF24L01_REG_RF_CH, value);

    value = ((nrf->config.retr_delay / 250) << 4) | (nrf->config.retr_count);
    nrf24l01_register_write(nrf, NRF24L01_REG_SETUP_RETR, value);

    value = (NRF24L01_RF_SETUP_RF_PWR | NRF24L01_RF_SETUP_RF_DR | NRF24L01_RF_SETUP_LNA_HCURR);
    nrf24l01_register_write(nrf, NRF24L01_REG_RF_SETUP, value);

    value = 0x73;
    nrf24l01_spi_write(nrf, NRF24L01_CMD_ACTIVATE, &value, 1);

    value = (NRF24L01_FEATURE_EN_DPL | NRF24L01_FEATURE_EN_ACK_PAY);
    nrf24l01_register_write(nrf, NRF24L01_REG_FEATURE, value);

    value = NRF24L01_EN_RXADDR_ERX_P0;
    nrf24l01_register_write(nrf, NRF24L01_REG_EN_RXADDR, value);

    value = NRF24L01_EN_AA_ENAA_P0;
    nrf24l01_register_write(nrf, NRF24L01_REG_EN_AA, value);

    value = NRF24L01_DYNPD_DPL_P0;
    nrf24l01_register_write(nrf, NRF24L01_REG_DYNPD, value);

    value = 3;
    nrf24l01_register_write(nrf, NRF24L01_REG_SETUP_AW, value);

    value = (NRF24L01_CMD_WRITE_REGISTER | NRF24L01_REG_RX_ADDR_P0);
    nrf24l01_spi_write(nrf, value, (uint8_t*) &nrf->config.address, 5);

    value = (NRF24L01_CMD_WRITE_REGISTER | NRF24L01_REG_TX_ADDR);
    nrf24l01_spi_write(nrf, value, (uint8_t*) &nrf->config.address, 5);

    nrf24l01_flush_tx(nrf);
    nrf24l01_flush_rx(nrf);

    nrf24l01_clear_status(nrf);
    nrf24l01_power_up(nrf);

    nrf->hal->attachIrq(&nrf24l01_irq_handler, nrf);

    NRF24L01_LOG("[nrf24l01] Channel open... \r\n");

    return 0;
}

void nrf24l01_trigger_irq(nrf24l01_t *nrf) {
    nrf->hal->irqTrigger();
}

bool nrf24l01_irq_pending(nrf24l01_t *nrf) {
    return (bool) nrf->hal->irqState();
}

void nrf24l01_flush_tx(nrf24l01_t *nrf) {
    nrf24l01_command(nrf, NRF24L01_CMD_FLUSH_TX);
}

void nrf24l01_flush_rx(nrf24l01_t *nrf) {
    nrf24l01_command(nrf, NRF24L01_CMD_FLUSH_RX);
}

void nrf24l01_standby(nrf24l01_t *nrf) {
    nrf->hal->clockControl(0);

    nrf->state = NRF_STANDBY;
}

void nrf24l01_power_up(nrf24l01_t *nrf) {
    uint8_t value;

    value = nrf24l01_register_read(nrf, NRF24L01_REG_CONFIG);
    value |= NRF24L01_CONFIG_PWR_UP;
    nrf24l01_register_write(nrf, NRF24L01_REG_CONFIG, value);

    nrf->state = NRF_STANDBY;
}

void nrf24l01_power_down(nrf24l01_t *nrf) {
    uint8_t value;

    nrf24l01_standby(nrf);
    nrf24l01_flush_tx(nrf);
    nrf24l01_flush_rx(nrf);

    value = nrf24l01_register_read(nrf, NRF24L01_REG_CONFIG);
    value &= ~NRF24L01_CONFIG_PWR_UP;
    nrf24l01_register_write(nrf, NRF24L01_REG_CONFIG, value);

    nrf->state = NRF_POWERDOWN;
}

void nrf24l01_listen(nrf24l01_t *nrf) {
    uint8_t value;

    value = nrf24l01_register_read(nrf, NRF24L01_REG_CONFIG);
    value |= NRF24L01_CONFIG_PRIM_RX;
    nrf24l01_register_write(nrf, NRF24L01_REG_CONFIG, value);

    nrf24l01_clear_status(nrf);

    nrf->state = NRF_RECEIVE;
    nrf->hal->clockControl(1);
}

void nrf24l01_stop_listening(nrf24l01_t *nrf) {
    uint8_t value;

    nrf24l01_standby(nrf);

    value = nrf24l01_register_read(nrf, NRF24L01_REG_CONFIG);
    value &= ~NRF24L01_CONFIG_PRIM_RX;
    nrf24l01_register_write(nrf, NRF24L01_REG_CONFIG, value);
}

uint8_t nrf24l01_read_status(nrf24l01_t *nrf) {
    uint8_t value;

    value = nrf24l01_register_read(nrf, NRF24L01_REG_STATUS);

    return (value & NRF24L01_STATUS_IRQ_MASK);
}

uint8_t nrf24l01_clear_status(nrf24l01_t *nrf) {
    uint8_t value;

    value = nrf24l01_register_read(nrf, NRF24L01_REG_STATUS);
    nrf24l01_register_write(nrf, NRF24L01_REG_STATUS, (value | NRF24L01_STATUS_IRQ_MASK));

    return (value & NRF24L01_STATUS_IRQ_MASK);
}

bool nrf24l01_tx_pending(nrf24l01_t *nrf) {
    uint8_t value;

    value = nrf24l01_register_read(nrf, NRF24L01_REG_FIFO_STATUS);

    return (value & NRF24L01_FIFO_STATUS_TX_EMPTY) ? false : true;
}

bool nrf24l01_channel_available(nrf24l01_t *nrf) {
    uint8_t value;

    value = nrf24l01_register_read(nrf, NRF24L01_REG_RPD);

    return (bool) !(value & NRF24L01_RPD_RPD);
}

int nrf24l01_write(nrf24l01_t *nrf, uint8_t *data, uint8_t size) {
    uint8_t value;

    if ((nrf == NULL) || (data == NULL)) {
        NRF24L01_LOG("[nrf24l01] Failed to write due to NULL argument... \r\n");
        return -1;
    }
    if ((size < 1) || (size > NRF24L01_MAX_PAYLOAD_SIZE)) {
        NRF24L01_LOG("[nrf24l01] Failed to write due to invalid size... \r\n");
        return -1;
    }
    if (nrf->state < NRF_STANDBY) {
        NRF24L01_LOG("[nrf24l01] Failed to write due to invalid device state... \r\n");
        return -1;
    }

    value = nrf24l01_register_read(nrf, NRF24L01_REG_FIFO_STATUS);

    if (value & NRF24L01_FIFO_STATUS_TX_FULL) {
        NRF24L01_LOG("[nrf24l01] Failed to write due to full transmit FIFO... \r\n");
        return -1;
    }

    if (nrf->state == NRF_RECEIVE) {
        nrf24l01_stop_listening(nrf);
    }

    nrf24l01_spi_write(nrf, NRF24L01_CMD_WRITE_TX_PAYLOAD, data, size);

    nrf->state = NRF_TRANSMIT;
    nrf->hal->clockControl(1);
    return 0;
}

int nrf24l01_write_ack(nrf24l01_t *nrf, uint8_t *data, uint8_t size) {
    return -1;
}

bool nrf24l01_rx_pending(nrf24l01_t *nrf) {
    uint8_t value;

    value = nrf24l01_register_read(nrf, NRF24L01_REG_FIFO_STATUS);

    return (value & NRF24L01_FIFO_STATUS_RX_EMPTY) ? false : true;
}

int nrf24l01_read(nrf24l01_t *nrf, uint8_t *data, uint8_t *size) {
    uint8_t value;

    if (nrf24l01_rx_pending(nrf)) {
        value = nrf24l01_read_payload_size(nrf);

        nrf24l01_spi_read(nrf, NRF24L01_CMD_READ_RX_PAYLOAD, data, value);

        if (size != NULL) {
            *size = value;
        }
        return 0;
    }
    return -1;
}

void nrf24l01_close(nrf24l01_t *nrf) {

    nrf24l01_power_down(nrf);
    /* Clear irq handler to disable interrupt */
    nrf->hal->attachIrq(NULL, NULL);
}

void nrf24l01_deinitialize(nrf24l01_t *nrf) {

    nrf24l01_close(nrf);

    nrf->hal->powerControl(0);
    nrf->hal->deinitialize();

    nrf->state = NRF_UNINIT;

}

static void nrf24l01_irq_handler(void *context) {
    nrf24l01_t *nrf;

    nrf = (nrf24l01_t*) context;

    /*
     * In case callback is attached, do not directly access
     * the device as it may interrupt pending SPI transmission
     * in thread context. Instead execute callback and let user
     * decide how to handle the event that caused the interrupt.
     */

    if (nrf->callback != NULL) {
        /* Callback attached, execute */
        nrf->callback(nrf->context);
    }
}

static void nrf24l01_spi_write(nrf24l01_t *nrf, uint8_t address, uint8_t *data, uint8_t size) {
    nrf24l01_hal_t *hal;

    hal = nrf->hal;

    hal->selectControl(0);
    hal->spiTransfer(address, NULL);
    while (size--) {
        hal->spiTransfer(*data++, NULL);
    }
    hal->selectControl(1);
}

static void nrf24l01_spi_read(nrf24l01_t *nrf, uint8_t address, uint8_t *data, uint8_t size) {
    nrf24l01_hal_t *hal;

    hal = nrf->hal;

    hal->selectControl(0);
    hal->spiTransfer(address, NULL);
    while (size--) {
        hal->spiTransfer(NRF24L01_CMD_NOP, data++);
    }
    hal->selectControl(1);
}

static void nrf24l01_command(nrf24l01_t *nrf, uint8_t command) {
    nrf24l01_spi_write(nrf, command, NULL, 0);
}

static void nrf24l01_register_write(nrf24l01_t *nrf, uint8_t address, uint8_t value) {
    address &= NRF24L01_REGISTER_MASK;
    address |= NRF24L01_CMD_WRITE_REGISTER;

    nrf24l01_spi_write(nrf, address, &value, 1);
}

static uint8_t nrf24l01_register_read(nrf24l01_t *nrf, uint8_t address) {
    uint8_t value;

    address &= NRF24L01_REGISTER_MASK;
    address |= NRF24L01_CMD_READ_REGISTER;

    nrf24l01_spi_read(nrf, address, &value, 1);
    return value;
}

static uint8_t nrf24l01_read_payload_size(nrf24l01_t *nrf) {
    uint8_t size;

    nrf24l01_spi_read(nrf, NRF24L01_CMD_READ_RX_PL_WID, &size, 1);
    if (size > NRF24L01_MAX_PAYLOAD_SIZE) {
        nrf24l01_flush_rx(nrf);
        size = 0;
    }
    return size;
}
