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
#include <GIFFile.h>
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

int maxGifDuration    = 0; // 5000ms, max GIF duration

static void TFTDraw(int x, int y, int w, int h, uint16_t* lBuf )
{
  tft.pushImage( x+xOffset, y+yOffset, w, h, lBuf );
}


void GIFDraw(GIFDRAW *pDraw)
{
  uint8_t *s;
  uint16_t *d, *usPalette, usTemp[320];
  int x, y, iWidth;

  iWidth = pDraw->iWidth;
  if (iWidth > tft.width() )
      iWidth = tft.width() ;
  usPalette = pDraw->pPalette;
  y = pDraw->iY + pDraw->y; // current line

  s = pDraw->pPixels;
  if (pDraw->ucDisposalMethod == 2) {// restore to background color
    for (x=0; x<iWidth; x++) {
      if (s[x] == pDraw->ucTransparent)
          s[x] = pDraw->ucBackground;
    }
    pDraw->ucHasTransparency = 0;
  }
  // Apply the new pixels to the main image
  if (pDraw->ucHasTransparency) { // if transparency used
    uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
    int x, iCount;
    pEnd = s + iWidth;
    x = 0;
    iCount = 0; // count non-transparent pixels
    while(x < iWidth) {
      c = ucTransparent-1;
      d = usTemp;
      while (c != ucTransparent && s < pEnd) {
        c = *s++;
        if (c == ucTransparent) { // done, stop
          s--; // back up to treat it like transparent
        } else { // opaque
            *d++ = usPalette[c];
            iCount++;
        }
      } // while looking for opaque pixels
      if (iCount) { // any opaque pixels?
        TFTDraw( pDraw->iX+x, y, iCount, 1, (uint16_t*)usTemp );
        x += iCount;
        iCount = 0;
      }
      // no, look for a run of transparent pixels
      c = ucTransparent;
      while (c == ucTransparent && s < pEnd) {
        c = *s++;
        if (c == ucTransparent)
            iCount++;
        else
            s--;
      }
      if (iCount) {
        x += iCount; // skip these
        iCount = 0;
      }
    }
  } else {
    s = pDraw->pPixels;
    // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
    for (x=0; x<iWidth; x++)
      usTemp[x] = usPalette[*s++];
    TFTDraw( pDraw->iX, y, iWidth, 1, (uint16_t*)usTemp );
  }
} /* GIFDraw() */


void gifPlay( const char* gifPath )
{ // 0=infinite

  gif.begin(BIG_ENDIAN_PIXELS);

  if( ! gif.open( gifPath, GIFOpenFile, GIFCloseFile, GIFReadFile, GIFSeekFile, GIFDraw ) )
  {
    log_e("Could not open gif %s", gifPath );
  }

  int frameDelay = 0; // store delay for the last frame
  int then = 0; // store overall delay
  bool showcomment = false;

  // center the GIF !!
  int w = gif.getCanvasWidth();
  int h = gif.getCanvasHeight();
  xOffset = ( tft.width()  - w )  /2;
  yOffset = ( tft.height() - h ) /2;

  while (gif.playFrame(true, &frameDelay)) {
    then += frameDelay;
    if( then > maxGifDuration ) { // avoid being trapped in infinite GIF's
      break;
    }
  }

  gif.close();
}


#define LVGL_BKG 0x10A3


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


  gifPlay( "/k7.gif" );
}
