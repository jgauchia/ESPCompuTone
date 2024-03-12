/**
 * @file keys_def.h
 * @author Jordi Gauchía
 * @brief  Key definitions
 * @version 0.3
 * @date 2024-03
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