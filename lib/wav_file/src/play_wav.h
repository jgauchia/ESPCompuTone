/**
 * @file play_wav.h
 * @author Jordi Gauchía
 * @brief WAV Play routines
 * @version 0.2
 * @date 2024-03
 */

#include <stdio.h>

void play_wav(const char *file_name)
{
    // WAV File open
    FILE *wav_file = fopen(file_name, "rb");
    if (wav_file == NULL)
    {
        log_e("WAV file open error");
        return;
    }

    // Read WAV header
    uint8_t wav_header[44];
    fread(wav_header, 1, 44, wav_file);

    // Get Header Info
    uint32_t sample_rate = wav_header[24] | (wav_header[25] << 8) | (wav_header[26] << 16) | (wav_header[27] << 24);
    uint16_t channels = wav_header[22] | (wav_header[23] << 8);
    uint16_t bits_per_sample = wav_header[34] | (wav_header[35] << 8);

    log_i("Sample Rate: %u Hz", sample_rate);
    log_i("Channels: %u", channels);
    log_i("Bits per Sample: %u", bits_per_sample);

    // Configure and start DAC
    DAC_start(sample_rate, channels, bits_per_sample);

    // Jump WAV header
    fseek(wav_file, 44, SEEK_SET);

    // Buffer to store WAV data
    uint8_t wav_data[1024];
    int16_t wav_data_16bits[1024];

    size_t bytes_written;

    while (true)
    {
        // Read WAV audio data
        size_t bytes_read = fread(wav_data, 1, sizeof(wav_data), wav_file);
        if (bytes_read == 0)
        {
            //  WAV file end
            break;
        }

        if (bits_per_sample == 8)
        {
            for (size_t i = 0; i < bytes_read; i++)
            {
                // Convert 8 bit per sample to 16 for the DAC
                wav_data_16bits[i] = (int16_t)((wav_data[i] - 0x80) << 8 );
            }
            if (!is_pause)
                // Out I2S data
                i2s_write(I2S_NUM_0, wav_data_16bits, bytes_read * 2, &bytes_written, portMAX_DELAY);
        }
        else
        {
            if (!is_pause)
                // Out I2S data
                i2s_write(I2S_NUM_0, wav_data, bytes_read, &bytes_written, portMAX_DELAY);
        }

        if (is_stop)
            break;
    }

    // Close WAV file
    fclose(wav_file);

    // Stop DAC
    DAC_stop();

    is_stop = true;
    is_play = false;
}
