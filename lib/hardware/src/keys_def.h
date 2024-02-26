/**
 * @file keys_def.h
 * @author Jordi Gauchia
 * @brief  Key definitions
 * @version 0.1
 * @date 2024-02-26
 */

/**
 * @brief Keys enum
 * 
 */
enum key_def
{
    REC,
    PLAY,
    REC_PLAY,
    REW,
    ADV,
    STOP_EJ,
    PAUSE,
    NONE,
};

/**
 * @brief Returns key pressed
 * 
 */
int key_pressed = NONE;