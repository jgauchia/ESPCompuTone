/**
 * @file i2s.h
 * @author Jordi Gauchía
 * @brief I2S Routines
 * @version 0.3
 * @date 2024-03
 */

#ifndef I2S_H
#define I2S_H

#include "driver/i2s.h"
#include <vars.h>
#include <hal.h>

/**
 * @brief I2S DAC Port Configuration
 *
 */
static i2s_config_t i2s_dac_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLING_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_24BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 2,
    .dma_buf_len = BUFFER,
    .use_apll = true,
    .tx_desc_auto_clear = true,
    .fixed_mclk = SAMPLING_RATE * 384};

/**
 * @brief I2S DAC Pin Configuration
 *
 */
static i2s_pin_config_t i2s_dac_pin_config = {.bck_io_num = I2S_BCK, .ws_io_num = I2S_LRC, .data_out_num = I2S_DOUT, .data_in_num = I2S_PIN_NO_CHANGE};

/**
 * @brief Start DAC
 *
 * @param sample_rate
 * @param num_channels
 * @param bit_depth
 */
static void DAC_start(int sample_rate, int num_channels, int bit_depth)
{
    i2s_dac_config.sample_rate = sample_rate;
    i2s_dac_config.fixed_mclk = sample_rate * 384;
    i2s_dac_config.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
    if (num_channels == 1)
        i2s_dac_config.channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT;
    i2s_driver_install(I2S_NUM_0, &i2s_dac_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &i2s_dac_pin_config);
    i2s_set_sample_rates(I2S_NUM_0, sample_rate);
    i2s_set_clk(I2S_NUM_0, sample_rate, I2S_BITS_PER_SAMPLE_16BIT, (i2s_channel_t)num_channels);
    i2s_start(I2S_NUM_0);
    // PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0_CLK_OUT1);
    // WRITE_PERI_REG(PIN_CTRL, 0xFFF0);
}

/**
 * @brief Stop DAC
 *
 */
static void DAC_stop()
{
    i2s_stop(I2S_NUM_0);
    i2s_driver_uninstall(I2S_NUM_0);
}

/**
 * @brief I2S ADC Port Configuration
 *
 */
static i2s_config_t i2s_adc_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLING_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_24BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = BUFFER,
    .use_apll = true,
    .tx_desc_auto_clear = true,
    .fixed_mclk = SAMPLING_RATE * 384};

/**
 * @brief I2S ADC Pin Configuration
 *
 */
static i2s_pin_config_t i2s_adc_pin_config = {.bck_io_num = I2S_BCK, .ws_io_num = I2S_LRC, .data_out_num = I2S_PIN_NO_CHANGE, .data_in_num = I2S_DIN};

/**
 * @brief Start ADC
 *
 * @param sample_rate
 * @param num_channels
 * @param bit_depth
 */
static void ADC_start(int sample_rate, int num_channels, int bit_depth)
{
    i2s_adc_config.sample_rate = sample_rate;
    i2s_adc_config.fixed_mclk = sample_rate * 384;
    if (num_channels == 1)
        i2s_adc_config.channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT;
    i2s_driver_install(I2S_NUM_0, &i2s_adc_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &i2s_adc_pin_config);
    // i2s_set_sample_rates(I2S_NUM_0, sample_rate);
}

/**
 * @brief Stop ADC
 *
 */
static void ADC_stop()
{
    i2s_driver_uninstall(I2S_NUM_0);
}

#endif