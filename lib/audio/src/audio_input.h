/**
 * @file audio_input.h
 * @author Jordi Gauchía
 * @brief Audio Input Routines
 * @version 0.1
 * @date 2024-03
 */

/**
 * @brief Input Samples from I2S ADC
 *
 * @param samples
 * @param count
 */
int in_sample(int16_t *samples, int count)
{
    // read from i2s
    int32_t *raw_samples = (int32_t *)malloc(sizeof(int32_t) * count);
    size_t bytes_read = 0;
    i2s_read(I2S_NUM_0, raw_samples, sizeof(int32_t) * count, &bytes_read, 1000);
    int samples_read = bytes_read / sizeof(int32_t);
    for (int i = 0; i < samples_read; i++)
    {
        samples[i] = (raw_samples[i] & 0xFFFFFFF0) >> 11;
    }
    free(raw_samples);
    return samples_read;
}

/**
 * @brief Record WAV file
 *
 * @param fname
 */
void rec_wav(const char *fname)
{
    int16_t *samples = (int16_t *)malloc(sizeof(int16_t) * 1024);
    log_i("Start recording");
    //  open the file on the sdcard
    FILE *fp = fopen(fname, "wb");
    // create a new wave file writer
    WAVFileWriter *wav = new WAVFileWriter(fp, SAMPLING_RATE);
    ADC_start();
    // keep writing until the user releases the button
    while (!Serial.available())
    {
        int samples_read = in_sample(samples, BUFFER);
        int64_t start = esp_timer_get_time();
        wav->write(samples, samples_read);
        int64_t end = esp_timer_get_time();
        // log_i("Wrote %d samples in %lld microseconds", samples_read, end - start);
    }
    // stop the input
    ADC_stop();
    // and finish the writing
    wav->finish();
    fclose(fp);
    delete wav;
    free(samples);
    log_i("Finished recording");
}
