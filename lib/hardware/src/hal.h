/**
 * @file hal.h
 * @author Jordi Gauchía
 * @brief  Pin definitions
 * @version 0.3.1
 * @date 2024-04
 */

#ifndef HAL_H
#define HAL_H

/**
 * @brief SD pin definition
 *
 */
const uint8_t SD_CS = 4;
const uint8_t SD_MISO = 19;
const uint8_t SD_MOSI = 23;
const uint8_t SD_CLK = 12;

/**
 * @brief I2S pin definition
 *
 */
const uint8_t I2S_BCK = 26;
const uint8_t I2S_LRC = 33;
const uint8_t I2S_DIN = 35;
const uint8_t I2S_DOUT = 25;
const uint8_t I2S_MCK = 0;

#endif