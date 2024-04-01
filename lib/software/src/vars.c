/**
 * @file vars.c
 * @author Jordi Gauchía
 * @brief Main program variables
 * @version 0.3
 * @date 2024-04
 */

#include <vars.h>

bool isRecord = false;
bool isPlay = false;
bool isRewind = false;
bool isAdvance = false;
bool isStop = true;
bool isEject = false;
bool isPause = false;

bool sdLoaded = false;
bool isMainScreen = true;
bool isConfig = false;
bool fileOpen = false;
bool fileLoad = false;
bool fileSave = false;
bool isDirRefresh = false;

char *filePath;
char *fileName;

uint32_t sampleRate = 22050;
uint8_t numChannels = 2;
uint8_t bitDepth = 16;

int xOffset = 0;
int yOffset = 0;

Network_Status_t networkStatus = NONE;

int totalWificount = 0;
int foundNetworks = 0;
unsigned long networkTimeout = 10 * 1000;

