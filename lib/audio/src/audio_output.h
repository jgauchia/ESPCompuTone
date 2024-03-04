/**
 * @file audio_output.h
 * @author Jordi Gauchía
 * @brief  Audio Output Routines.
 * @version 0.1
 * @date 2024-02-23
 */

const int NUM_FRAMES_TO_SEND = 256;

/**
 * @brief Process current Sample
 *
 */
int16_t process_sample(int16_t sample) { return sample; }

/**
 * @brief Output samples to I2S DAC
 *
 * @param samples
 * @param count
 */
void out_sample(int16_t *samples, int count)
{
    int16_t *frames = (int16_t *)malloc(2 * sizeof(int16_t) * NUM_FRAMES_TO_SEND);
    int sample_index = 0;
    while (sample_index < count)
    {
        int samples_to_send = 0;
        for (int i = 0; i < NUM_FRAMES_TO_SEND && sample_index < count; i++)
        {
            int sample = process_sample(samples[sample_index]);
            frames[i * 2] = sample;
            frames[i * 2 + 1] = sample;
            samples_to_send++;
            sample_index++;
        }
        size_t bytes_written = 0;
        i2s_write(I2S_NUM_0, frames, samples_to_send * sizeof(int16_t) * 2, &bytes_written, portMAX_DELAY);
        if (bytes_written != samples_to_send * sizeof(int16_t) * 2)
        {
            log_i("Did not output all bytes");
        }
    }
    free(frames);
}

/**
 * @brief Play WAV File
 *
 * @param fname
 */
void play_wav(const char *fname)
{
    int16_t *samples = (int16_t *)malloc(sizeof(int16_t) * 1024);
    FILE *fp = fopen(fname, "rb");
    WAVFileReader *wav = new WAVFileReader(fp);
    log_i("Opened wav file");
    DAC_start(wav->sample_rate(), wav->num_channels(), wav->bit_depth());
    log_i("Start playing");
    while (true)
    {
        int samples_read = wav->read(samples, 1024);
        if (samples_read == 0)
        {
            break;
        }
        // log_i("Read %d samples", samples_read);
        out_sample(samples, samples_read);
        // log_i("Played samples");
    }
    DAC_stop();
    fclose(fp);
    delete wav;
    free(samples);
    is_stop = true;
    is_play = false;
    log_i("Finished playing");
}