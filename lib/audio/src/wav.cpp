/**
 * @file wav.cpp
 * @author Jordi Gauchía
 * @brief WAV audio file class
 * @version 0.3
 * @date 2024-03
 */

#include "wav.hpp"

/**
 * @brief Play WAV
 *
 * @param file_name
 */
bool WAV::play(const char *file_name, eventCallback event)
{
    // WAV File open
    this->wav_file = fopen(file_name, "rb");
    if (this->wav_file == NULL)
    {
        log_e("WAV file open error");
        return true;
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
    fseek(this->wav_file, 44, SEEK_SET);

    // Buffer to store WAV data
    uint8_t wav_data[WAV::buffer_size];
    int16_t wav_data_16bits[WAV::buffer_size];

    size_t bytes_written;

    Tape_event current_event;
    while (!feof(this->wav_file))
    {
        if (current_event != Tape_event::PAUSE)
        {
            // Read WAV audio data
            size_t bytes_read = fread(wav_data, 1, sizeof(wav_data), wav_file);

            if (bytes_read == 0)
                //  WAV file end
                break;

            if (bits_per_sample == 8)
            {
                for (size_t i = 0; i < bytes_read; i++)
                {
                    // Convert 8 bit per sample to 16 for the DAC
                    wav_data_16bits[i] = (int16_t)((wav_data[i] - 0x80) << 8);
                }
                //  Out I2S data
                i2s_write(I2S_NUM_0, wav_data_16bits, bytes_read * 2, &bytes_written, portMAX_DELAY);
            }
            else
                //  Out I2S data
                i2s_write(I2S_NUM_0, wav_data, bytes_read, &bytes_written, portMAX_DELAY);
        }
        if (current_event == Tape_event::STOP)
            break;

        current_event = event();
    }

    // Close WAV file
    fclose(this->wav_file);

    // Stop DAC
    DAC_stop();

    return false;
}

bool WAV::rec(const char *file_name, uint32_t sample_rate, uint8_t channels, uint8_t bits_per_sample, eventCallback event)
{
    bool file_write_error = false;
    size_t wav_bytes_written = 0;

    // Configure and start ADC
    ADC_start(sample_rate, channels, bits_per_sample);

    // Open WAV file
    this->wav_file = fopen(file_name, "wb");
    if (this->wav_file == NULL)
    {
        log_e("Error open WAV file");
        return true;
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
    fwrite(wav_header, sizeof(uint8_t), sizeof(wav_header), this->wav_file);

    // Input Buffer for Audio input
    int32_t *wav_data_in = (int32_t *)malloc(sizeof(int32_t) * WAV::buffer_size);
    int16_t *wav_data_16bits = (int16_t *)malloc(sizeof(int16_t) * WAV::buffer_size);
    size_t bytes_read;

    Tape_event current_event = Tape_event::RECORD;
    while (current_event != Tape_event::STOP)
    {
        // read data from ADC
        i2s_read(I2S_NUM_0, wav_data_in, sizeof(int32_t) * WAV::buffer_size, &bytes_read, portMAX_DELAY);

        // calculate total samples
        int samples_read = bytes_read / sizeof(int32_t);

        // convert 24bit audio to 16bit
        for (int i = 0; i < samples_read; i++)
        {
            wav_data_16bits[i] = (wav_data_in[i]) >> 11;
        }

        // Write WAV samples
        fwrite(wav_data_16bits, sizeof(int16_t), samples_read, this->wav_file);
        wav_bytes_written += sizeof(int16_t) * samples_read;

        if (ferror(this->wav_file) == 1)
        {
            log_e("Recording Error");
            file_write_error = true;
            break;
        }
        // else
        //     log_i("%d bytes read, %d samples , %d bytes", bytes_read, samples_read, bytes_written);

        current_event = event();
    }
    // Write Header ChunkSize
    fseek(this->wav_file, 4, SEEK_SET);
    uint32_t chunk_size = wav_bytes_written + 36;
    fwrite(&chunk_size, sizeof(uint32_t), 1, this->wav_file);

    // Write Header Subchunk2Size
    fseek(this->wav_file, 40, SEEK_SET);
    uint32_t subchunk2_size = wav_bytes_written;
    fwrite(&subchunk2_size, sizeof(uint32_t), 1, this->wav_file);

    // Close WAV file
    fclose(this->wav_file);

    // Delete file if has Write error
    if (file_write_error)
        remove(file_name);

    // Free Audio Buffers
    free(wav_data_in);
    free(wav_data_16bits);

    // Stop ADC
    ADC_stop();

    return file_write_error;
}
