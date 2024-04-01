/**
 * @file tft.h
 * @author Jordi Gauchía
 * @brief TFT definition and functions
 * @version 0.3
 * @date 2024-04
 */

#ifndef TFT_H
#define TFT_H

#include <lvgfx.hpp>
#include <LGFX_TFT_eSPI.hpp>
#include <SPIFFS.h>
#include <SPI.h>
#include <FS.h>
#include <SD.h>

/**
 * @brief TFT Calibration
 *
 */
#define CALIBRATION_FILE "/TouchCalData1"
static bool repeatCal = false;

/**
 * @brief TFT object declaration
 *
 */
static TFT_eSPI tft;


/**
 * @brief Touch calibrate
 *
 */
static void touchCalibrate()
{
    uint16_t calData[8];
    uint8_t calDataOK = 0;

    if (SPIFFS.exists(CALIBRATION_FILE))
    {
        if (repeatCal)
            SPIFFS.remove(CALIBRATION_FILE);
        else
        {
            File f = SPIFFS.open(CALIBRATION_FILE, "r");
            if (f)
            {
                if (f.readBytes((char *)calData, 16) == 16)
                    calDataOK = 1;
                f.close();
            }
            else
                log_v("Error opening touch configuration");
        }
    }

    if (calDataOK && !repeatCal)
        tft.setTouchCalibrate(calData);
    else
    {
        tft.drawCenterString("TOUCH THE ARROW MARKER.", 160, tft.height() >> 1, &fonts::DejaVu18);
        tft.calibrateTouch(calData, TFT_WHITE, TFT_BLACK, std::max(tft.width(), tft.height()) >> 3);
        tft.drawCenterString("DONE!", 160, (tft.height() >> 1) + 30, &fonts::DejaVu40);
        delay(500);
        tft.drawCenterString("TOUCH TO CONTINUE.", 160, (tft.height() >> 1) + 100, &fonts::DejaVu18);

        File f = SPIFFS.open(CALIBRATION_FILE, "w");
        if (f)
        {
            f.write((const unsigned char *)calData, 16);
            f.close();
        }

        uint16_t touchX, touchY;
        while (!tft.getTouch(&touchX, &touchY))
        {
        };
    }
}

/**
 * @brief Init tft display
 *
 */
static void initTFT()
{
    tft.init();
    tft.setRotation(1);
    touchCalibrate();
    tft.initDMA();
}

#endif