/**
 * @file vars.h
 * @author Jordi Gauchía
 * @brief Main program variables
 * @version 0.1
 * @date 2024-02-26
 */

/**
 * @brief LVGL Backgroun color for LovyanGFX
 * 
 */
#define LVGL_BKG 0x10A3

/**
 * @brief Sampling rate and buffer recording
 * 
 */
#define SAMPLING_RATE 22000
#define BUFFER 1024

/**
 * @brief Boolean vars for tape status
 * 
 */
bool is_record    = false;
bool is_play      = false;
bool is_rewind    = false;
bool is_advance   = false;
bool is_stop      = true;
bool is_eject     = false;
bool is_pause     = false;


/**
 * @brief PCF8574 Object declaration
 * 
 */
PCF8574 keys(0x20);


/**
 * @brief Keys read delay
 *
 */
#define KEYS_UPDATE_TIME 120
MyDelay keys_delay(KEYS_UPDATE_TIME);


/**
 * @brief TFT object declaration
 * 
 */
static TFT_eSPI tft;

/**
 * @brief GIF object declaration
 * 
 */
AnimatedGIF gif;


/**
 * @brief Max GIF duration (ms)
 * 
 */
int maxGifDuration = 5000; 
