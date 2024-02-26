/**
 * @file main.cpp
 * @author Jordi Gauchia
 * @brief ESP Digital recorder
 * @version 0.1
 * @date 2024-02-21
 */

#include "Arduino.h"
#include <Wire.h>
#include <PCF8574.h>
#include <MyDelay.h>
#include <stdio.h>
#include <SPIFFS.h>
#include <SPI.h>
#include <FS.h>
#include <SD.h>
#include "driver/i2s.h"
#include <WAVFileReader.h>
#include <WAVFileWriter.h>
#include <vars.h>
#include <hal.h>
#include <sdcard.h>
#include <i2s.h>
#include <keys_def.h>
#include <keys.h>
#include <audio_output.h>
#include <audio_input.h>

void setup()
{
  init_sd();
  Wire.begin();
  Wire.setClock(4000);
  keys.begin();
  keys_delay.start();

  // Serial.begin(115200);
  //  play_wav("/sdcard/test.wav");
  //  rec_wav("/sdcard/input.wav");
  //  play_wav("/sdcard/input.wav");
  //
}

void loop()
{
  if (keys_delay.update())
    Check_keys();
}
