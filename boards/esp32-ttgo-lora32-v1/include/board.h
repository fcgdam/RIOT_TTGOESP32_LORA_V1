/*
 * Copyright (C) 2018 Gunar Schorcht
 *
 * Changed by FcGDAM (2018) for supporting the TTGO V1 ESP32 LORA OLED board
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     boards_esp32_TTGO_LORA_V1
 * @brief       Board specific definition for TTGO ESP32 LORA OLED V1 board
 * @{
 *
 * For detailed information about the configuration of ESP32 boards, see
 * section \ref esp32_comm_periph "Common Peripherals".
 *
 * @note
 * Most definitions can be overridden by an \ref esp32_app_spec_conf
 * "application-specific board configuration".
 *
 * @file
 * @author      Gunar Schorcht <gunar@schorcht.net>
 */

#ifndef BOARD_H
#define BOARD_H

/* The TTGO ESP32 LORA board uses 26MHz crystal instead of 40Mhz */
#define ESP32_XTAL_FREQ (26)

#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

#ifdef __cplusplus
} /* end extern "C" */
#endif

/**
 * @name    Button pin definitions
 * @{
 */
/**
  * Generic ESP32 boards have a BOOT button, which can be used as normal button
  * during normal operation. Since the GPIO0 pin is pulled up, the button
  * signal is inverted, i.e., pressing the button will give a low signal.
  */
#define BUTTON0_PIN         GPIO0
/** @} */

/**
 * @name    LED (on-board) configuration
 *
 * TTGO ESP32 board on-board LED.
 * @{
 */
/** @} */
#define LED0_PIN GPIO2
#define LED0_ACTIVE 1

/**
 * @name   OLED (on-board) reset pin configuration
 *
 * TTGO SSD1306 OLED Reset pin
 * @{
 */
/** @} */
#define OLED_RESET_PIN GPIO16

/**
 * @name   OLED (on-board) I2C address
 *
 * TTGO SSD1306 OLED I2C address
 * @{
 */
/** @} */
#define OLED_I2C_ADDR    0x3C

/**
 * @name    sx1276 (on-board) pins
 *
 * TTGO V1 sx1276 Lora transceiver pins
 * @{
 */
/** @} */
//#define SX127X_PARAM_SPI        (SPI_DEV(0))
#define SX127X_PARAM_SPI_NSS    GPIO_PIN(0,18)
#define SX127X_PARAM_SPI_RESET  GPIO_PIN(0,14)
#define SX127X_PARAM_DIO0       GPIO_PIN(0,26)
#define SX127X_PARAM_DIO1       GPIO_PIN(0,33)
#define SX127X_PARAM_DIO2       GPIO_PIN(0,32)
#define SX127X_PARAM_DIO3       GPIO_UNDEF
#define SX127X_PARAM_PASELECT   (SX127X_PA_RFO)

#define SX127X_PARAMS   { /*.spi       = SX127X_PARAM_SPI,*/     \
                          .nss_pin   = SX127X_PARAM_SPI_NSS, \
                          .reset_pin = SX127X_PARAM_RESET,   \
                          .dio0_pin  = SX127X_PARAM_DIO0,    \
                          .dio1_pin  = SX127X_PARAM_DIO1,    \
                          .dio2_pin  = SX127X_PARAM_DIO2,    \
                          .dio3_pin  = SX127X_PARAM_DIO3,    \
                          .paselect  = SX127X_PARAM_PASELECT \
                        }

/* include common board definitions as last step */
#include "board_common.h"

#endif /* BOARD_H */
/** @} */
