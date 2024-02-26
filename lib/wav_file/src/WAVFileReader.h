#pragma once

#include "WAVFile.h"
#include <stdio.h>
#include "esp32-hal-log.h"

class WAVFileReader
{
private:
    wav_header_t m_wav_header;

    FILE *m_fp;

public:
    WAVFileReader(FILE *fp);
    int sample_rate() { return m_wav_header.sample_rate; }
    int num_channels() { return m_wav_header.num_channels; }
    int read(int16_t *samples, int count);
};
