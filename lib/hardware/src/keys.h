/**
 * @file keys.h
 * @author Jordi Gauchía
 * @brief Keys imput routines
 * @version 0.3
 * @date 2024-03
 */

#ifndef KEYS_H
#define KEYS_H

#include <PCF8574.h>
#include <MyDelay.h>

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
    NOKEY,
};

/**
 * @brief Returns key pressed
 *
 */
int key_pressed = NOKEY;

/**
 * @brief Read keys
 *
 * @return int -> enum structure keys index
 */
int Read_Keys()
{
    keys.read8();
    switch (keys.value())
    {
    case 254:
        return REC;
        break;
    case 253:
        return PLAY;
        break;
    case 252:
        return REC_PLAY;
        break;
    case 251:
        return REW;
        break;
    case 247:
        return ADV;
        break;
    case 239:
        return STOP_EJ;
        break;
    case 223:
        return PAUSE;
        break;
    default:
        return NOKEY;
        break;
    }
}

/**
 * @brief Keys actions
 *
 *
 */
void Check_keys()
{
    key_pressed = Read_Keys();

    switch (key_pressed)
    {
    case PLAY:
        if (is_stop && !is_pause && fileload)
        {
            log_i("PLAY");
            is_record = false;
            is_play = true;
            is_rewind = false;
            is_advance = false;
            is_stop = false;
            is_eject = false;
            is_pause = false;
            selectTapeKey(playBtn, true);
            lv_obj_send_event(playBtn, LV_EVENT_REFRESH, NULL);
        }
        break;
    case REC_PLAY:
        if (is_stop && !is_pause)
        {
            log_i("RECORD");
            is_record = true;
            is_play = false;
            is_rewind = false;
            is_advance = false;
            is_stop = false;
            is_eject = false;
            is_pause = false;
            selectTapeKey(recBtn, true);
            lv_obj_send_event(recBtn, LV_EVENT_REFRESH, NULL);
            selectTapeKey(playBtn, true);
            lv_obj_send_event(playBtn, LV_EVENT_REFRESH, NULL);
            lv_label_set_text(file, "Audio Recording");
            lv_obj_send_event(file, LV_EVENT_REFRESH, NULL);
        }
        break;
    case REW:
        if (is_stop && !is_pause)
        {
            log_i("REWIND");
            is_record = false;
            is_play = false;
            is_rewind = true;
            is_advance = false;
            is_stop = false;
            is_eject = false;
            is_pause = false;
            selectTapeKey(rewBtn, true);
            lv_obj_send_event(rewBtn, LV_EVENT_REFRESH, NULL);
        }
        break;
    case ADV:
        if (is_stop && !is_pause)
        {
            log_i("ADVANCE");
            is_record = false;
            is_play = false;
            is_rewind = false;
            is_advance = true;
            is_stop = false;
            is_eject = false;
            is_pause = false;
            selectTapeKey(ffwBtn, true);
            lv_obj_send_event(ffwBtn, LV_EVENT_REFRESH, NULL);
        }
        break;
    case STOP_EJ:
        if (is_stop)
        {
            log_i("EJECT");
            is_stop = false;
            is_eject = true;
            if (!filesave)
                fileopen = true;
            // filesave = false;
            fileload = false;
        }
        else
        {
            log_i("STOP");
            is_stop = true;
            is_eject = false;
            fileopen = false;
            filesave = false;
            selectTapeKey(rewBtn, false);
            lv_obj_send_event(rewBtn, LV_EVENT_REFRESH, NULL);
            selectTapeKey(ffwBtn, false);
            lv_obj_send_event(ffwBtn, LV_EVENT_REFRESH, NULL);
            selectTapeKey(recBtn, false);
            lv_obj_send_event(recBtn, LV_EVENT_REFRESH, NULL);
            selectTapeKey(playBtn, false);
            lv_obj_send_event(playBtn, LV_EVENT_REFRESH, NULL);
            if (!fileload)
            {
                lv_label_set_text(file, "No File");
                lv_obj_send_event(file, LV_EVENT_REFRESH, NULL);
            }
        }
        is_record = false;
        is_play = false;
        is_rewind = false;
        is_advance = false;
        is_pause = false;
        break;
    case PAUSE:
        if (!is_pause)
        {
            log_i("PAUSE");
            is_pause = true;
            selectTapeKey(pauseBtn, true);
        }
        else
        {
            log_i("NO PAUSE");
            is_pause = false;
            selectTapeKey(pauseBtn, false);
        }
        lv_obj_send_event(pauseBtn, LV_EVENT_REFRESH, NULL);
        break;
    default:
        break;
    }
}

#endif