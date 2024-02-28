/**
 * @file GIFFile.h
 * @author Jordi Gauchía
 * @brief GIF File callback functions
 * @version 0.1
 * @date 2024-02-28
 */

/**
 * @brief GIF File holder
 * 
 */
static File FSGifFile;

/**
 * @brief X,Y Offsets to center GIF
 * 
 */
static int xOffset = 0;
static int yOffset = 0;

/**
 * @brief GIF File open callback
 * 
 * @param fname 
 * @param pSize 
 * @return void* 
 */
static void * GIFOpenFile(const char *fname, int32_t *pSize)
{
  FSGifFile = SD.open(fname);
  if (FSGifFile) {
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