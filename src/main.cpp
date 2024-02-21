/**
 * @file main.cpp
 * @author Jordi Gauchia
 * @brief ESP Digital recorder
 * @version 0.1
 * @date 2024-02-21
 */

#include "Arduino.h"
#include <SPIFFS.h>
#include <SPI.h>
#include "driver/i2s.h"
#include "hardware/hal.h"
#include "hardware/sdcard.h"
#include "hardware/i2s.h"

void setup()
{
  set_I2S();
  xTaskCreatePinnedToCore(process_Audio, "process_Audio", 4096, NULL, 10, NULL, 0);
}

void loop()
{
  // put your main code here, to run repeatedly:
}
