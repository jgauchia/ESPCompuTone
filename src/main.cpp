/**
 * @file main.cpp
 * @author Jordi Gauchía
 * @brief ESP Digital recorder
 * @version 0.3
 * @date 2024-03
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

#include <vars.h>
#include <hal.h>
#include <network.hpp>
#include <sdcard.h>
#include <tft.h>
#include <gif.h>
#include <lvgl_setup.h>
#include <keys.h>
#include <i2s.h>
#include <tasks.h>

unsigned long millis_actual = 0;
static ulong lvgl_tick_millis = millis();

void setup()
{
  init_sd();
  init_SPIFFS();

  Wire.begin();
  keys.begin();
  keysDelay.start();

  init_tft();
  init_GIF("/k7.gif");
  init_LVGL();
  initTasks();

  log_i("Model:%s %dMhz - Free mem:%dK %d%%", ESP.getChipModel(), ESP.getCpuFreqMHz(), (ESP.getFreeHeap() / 1024), (ESP.getFreeHeap() * 100) / ESP.getHeapSize());
  log_i("SPIFFS: Total %d - Free %d", SPIFFS.totalBytes(), (SPIFFS.totalBytes() - SPIFFS.usedBytes()));
  log_i("FLASH: Total %d - Free %d", ESP.getFlashChipSize(), ESP.getFreeSketchSpace());
}

void loop()
{
  if (!fileOpen && !fileSave)
  {
    if (!isMainScreen)
    {
      lv_textarea_set_text(textarea, "");
      lv_obj_send_event(textarea, LV_EVENT_REFRESH, NULL);
      isMainScreen = true;
      lv_screen_load(mainScr);
    }
    if (!isConfig)
    {
      if (isStop || isPause)
      {
        gif.playFrame(true, &maxGifDuration);
        gif.reset();
      }
      else
      {
        gif.playFrame(true, NULL);
      }
    }
  }
  else if (fileOpen && !fileSave)
  {
    // Call file open screen
    if (sdLoaded)
    {
      isMainScreen = false;
      lv_screen_load(fileExplorer);
    }
  }
  else if (fileSave && !fileOpen)
  {
    // Call file save screen
    isMainScreen = false;
    lv_screen_load(fileSaveScr);
  }

  lv_timer_handler();
  unsigned long tick_millis = millis() - lvgl_tick_millis;
  lvgl_tick_millis = millis();
  lv_tick_inc(tick_millis);
  yield();
  delay(5);
}
