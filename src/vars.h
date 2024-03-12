/**
 * @file vars.h
 * @author Jordi Gauchía
 * @brief Main program variables
 * @version 0.3
 * @date 2024-03
 */

/**
 * @brief LVGL Backgroun color for LovyanGFX
 *
 */
#define LVGL_BKG 0x10A3

/**
 * @brief TFT Calibration
 *
 */
#define CALIBRATION_FILE "/TouchCalData1"
bool REPEAT_CAL = false;

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
bool is_record = false;
bool is_play = false;
bool is_rewind = false;
bool is_advance = false;
bool is_stop = true;
bool is_eject = false;
bool is_pause = false;

/**
 * @brief PCF8574 Object declaration
 *
 */
PCF8574 keys(0x20);

/**
 * @brief Keys read delay
 *
 */
#define KEYS_UPDATE_TIME 135
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
int maxGifDuration = 0;

/**
 * @brief Various boolean flags
 *
 */
bool sdloaded = false;
bool fileopen = false;
bool is_mainscreen = true;
bool fileload = false;
bool filesave = false;

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
uint32_t sample_rate = 18000;
uint8_t num_channels = 2;
uint8_t bit_depth = 16;

/**
 * @brief Total size of WAV samples in bytes
 * 
 */
size_t wav_bytes_written = 0;