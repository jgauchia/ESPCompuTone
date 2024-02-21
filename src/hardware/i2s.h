/**
 * @file i2s.h
 * @author Jordi Gauchía
 * @brief I2S Routines
 * @version 0.1
 * @date 2024-02-21
 */

#define SAMPLING_RATE 16000
#define BUFFER 512
#define CHANNEL_BUFFER 1024

int rxbuf[CHANNEL_BUFFER], txbuf[CHANNEL_BUFFER];
float l_in[CHANNEL_BUFFER / 2], r_in[CHANNEL_BUFFER / 2];
float l_out[CHANNEL_BUFFER / 2], r_out[CHANNEL_BUFFER / 2];

float val_gain_input = 1.0f;
float val_gain_out = 2.0f;

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
    i2s_start(I2S_NUM_0);
}


void process_Audio(void *pvParameters)
{
    (void)pvParameters;
    float sample = 0;
    for (;;)
    {
        size_t readsize = 0;

        esp_err_t rxfb = i2s_read(I2S_NUM_0, &rxbuf[0], CHANNEL_BUFFER * 2, &readsize, 1000);

        if (rxfb == ESP_OK && readsize == CHANNEL_BUFFER * 2)
        {
            /*split L an R input*/
            int y = 0;
            for (int i = 0; i < CHANNEL_BUFFER; i += 2)
            {
                r_out[y] = (float)rxbuf[i];
                l_out[y] = (float)rxbuf[i + 1];
                y++;
            }

            /*gain input   */
            for (int i = 0; i < CHANNEL_BUFFER / 2; i++)
            {
                r_out[i] = val_gain_input * r_out[i];
                l_out[i] = val_gain_input * l_out[i];
            }

            /*gain output */
            for (int i = 0; i < CHANNEL_BUFFER / 2; i++)
            {
                r_out[i] = val_gain_out * r_out[i];
                l_out[i] = val_gain_out * l_out[i];
            }
            y = 0;
            for (int i = 0; i < CHANNEL_BUFFER / 2; i++)
            {
                txbuf[y] = (int)l_out[i];
                txbuf[y + 1] = (int)r_out[i];
                y = y + 2;
            }

            i2s_write(I2S_NUM_0, &txbuf[0], CHANNEL_BUFFER * 2, &readsize, 1000);
        }
    }
    vTaskDelete(NULL);
}