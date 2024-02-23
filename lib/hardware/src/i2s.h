/**
 * @file i2s.h
 * @author Jordi Gauchía
 * @brief I2S Routines
 * @version 0.1
 * @date 2024-02-21
 */

#define SAMPLING_RATE 44100
#define BUFFER 512
#define CHANNEL_BUFFER 1024

/**
 * @brief I2S Port Configuration
 *
 */
i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX),
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
 * @brief I2S Pin Configuration
 *
 */
i2s_pin_config_t i2s_pin_config = {.bck_io_num = I2S_BCK, .ws_io_num = I2S_LRC, .data_out_num = I2S_DOUT, .data_in_num = I2S_DIN};

/**
 * @brief Set the I2S 
 *
 */
void set_I2S()
{
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &i2s_pin_config);
}
