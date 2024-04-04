/**
 * @file i2s.h
 * @author Jordi Gauchía
 * @brief I2S Routines
 * @version 0.3.1
 * @date 2024-04
 */

#ifndef I2S_H
#define I2S_H

#include "driver/i2s.h"
#include <hal.h>

/**
 * @brief I2S DAC Port Configuration
 *
 */
i2s_config_t i2sConfigDAC = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_24BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 2,
    .dma_buf_len = 1024,
    .use_apll = true,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 44100 * 384};

/**
 * @brief I2S DAC Pin Configuration
 *
 */
i2s_pin_config_t i2sPinConfigDAC = {.bck_io_num = I2S_BCK, .ws_io_num = I2S_LRC, .data_out_num = I2S_DOUT, .data_in_num = I2S_PIN_NO_CHANGE};

/**
 * @brief Start DAC
 *
 * @param sampleRate
 * @param numChannels
 * @param bitDepth
 */
void startDAC(int sampleRate, int numChannels, int bitDepth)
{
    i2sConfigDAC.sample_rate = sampleRate;
    i2sConfigDAC.fixed_mclk = sampleRate * 384;
    i2sConfigDAC.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
    if (numChannels == 1)
       i2sConfigDAC.channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT;
    i2s_driver_install(I2S_NUM_0, &i2sConfigDAC, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &i2sPinConfigDAC);
    i2s_set_sample_rates(I2S_NUM_0, sampleRate);
    i2s_set_clk(I2S_NUM_0, sampleRate, I2S_BITS_PER_SAMPLE_16BIT, (i2s_channel_t)numChannels);
    i2s_start(I2S_NUM_0);
    // PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0_CLK_OUT1);
    // WRITE_PERI_REG(PIN_CTRL, 0xFFF0);
}

/**
 * @brief Stop DAC
 *
 */
void stopDAC()
{
    i2s_stop(I2S_NUM_0);
    i2s_driver_uninstall(I2S_NUM_0);
}

/**
 * @brief I2S ADC Port Configuration
 *
 */
i2s_config_t i2sConfigADC = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_24BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 1024,
    .use_apll = true,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 44100 * 384};

/**
 * @brief I2S ADC Pin Configuration
 *
 */
i2s_pin_config_t i2sPinConfigADC = {.bck_io_num = I2S_BCK, .ws_io_num = I2S_LRC, .data_out_num = I2S_PIN_NO_CHANGE, .data_in_num = I2S_DIN};

/**
 * @brief Start ADC
 *
 * @param sampleRate
 * @param numChannels
 * @param bitDepth
 */
void startADC(int sampleRate, int numChannels, int bitDepth)
{
    i2sConfigADC.sample_rate = sampleRate;
    i2sConfigADC.fixed_mclk = sampleRate * 384;
    if (numChannels == 1)
        i2sConfigADC.channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT;
    i2s_driver_install(I2S_NUM_0, &i2sConfigADC, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &i2sPinConfigADC);
    // i2s_set_sample_rates(I2S_NUM_0, sampleRate);
}

/**
 * @brief Stop ADC
 *
 */
void stopADC()
{
    i2s_driver_uninstall(I2S_NUM_0);
}

#endif