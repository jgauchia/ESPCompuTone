/**
 * @file gif.h
 * @author Jordi Gauchía
 * @brief GIF File callback functions
* @version 0.3
 * @date 2024-04
 */

#ifndef GIF_H
#define GIF_H

#include <AnimatedGIF.h>

/**
 * @brief GIF object declaration
 *
 */
AnimatedGIF gif;

/**
 * @brief Max GIF duration (ms)
 *
 */
int maxGifDuration = 0;

/**
 * @brief GIF File holder
 *
 */
static File FSGifFile;

/**
 * @brief GIF File open callback
 *
 * @param fname
 * @param pSize
 * @return void*
 */
static void *GIFOpenFile(const char *fname, int32_t *pSize)
{
  FSGifFile = SPIFFS.open(fname);
  if (FSGifFile)
  {
    *pSize = FSGifFile.size();
    return (void *)&FSGifFile;
  }
  return NULL;
}

/**
 * @brief GIF File close callback
 *
 * @param pHandle
 */
static void GIFCloseFile(void *pHandle)
{
  File *f = static_cast<File *>(pHandle);
  if (f != NULL)
    f->close();
}

/**
 * @brief GIF File read callback
 *
 * @param pFile
 * @param pBuf
 * @param iLen
 * @return int32_t
 */
static int32_t GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen)
{
  int32_t iBytesRead;
  iBytesRead = iLen;
  File *f = static_cast<File *>(pFile->fHandle);
  if ((pFile->iSize - pFile->iPos) < iLen)
    iBytesRead = pFile->iSize - pFile->iPos - 1;
  if (iBytesRead <= 0)
    return 0;
  iBytesRead = (int32_t)f->read(pBuf, iBytesRead);
  pFile->iPos = f->position();
  return iBytesRead;
}

/**
 * @brief GIF File seek callback
 *
 * @param pFile
 * @param iPosition
 * @return int32_t
 */
static int32_t GIFSeekFile(GIFFILE *pFile, int32_t iPosition)
{
  int i = micros();
  File *f = static_cast<File *>(pFile->fHandle);
  f->seek(iPosition);
  pFile->iPos = (int32_t)f->position();
  i = micros() - i;
  return pFile->iPos;
}

/**
 * @brief Draw GIF on screen
 *
 * @param pDraw
 */
void GIFDraw(GIFDRAW *pDraw)
{
  uint8_t *s;
  uint16_t *d, *usPalette, usTemp[320];
  int x, y, iWidth;

  iWidth = pDraw->iWidth;
  if (iWidth > tft.width())
    iWidth = tft.width();
  usPalette = pDraw->pPalette;
  y = pDraw->iY + pDraw->y; // current line

  s = pDraw->pPixels;
  if (pDraw->ucDisposalMethod == 2)
  { // restore to background color
    for (x = 0; x < iWidth; x++)
    {
      if (s[x] == pDraw->ucTransparent)
        s[x] = pDraw->ucBackground;
    }
    pDraw->ucHasTransparency = 0;
  }
  // Apply the new pixels to the main image
  if (pDraw->ucHasTransparency)
  { // if transparency used
    uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
    int x, iCount;
    pEnd = s + iWidth;
    x = 0;
    iCount = 0; // count non-transparent pixels
    while (x < iWidth)
    {
      c = ucTransparent - 1;
      d = usTemp;
      while (c != ucTransparent && s < pEnd)
      {
        c = *s++;
        if (c == ucTransparent)
        {      // done, stop
          s--; // back up to treat it like transparent
        }
        else
        { // opaque
          *d++ = usPalette[c];
          iCount++;
        }
      } // while looking for opaque pixels
      if (iCount && y>30)
      { // any opaque pixels?
        tft.pushImage((pDraw->iX + x) + xOffset, y + yOffset, iCount, 1, (uint16_t *)usTemp);
        x += iCount;
        iCount = 0;
      }
      // no, look for a run of transparent pixels
      c = ucTransparent;
      while (c == ucTransparent && s < pEnd)
      {
        c = *s++;
        if (c == ucTransparent)
          iCount++;
        else
          s--;
      }
      if (iCount)
      {
        x += iCount; // skip these
        iCount = 0;
      }
    }
  }
  else
  {
    s = pDraw->pPixels;
    // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
    for (x = 0; x < iWidth; x++)
      usTemp[x] = usPalette[*s++];
    tft.pushImage((pDraw->iX + x) + xOffset, y + yOffset, iWidth, 1, (uint16_t *)usTemp);
  }
}

/**
 * @brief Init GIF File
 *
 * @param gifPath
 */
void initGIF(const char *gifPath)
{
  gif.begin(BIG_ENDIAN_PIXELS);

  if (!gif.open(gifPath, GIFOpenFile, GIFCloseFile, GIFReadFile, GIFSeekFile, GIFDraw))
    log_e("Could not open gif %s", gifPath);

  // center the GIF !!
  int w = gif.getCanvasWidth();
  int h = gif.getCanvasHeight();
  xOffset = (tft.width() - w) / 2;
  yOffset = (tft.height() - h) / 2;
}

#endif