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

extern bool isRecord;

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
MyDelay keysDelay(KEYS_UPDATE_TIME);


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
void checkKeys()
{
    key_pressed = Read_Keys();

    switch (key_pressed)
    {
    case PLAY:
        if (isStop && !isPause && fileLoad)
        {
            log_i("PLAY");
            isRecord = false;
            isPlay = true;
            isRewind = false;
            isAdvance = false;
            isStop = false;
            isEject = false;
            isPause = false;
            selectTapeKey(playBtn, true);
            lv_obj_send_event(playBtn, LV_EVENT_REFRESH, NULL);
        }
        break;
    case REC_PLAY:
        if (isStop && !isPause)
        {
            log_i("RECORD");
            isRecord = true;
            isPlay = false;
            isRewind = false;
            isAdvance = false;
            isStop = false;
            isEject = false;
            isPause = false;
            selectTapeKey(recBtn, true);
            lv_obj_send_event(recBtn, LV_EVENT_REFRESH, NULL);
            selectTapeKey(playBtn, true);
            lv_obj_send_event(playBtn, LV_EVENT_REFRESH, NULL);
            lv_label_set_text(file, "Audio Recording");
            lv_obj_send_event(file, LV_EVENT_REFRESH, NULL);
        }
        break;
    case REW:
        if (isStop && !isPause)
        {
            log_i("REWIND");
            isRecord = false;
            isPlay = false;
            isRewind = true;
            isAdvance = false;
            isStop = false;
            isEject = false;
            isPause = false;
            selectTapeKey(rewBtn, true);
            lv_obj_send_event(rewBtn, LV_EVENT_REFRESH, NULL);
        }
        break;
    case ADV:
        if (isStop && !isPause)
        {
            log_i("ADVANCE");
            isRecord = false;
            isPlay = false;
            isRewind = false;
            isAdvance = true;
            isStop = false;
            isEject = false;
            isPause = false;
            selectTapeKey(ffwBtn, true);
            lv_obj_send_event(ffwBtn, LV_EVENT_REFRESH, NULL);
        }
        break;
    case STOP_EJ:
        if (isStop)
        {
            log_i("EJECT");
            isStop = false;
            isEject = true;
            if (!fileSave)
                fileOpen = true;
            // fileSave = false;
            fileLoad = false;
        }
        else
        {
            log_i("STOP");
            isStop = true;
            isEject = false;
            fileOpen = false;
            fileSave = false;
            selectTapeKey(rewBtn, false);
            lv_obj_send_event(rewBtn, LV_EVENT_REFRESH, NULL);
            selectTapeKey(ffwBtn, false);
            lv_obj_send_event(ffwBtn, LV_EVENT_REFRESH, NULL);
            selectTapeKey(recBtn, false);
            lv_obj_send_event(recBtn, LV_EVENT_REFRESH, NULL);
            selectTapeKey(playBtn, false);
            lv_obj_send_event(playBtn, LV_EVENT_REFRESH, NULL);
            if (!fileLoad)
            {
                lv_label_set_text(file, "No File");
                lv_obj_send_event(file, LV_EVENT_REFRESH, NULL);
            }
        }
        isRecord = false;
        isPlay = false;
        isRewind = false;
        isAdvance = false;
        isPause = false;
        break;
    case PAUSE:
        if (!isPause)
        {
            log_i("PAUSE");
            isPause = true;
            selectTapeKey(pauseBtn, true);
        }
        else
        {
            log_i("NO PAUSE");
            isPause = false;
            selectTapeKey(pauseBtn, false);
        }
        lv_obj_send_event(pauseBtn, LV_EVENT_REFRESH, NULL);
        break;
    default:
        break;
    }
}

#endif