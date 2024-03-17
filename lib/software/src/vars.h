/**
 * @file vars.h
 * @author Jordi Gauchía
 * @brief Main program variables
 * @version 0.3
 * @date 2024-03
 */

#ifndef VARS_H
#define VARS_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief LVGL Backgroun color for LovyanGFX and TFT size
 *
 */
#define LVGL_BKG 0x10A3
#define TFT_WIDTH 320
#define TFT_HEIGHT 240

/**
 * @brief Boolean vars for tape status
 *
 */
bool isRecord = false;
bool isPlay = false;
bool isRewind = false;
bool isAdvance = false;
bool isStop = true;
bool isEject = false;
bool isPause = false;

/**
 * @brief Various boolean flags
 *
 */
bool sdLoaded = false;
bool isMainScreen = true;
bool isConfig = false;
bool fileOpen = false;
bool fileLoad = false;
bool fileSave = false;

/**
 * @brief File explorer vars
 *
 */
char *filePath;
char *fileName;

/**
 * @brief Default Recording
 *
 */
uint32_t sampleRate = 22050;
uint8_t numChannels = 2;
uint8_t bitDepth = 16;

/**
 * @brief X,Y Offsets to center GIF
 *
 */
int xOffset = 0;
int yOffset = 0;


#ifdef __cplusplus
} /*extern "C"*/
#endif


#endif