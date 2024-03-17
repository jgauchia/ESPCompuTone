/**
 * @file sdcard.h
 * @author Jordi Gauchía
 * @brief SD Card definition and functions
 * @version 0.3
 * @date 2024-03
 */

#ifndef SDCARD_H
#define SDCARD_H

SPIClass spiSD = SPIClass(VSPI);
uint32_t sd_freq = 40000000;

/**
 * @brief SD Card init
 *
 */
void init_sd()
{
  spiSD.begin(SD_CLK, SD_MISO, SD_MOSI, SD_CS);
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, LOW);
  if (!SD.begin(SD_CS, spiSD, sd_freq, "/sdcard"))
  {
    log_i("SD Card Mount Failed");
    return;
  }
  else
  {
    log_i("SD Card Mounted");
    sdLoaded = true;
  }
}

/**
 * @brief SPIFFS Init
 *
 */
void init_SPIFFS()
{
  if (!SPIFFS.begin(true))
    log_i("SPIFFS Mount Failed");
  else
    log_i("SPIFFS Mounted");
}

#endif