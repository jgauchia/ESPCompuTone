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
#include <lvgfx.hpp>
#include <LGFX_TFT_eSPI.hpp>
#include <AnimatedGIF.h>
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
#include <GIFFile.h>
#include <tasks.h>
#include <lv_sd_fs.h>
#include <lvgl_setup.h>

unsigned long millis_actual = 0;
static ulong lvgl_tick_millis = millis();

void setup()
{
  init_sd();
  init_SPIFFS();
  init_tasks();

  Wire.begin();
  keys.begin();
  keys_delay.start();

  tft.init();
  tft.setRotation(1);
  tft.initDMA();
  init_LVGL();

  create_notify_bar();
  // Serial.begin(115200);
  //  play_wav("/sdcard/test.wav");
  //  rec_wav("/sdcard/input.wav");
  //  play_wav("/sdcard/input.wav");
  //

  log_i("Model:%s %dMhz - Free mem:%dK %d%%", ESP.getChipModel(), ESP.getCpuFreqMHz(), (ESP.getFreeHeap() / 1024), (ESP.getFreeHeap() * 100) / ESP.getHeapSize());
  log_i("SPIFFS: Total %d - Free %d", SPIFFS.totalBytes(), (SPIFFS.totalBytes() - SPIFFS.usedBytes()));
  log_i("FLASH: Total %d - Free %d", ESP.getFlashChipSize(), ESP.getFreeSketchSpace());
}

void loop()
{
  gifPlay("/k7.gif");
  lv_timer_handler();
  unsigned long tick_millis = millis() - lvgl_tick_millis;
  lvgl_tick_millis = millis();
  lv_tick_inc(tick_millis);
  // yield();
  //  delay(5);
}
