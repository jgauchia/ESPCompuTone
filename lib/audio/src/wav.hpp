/**
 * @file wav.hpp
 * @author Jordi Gauchía
 * @brief WAV audio file class
 * @version 0.3
 * @date 2024-03
 */

#ifndef WAV_HPP
#define WAV_HPP

#include <Arduino.h>
#include <stdio.h>
#include <i2s.h>
#include <vars.h>
#include <events.h>

/**
 * @brief WAV audio class
 *
 */
class WAV
{
private:
    FILE *wav_file;
    const size_t buffer_size = 1024;

public:
    using eventCallback = Tape_event (*)();
    bool play(const char *file_name, eventCallback event);
    bool rec(const char *file_name, uint32_t sample_rate, uint8_t channels, uint8_t bits_per_sample, eventCallback event);
};

#endif