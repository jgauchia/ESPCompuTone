/**
 * @file vars.h
 * @author Jordi Gauchía
 * @brief Main program variables
 * @version 0.3
 * @date 2024-03
 */

#ifndef VARS_H
#define VARS_H

#include <Arduino.h>

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
extern bool isRecord;
extern bool isPlay;
extern bool isRewind;
extern bool isAdvance;
extern bool isStop;
extern bool isEject;
extern bool isPause;

/**
 * @brief Various boolean flags
 *
 */
extern bool sdLoaded;
extern bool isMainScreen;
extern bool isConfig;
extern bool fileOpen;
extern bool fileLoad;
extern bool fileSave;

/**
 * @brief File explorer vars
 *
 */
extern char *filePath;
extern char *fileName;

/**
 * @brief Default Recording
 *
 */
extern uint32_t sampleRate;
extern uint8_t numChannels;
extern uint8_t bitDepth;

/**
 * @brief X,Y Offsets to center GIF
 *
 */
extern int xOffset;
extern int yOffset;

/**
 * @brief Network variables
 * 
 */
typedef enum
{
    NONE,
    NETWORK_SEARCHING,
    NETWORK_CONNECTED_POPUP,
    NETWORK_CONNECTED,
    NETWORK_CONNECT_FAILED
} Network_Status_t;
extern Network_Status_t networkStatus;

extern int totalWificount;
extern int foundNetworks;
extern unsigned long networkTimeout;

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif