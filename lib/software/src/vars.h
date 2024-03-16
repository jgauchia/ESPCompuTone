/**
 * @file vars.h
 * @author Jordi Gauchía
 * @brief Main program variables
 * @version 0.3
 * @date 2024-03
 */

#ifndef VARS_H
#define VARS_H

/**
 * @brief LVGL Backgroun color for LovyanGFX
 *
 */
#define LVGL_BKG 0x10A3


/**
 * @brief Sampling rate and buffer recording
 *
 */
#define SAMPLING_RATE 44100
#define BUFFER 256

#define TFT_WIDTH 320
#define TFT_HEIGHT 240

/**
 * @brief Boolean vars for tape status
 *
 */
static bool is_record = false;
static bool is_play = false;
static bool is_rewind = false;
static bool is_advance = false;
static bool is_stop = true;
static bool is_eject = false;
static bool is_pause = false;

/**
 * @brief Various boolean flags
 *
 */
static bool sdloaded = false;
static bool fileopen = false;
static bool is_mainscreen = true;
static bool fileload = false;
static bool filesave = false;
static bool is_config = false;

/**
 * @brief File explorer vars
 *
 */
static char *file_path;
static char *file_name;

/**
 * @brief Default Recording
 *
 */
static uint32_t sample_rate = 22050;
static uint8_t num_channels = 2;
static uint8_t bit_depth = 16;

/**
 * @brief X,Y Offsets to center GIF
 *
 */
static int xOffset = 0;
static int yOffset = 0;

#endif