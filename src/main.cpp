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
#include <lvgl_setup.h>

unsigned long millis_actual = 0;
static ulong lvgl_tick_millis = millis();

void setup()
{
  init_sd();
  Wire.begin();
  Wire.setClock(4000);
  keys.begin();
  keys_delay.start();

  tft.init();
  tft.setRotation(1);
  tft.initDMA();
  tft.startWrite();
  tft.fillScreen(LVGL_BKG);
  tft.endWrite();
  init_LVGL();

  /*Create a white label, set its text and align it to the center*/
  lv_obj_t *label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, "Hello world");
  lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

  // Serial.begin(115200);
  //  play_wav("/sdcard/test.wav");
  //  rec_wav("/sdcard/input.wav");
  //  play_wav("/sdcard/input.wav");
  //

  log_i("Model:%s %dMhz - Free mem:%dK %d%%", ESP.getChipModel(), ESP.getCpuFreqMHz(), (ESP.getFreeHeap() / 1024), (ESP.getFreeHeap() * 100) / ESP.getHeapSize());
}

void loop()
{
  if (keys_delay.update())
    Check_keys();
  gifPlay("/k7.gif");

  lv_timer_handler();
  unsigned long tick_millis = millis() - lvgl_tick_millis;
  lvgl_tick_millis = millis();
  lv_tick_inc(tick_millis);
  yield();
  delay(5);
}
