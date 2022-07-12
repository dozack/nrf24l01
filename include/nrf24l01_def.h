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

#ifndef NRF24L01_DEF_H_
#define NRF24L01_DEF_H_

#if defined __cplusplus
extern "C"{
#endif

#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdbool.h"

#define NRF24L01_MAX_PAYLOAD_SIZE                   (32)

#define NRF24L01_MAX_TX_FIFO_SIZE                   (3)
#define NRF24L01_MAX_RX_FIFO_SIZE                   (3)

#define NRF24L01_DEFAULT_ADDRESS                    (0x5555555555)
#define NRF24L01_DEFAULT_CHANNEL                    (80)
#define NRF24L01_DEFAULT_RETR_COUNT                 (0xf)
#define NRF24L01_DEFAULT_RETR_DELAY                 (4000)

/**
 * @brief   NRF24L01 register addresses
 */
#define NRF24L01_REG_CONFIG                         (0x00)
#define NRF24L01_REG_EN_AA                          (0x01)
#define NRF24L01_REG_EN_RXADDR                      (0x02)
#define NRF24L01_REG_SETUP_AW                       (0x03)
#define NRF24L01_REG_SETUP_RETR                     (0x04)
#define NRF24L01_REG_RF_CH                          (0x05)
#define NRF24L01_REG_RF_SETUP                       (0x06)
#define NRF24L01_REG_STATUS                         (0x07)
#define NRF24L01_REG_OBSERVE_TX                     (0x08)
#define NRF24L01_REG_RPD                            (0x09)              /* nRF24L01+    */
#define NRF24L01_REG_CD                             (NRF24L01_REG_RPD)  /* nRF24L01     */
#define NRF24L01_REG_RX_ADDR_P0                     (0x0a)
#define NRF24L01_REG_RX_ADDR_P1                     (0x0b)
#define NRF24L01_REG_RX_ADDR_P2                     (0x0c)
#define NRF24L01_REG_RX_ADDR_P3                     (0x0d)
#define NRF24L01_REG_RX_ADDR_P4                     (0x0e)
#define NRF24L01_REG_RX_ADDR_P5                     (0x0f)
#define NRF24L01_REG_TX_ADDR                        (0x10)
#define NRF24L01_REG_RX_PW_P0                       (0x11)
#define NRF24L01_REG_RX_PW_P1                       (0x12)
#define NRF24L01_REG_RX_PW_P2                       (0x13)
#define NRF24L01_REG_RX_PW_P3                       (0x14)
#define NRF24L01_REG_RX_PW_P4                       (0x15)
#define NRF24L01_REG_RX_PW_P5                       (0x16)
#define NRF24L01_REG_FIFO_STATUS                    (0x17)
#define NRF24L01_REG_DYNPD                          (0x1c)
#define NRF24L01_REG_FEATURE                        (0x1d)

/**
 * @brief   REG_CONFIG masks
 */
#define NRF24L01_CONFIG_PRIM_RX                     (1 << 0)
#define NRF24L01_CONFIG_PWR_UP                      (1 << 1)
#define NRF24L01_CONFIG_CRCO                        (1 << 2)
#define NRF24L01_CONFIG_EN_CRC                      (1 << 3)
#define NRF24L01_CONFIG_MASK_MAX_RT                 (1 << 4)
#define NRF24L01_CONFIG_MASK_TX_DS                  (1 << 5)
#define NRF24L01_CONFIG_MASK_RX_DR                  (1 << 6)

/**
 * @brief   REG_EN_AA masks
 */
#define NRF24L01_EN_AA_ENAA_P0                      (1 << 0)
#define NRF24L01_EN_AA_ENAA_P1                      (1 << 1)
#define NRF24L01_EN_AA_ENAA_P2                      (1 << 2)
#define NRF24L01_EN_AA_ENAA_P3                      (1 << 3)
#define NRF24L01_EN_AA_ENAA_P4                      (1 << 4)
#define NRF24L01_EN_AA_ENAA_P5                      (1 << 5)

/**
 * @brief   REG_EN_RXADDR masks
 */
#define NRF24L01_EN_RXADDR_ERX_P0                   (1 << 0)
#define NRF24L01_EN_RXADDR_ERX_P1                   (1 << 1)
#define NRF24L01_EN_RXADDR_ERX_P2                   (1 << 2)
#define NRF24L01_EN_RXADDR_ERX_P3                   (1 << 3)
#define NRF24L01_EN_RXADDR_ERX_P4                   (1 << 4)
#define NRF24L01_EN_RXADDR_ERX_P5                   (1 << 5)

/*
 * @brief   REG_RF_SETUP masks
 */
#define NRF24L01_RF_SETUP_LNA_HCURR                 (1 << 0)
#define NRF24L01_RF_SETUP_RF_PWR                    (3 << 1)
#define NRF24L01_RF_SETUP_RF_DR                     (1 << 3)
#define NRF24L01_RF_SETUP_PLL_LOCK                  (1 << 4)

/**
 * @brief   REG_STATUS masks
 */
#define NRF24L01_STATUS_TX_FULL                     (1 << 0)
#define NRF24L01_STATUS_RX_P_NO                     (7 << 1)
#define NRF24L01_STATUS_MAX_RT                      (1 << 4)
#define NRF24L01_STATUS_TX_DS                       (1 << 5)
#define NRF24L01_STATUS_RX_DR                       (1 << 6)

/**
 * @brief   REG_RPD masks
 */
#define NRF24L01_RPD_RPD                            (1 << 0)

/**
 * @brief   Mask of all interrupt sources at once
 */
#define NRF24L01_STATUS_IRQ_MASK                    (NRF24L01_STATUS_MAX_RT | NRF24L01_STATUS_TX_DS | NRF24L01_STATUS_RX_DR)

/**
 * @brief   REG_FIFO_STATUS masks
 */
#define NRF24L01_FIFO_STATUS_RX_EMPTY               (1 << 0)
#define NRF24L01_FIFO_STATUS_RX_FULL                (1 << 1)
#define NRF24L01_FIFO_STATUS_TX_EMPTY               (1 << 4)
#define NRF24L01_FIFO_STATUS_TX_FULL                (1 << 5)
#define NRF24L01_FIFO_STATUS_TX_REUSE               (1 << 6)

/**
 * @brief   REG_EN_DYNPD masks
 */
#define NRF24L01_DYNPD_DPL_P0                       (1 << 0)
#define NRF24L01_DYNPD_DPL_P1                       (1 << 1)
#define NRF24L01_DYNPD_DPL_P2                       (1 << 2)
#define NRF24L01_DYNPD_DPL_P3                       (1 << 3)
#define NRF24L01_DYNPD_DPL_P4                       (1 << 4)
#define NRF24L01_DYNPD_DPL_P5                       (1 << 5)

/**
 * @brief   REG_EN_FEATURE masks
 */
#define NRF24L01_FEATURE_DYN_ACK                    (1 << 0)
#define NRF24L01_FEATURE_EN_ACK_PAY                 (1 << 1)
#define NRF24L01_FEATURE_EN_DPL                     (1 << 2)

/**
 * @brief   Register operation command mask
 */
#define NRF24L01_REGISTER_MASK                      (0x1f)

/**
 * @brief   Device SPI commands
 */
#define NRF24L01_CMD_READ_REGISTER                  (0x00)
#define NRF24L01_CMD_WRITE_REGISTER                 (0x20)
#define NRF24L01_CMD_ACTIVATE                       (0x50)
#define NRF24L01_CMD_READ_RX_PL_WID                 (0x60)
#define NRF24L01_CMD_READ_RX_PAYLOAD                (0x61)
#define NRF24L01_CMD_WRITE_TX_PAYLOAD               (0xa0)
#define NRF24L01_CMD_WRITE_ACK_PAYLOAD              (0xa8)
#define NRF24L01_CMD_WRITE_TX_PAYLOAD_NO_ACK        (0xb0)
#define NRF24L01_CMD_FLUSH_TX                       (0xe1)
#define NRF24L01_CMD_FLUSH_RX                       (0xe2)
#define NRF24L01_CMD_REUSE_TX_PL                    (0xe3)
#define NRF24L01_CMD_NOP                            (0xff)

#if defined __cplusplus
}
#endif

#endif /* NRF24L01_COMMON_H_ */
