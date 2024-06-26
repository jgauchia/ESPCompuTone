/**
 * @file wav.hpp
 * @author Jordi Gauchía
 * @brief WAV audio file class
 * @version 0.3
 * @date 2024-04
 */

#ifndef WAV_HPP
#define WAV_HPP

#include <Arduino.h>
#include <stdio.h>
#include "driver/i2s.h"
#include <events.h>

extern void startDAC(int sampleRate, int numChannels, int bitDepth);
extern void stopDAC();
extern void startADC(int sampleRate, int numChannels, int bitDepth);
extern void stopADC();

/**
 * @brief WAV audio class
 *
 */
class WAV
{
private:
    FILE *wavFile;
    const size_t bufferSize = 1024;
    size_t bytesWritten;

public:
    using eventCallback = tapeEvent (*)();
    bool play(const char *fileName, eventCallback event);
    bool rec(const char *fileName, uint32_t sampleRate, uint8_t numChannels, uint8_t bitsPerSample, eventCallback event);
};

#endif