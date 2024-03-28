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
 * @param fileName
 */
bool WAV::play(const char *fileName, eventCallback event)
{
    // WAV File open
    this->wavFile = fopen(fileName, "rb");
    if (this->wavFile == NULL)
    {
        log_e("WAV file open error");
        return true;
    }

    // Read WAV header
    uint8_t wavHeader[44];
    fread(wavHeader, 1, 44, wavFile);

    // Get Header Info
    uint32_t sampleRate = wavHeader[24] | (wavHeader[25] << 8) | (wavHeader[26] << 16) | (wavHeader[27] << 24);
    uint16_t numChannels = wavHeader[22] | (wavHeader[23] << 8);
    uint16_t bitsPerSample = wavHeader[34] | (wavHeader[35] << 8);

    log_i("Sample Rate: %u Hz", sampleRate);
    log_i("Channels: %u", numChannels);
    log_i("Bits per Sample: %u", bitsPerSample);

    // Configure and start DAC
    startDAC(sampleRate, numChannels, bitsPerSample);

    // Jump WAV header
    fseek(this->wavFile, 44, SEEK_SET);

    // Buffer to store WAV data
    uint8_t wavData[this->bufferSize];
    int16_t wavData16bits[this->bufferSize];

    tapeEvent currentEvent = tapeEvent::PLAY;
    while (!feof(this->wavFile))
    {
        if (currentEvent != tapeEvent::PAUSE)
        {
            // Read WAV audio data
            size_t bytesRead = fread(wavData, 1, sizeof(wavData), wavFile);

            if (bytesRead == 0)
                //  WAV file end
                break;

            if (bitsPerSample == 8)
            {
                for (size_t i = 0; i < bytesRead; i++)
                {
                    // Convert 8 bit per sample to 16 for the DAC
                    wavData16bits[i] = (int16_t)((wavData[i] - 0x80) << 8);
                }
                //  Out I2S data
                i2s_write(I2S_NUM_0, wavData16bits, bytesRead * 2, &bytesWritten, portMAX_DELAY);
            }
            else
                //  Out I2S data
                i2s_write(I2S_NUM_0, wavData, bytesRead, &bytesWritten, portMAX_DELAY);
        }
        if (currentEvent == tapeEvent::STOP)
            break;

        currentEvent = event();
    }

    // Close WAV file
    fclose(this->wavFile);

    // Stop DAC
    stopDAC();

    return false;
}

/**
 * @brief Record WAV
 *
 * @param fileName
 * @param sampleRate
 * @param numChannels
 * @param bitsPerSample
 * @param event
 * @return true
 * @return false
 */
bool WAV::rec(const char *fileName, uint32_t sampleRate, uint8_t numChannels, uint8_t bitsPerSample, eventCallback event)
{
    bool file_write_error = false;

    // Configure and start ADC
    startADC(sampleRate, numChannels, bitsPerSample);

    // Open WAV file
    this->wavFile = fopen(fileName, "wb");
    if (this->wavFile == NULL)
    {
        log_e("Error open WAV file");
        return true;
    }

    // Write WAV Header
    uint8_t wavHeader[44] = {
        'R', 'I', 'F', 'F',                                                                                                                                                                                                                                        // ChunkID
        0, 0, 0, 0,                                                                                                                                                                                                                                                // ChunkSize (actualizar luego)
        'W', 'A', 'V', 'E',                                                                                                                                                                                                                                        // Format
        'f', 'm', 't', ' ',                                                                                                                                                                                                                                        // Subchunk1ID
        16, 0, 0, 0,                                                                                                                                                                                                                                               // Subchunk1Size
        1, 0,                                                                                                                                                                                                                                                      // AudioFormat (PCM = 1)
        numChannels, 0,                                                                                                                                                                                                                                            // NumChannels
        sampleRate & 0xFF, (sampleRate >> 8) & 0xFF, (sampleRate >> 16) & 0xFF, (sampleRate >> 24) & 0xFF,                                                                                                                                                         // SampleRate
        (sampleRate * numChannels * (bitsPerSample / 8)) & 0xFF, ((sampleRate * numChannels * (bitsPerSample / 8)) >> 8) & 0xFF, ((sampleRate * numChannels * (bitsPerSample / 8)) >> 16) & 0xFF, ((sampleRate * numChannels * (bitsPerSample / 8)) >> 24) & 0xFF, // ByteRate
        (numChannels * (bitsPerSample / 8)) & 0xFF, ((numChannels * (bitsPerSample / 8)) >> 8) & 0xFF,                                                                                                                                                             // BlockAlign
        bitsPerSample, 0,                                                                                                                                                                                                                                          // BitsPerSample
        'd', 'a', 't', 'a',                                                                                                                                                                                                                                        // Subchunk2ID
        0, 0, 0, 0                                                                                                                                                                                                                                                 // Subchunk2Size (actualizar luego)
    };
    fwrite(wavHeader, sizeof(uint8_t), sizeof(wavHeader), this->wavFile);

    // Input Buffer for Audio input
    int32_t *wavData = (int32_t *)malloc(sizeof(int32_t) * this->bufferSize);
    int16_t *wavData16bits = (int16_t *)malloc(sizeof(int16_t) * this->bufferSize);
    size_t bytesRead;

    tapeEvent currentEvent = tapeEvent::RECORD;
    while (currentEvent != tapeEvent::STOP)
    {
        // read data from ADC
        i2s_read(I2S_NUM_0, wavData, sizeof(int32_t) * this->bufferSize, &bytesRead, portMAX_DELAY);

        // calculate total samples
        int samples_read = bytesRead / sizeof(int32_t);

        // convert 24bit audio to 16bit
        for (int i = 0; i < samples_read; i++)
        {
            wavData16bits[i] = (wavData[i]) >> 11;
        }

        // Write WAV samples
        fwrite(wavData16bits, sizeof(int16_t), samples_read, this->wavFile);
        bytesWritten += sizeof(int16_t) * samples_read;

        if (ferror(this->wavFile) == 1)
        {
            log_e("Recording Error");
            file_write_error = true;
            break;
        }
        // else
        //     log_i("%d bytes read, %d samples , %d bytes", bytesRead, samples_read, bytesWritten);

        currentEvent = event();
    }
    // Write Header ChunkSize
    fseek(this->wavFile, 4, SEEK_SET);
    uint32_t chunk_size = bytesWritten + 36;
    fwrite(&chunk_size, sizeof(uint32_t), 1, this->wavFile);

    // Write Header Subchunk2Size
    fseek(this->wavFile, 40, SEEK_SET);
    uint32_t subchunk2_size = bytesWritten;
    fwrite(&subchunk2_size, sizeof(uint32_t), 1, this->wavFile);

    // Close WAV file
    fclose(this->wavFile);

    // Delete file if has Write error
    if (file_write_error)
        remove(fileName);

    // Free Audio Buffers
    free(wavData);
    free(wavData16bits);

    // Stop ADC
    stopADC();

    return file_write_error;
}
