/**
 * @file rec_wav.h
 * @author Jordi Gauchía
 * @brief WAV Record routines
 * @version 0.2
 * @date 2024-03
 */

#include <stdio.h>

#define FILE_NAME_SIZE 128
// Función para grabar audio en un archivo WAV con parámetros personalizables
void rec_wav(const char *file_name, uint32_t sample_rate, uint8_t channels, uint8_t bits_per_sample)
{
    ADC_start(sample_rate, channels, bits_per_sample);

    // Abrir el archivo WAV
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
    int32_t *i2s_data_in = (int32_t *)malloc(sizeof(int32_t) * buffer_size);
    int16_t *i2s_data_out = (int16_t *)malloc(sizeof(int16_t) * buffer_size);
    size_t bytes_read;

    while (!is_stop)
    {
        // Escribir datos de audio en el controlador I2S
        i2s_read(I2S_NUM_0, i2s_data_in, sizeof(int32_t) * buffer_size, &bytes_read, portMAX_DELAY);
        // i2s_adc_data_scale(i2s_data_write, i2s_data, buffer_size);
        //  Escribir datos de audio en el archivo WAV
        int samples_read = bytes_read / sizeof(int32_t);

        for (int i = 0; i < samples_read; i++)
        {
            i2s_data_out[i] = (i2s_data_in[i] & 0xFFFFFF00) >> 11;
        }

        fwrite(i2s_data_out, sizeof(int16_t), samples_read, wav_file);
        taskYIELD();
        bytes_written += sizeof(int16_t) * samples_read;

        if (ferror(wav_file) == 1)
            log_e("error");
        else
            log_i("%d bytes read, %d samples , %d bytes", bytes_read, samples_read, bytes_written);
    }

    log_i("size %d", bytes_written);
    // Actualizar los campos ChunkSize y Subchunk2Size en el encabezado del archivo WAV
    fseek(wav_file, 4, SEEK_SET);
    uint32_t chunk_size = bytes_written + 36;
    fwrite(&chunk_size, sizeof(uint32_t), 1, wav_file);

    fseek(wav_file, 40, SEEK_SET);
    uint32_t subchunk2_size = bytes_written;
    fwrite(&subchunk2_size, sizeof(uint32_t), 1, wav_file);
    // Cerrar el archivo WAV y liberar la memoria
    fclose(wav_file);
    free(i2s_data_in);
    free(i2s_data_out);

    // Desinstalar el controlador I2S
    ADC_stop();
    is_record = false;
    is_stop = true;
}
