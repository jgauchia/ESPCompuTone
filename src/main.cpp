/**
 * @file main.cpp
 * @author Jordi Gauchia
 * @brief ESP Digital recorder
 * @version 0.1
 * @date 2024-02-21
 */

#include "Arduino.h"
#include <stdio.h>
#include <SPIFFS.h>
#include <SPI.h>
#include <FS.h>
#include <SD.h>
#include "driver/i2s.h"
#include <WAVFileReader.h>
#include <WAVFileWriter.h>
#include <hal.h>
#include <sdcard.h>
#include <i2s.h>
#include <audio_output.h>


void setup()
{
  init_sd();
  set_I2S();
  play_wav("/sdcard/test.wav");
}

void loop()
{
}
