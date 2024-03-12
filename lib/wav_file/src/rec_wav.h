/**
 * @file rec_wav.h
 * @author Jordi Gauchía
 * @brief WAV Record routines
 * @version 0.3
 * @date 2024-03
 */

#include <stdio.h>

/**
 * @brief Record WAV
 *
 * @param file_name
 * @param sample_rate
 * @param channels
 * @param bits_per_sample
 */
void rec_wav(const char *file_name, uint32_t sample_rate, uint8_t channels, uint8_t bits_per_sample)
{
    // Configure and start ADC
    ADC_start(sample_rate, channels, bits_per_sample);

    // Open WAV file
    FILE *wav_file = fopen(file_name, "wb");
    if (wav_file == NULL)
    {
        log_e("Error open WAV file");
        return;
    }

    // Write WAV Header
    uint8_t wav_header[44] = {
        'R', 'I', 'F', 'F',                                                                                                                                                                                                                                        // ChunkID
        0, 0, 0, 0,                                                                                                                                                                                                                                                // ChunkSize (actualizar luego)
        'W', 'A', 'V', 'E',                                                                                                                                                                                                                                        // Format
        'f', 'm', 't', ' ',                                                                                                                                                                                                                                        // Subchunk1ID
        16, 0, 0, 0,                                                                                                                                                                                                                                               // Subchunk1Size
        1, 0,                                                                                                                                                                                                                                                      // AudioFormat (PCM = 1)
        channels, 0,                                                                                                                                                                                                                                               // NumChannels
        sample_rate & 0xFF, (sample_rate >> 8) & 0xFF, (sample_rate >> 16) & 0xFF, (sample_rate >> 24) & 0xFF,                                                                                                                                                     // SampleRate
        (sample_rate * channels * (bits_per_sample / 8)) & 0xFF, ((sample_rate * channels * (bits_per_sample / 8)) >> 8) & 0xFF, ((sample_rate * channels * (bits_per_sample / 8)) >> 16) & 0xFF, ((sample_rate * channels * (bits_per_sample / 8)) >> 24) & 0xFF, // ByteRate
        (channels * (bits_per_sample / 8)) & 0xFF, ((channels * (bits_per_sample / 8)) >> 8) & 0xFF,                                                                                                                                                               // BlockAlign
        bits_per_sample, 0,                                                                                                                                                                                                                                        // BitsPerSample
        'd', 'a', 't', 'a',                                                                                                                                                                                                                                        // Subchunk2ID
        0, 0, 0, 0                                                                                                                                                                                                                                                 // Subchunk2Size (actualizar luego)
    };
    fwrite(wav_header, sizeof(uint8_t), sizeof(wav_header), wav_file);

    // Input Buffer for Audio input
    const size_t buffer_size = 1024;
    int32_t *wav_data_in = (int32_t *)malloc(sizeof(int32_t) * buffer_size);
    int16_t *wav_data_16bits = (int16_t *)malloc(sizeof(int16_t) * buffer_size);
    size_t bytes_read;

    while (!is_stop)
    {
        // read data from ADC
        i2s_read(I2S_NUM_0, wav_data_in, sizeof(int32_t) * buffer_size, &bytes_read, portMAX_DELAY);

        // calculate total samples
        int samples_read = bytes_read / sizeof(int32_t);

        // convert 24bit audio to 16bit
        for (int i = 0; i < samples_read; i++)
        {
            wav_data_16bits[i] = (wav_data_in[i]) >> 11;
        }

        // Write WAV samples
        fwrite(wav_data_16bits, sizeof(int16_t), samples_read, wav_file);
        wav_bytes_written += sizeof(int16_t) * samples_read;

        if (ferror(wav_file) == 1)
        {
            log_e("error");
            lv_label_set_text(file, "Recording Error");
            lv_obj_send_event(file, LV_EVENT_REFRESH, NULL);
            break;
        }
        // else
        //     log_i("%d bytes read, %d samples , %d bytes", bytes_read, samples_read, bytes_written);
    }
    // Write Header ChunkSize
    fseek(wav_file, 4, SEEK_SET);
    uint32_t chunk_size = wav_bytes_written + 36;
    fwrite(&chunk_size, sizeof(uint32_t), 1, wav_file);

    // Write Header Subchunk2Size
    fseek(wav_file, 40, SEEK_SET);
    uint32_t subchunk2_size = wav_bytes_written;
    fwrite(&subchunk2_size, sizeof(uint32_t), 1, wav_file);

    // Close WAV file
    fclose(wav_file);

    // Free Audio Buffers
    free(wav_data_in);
    free(wav_data_16bits);

    // Stop ADC
    ADC_stop();

    is_record = false;
    is_stop = true;
    filesave = true;
}
