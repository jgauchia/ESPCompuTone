/**
 * @file tasks.h
 * @author Jordi Gauchía
 * @brief Core Tasks functions
 * @version 0.3
 * @date 2024-03
 */

#ifndef TASKS_H
#define TASKS_H

#include <wav.hpp>

extern bool isRecord;

WAV audioWAV;

/**
 * @brief Keys Read task
 *
 * @param pvParameters
 */
void keysTask(void *pvParameters)
{
    log_i("Task running on core %d", xPortGetCoreID());
    for (;;)
    {
        if (keysDelay.update())
            checkKeys();
        delay(1);
    }
}

void audioTask(void *pvParameters)
{
    bool fileError = false;
    log_i("Task running on core %d", xPortGetCoreID());
    for (;;)
    {
        if (isPlay)
        {
            char fileInfo[LV_FILE_EXPLORER_PATH_MAX_LEN];
            strcpy(fileInfo, "/sdcard");
            strcat(fileInfo, filePath + 3);
            strcat(fileInfo, fileName);
            log_i("%s", fileInfo);

            fileError = audioWAV.play(fileInfo, getTapeEvent);

            if (getTapeEvent() == tapeEvent::STOP)
            {
                selectTapeKey(playBtn, false);
                lv_obj_send_event(playBtn, LV_EVENT_REFRESH, NULL);
                isStop = true;
                isPlay = false;
            }
            else if (!fileError)
            {
                selectTapeKey(playBtn, false);
                lv_obj_send_event(playBtn, LV_EVENT_REFRESH, NULL);
                isStop = true;
                isPlay = false;
            }
            else if (fileError)
            {
                isStop = true;
                isPlay = false;
                lv_label_set_text(file, "Playback Error");
                lv_obj_send_event(file, LV_EVENT_REFRESH, NULL);
            }
        }
        if (isRecord)
        {
            fileError = audioWAV.rec("/sdcard/temp.wav", sampleRate, numChannels, bitDepth, getTapeEvent);
            if (getTapeEvent() == tapeEvent::STOP)
            {
                isRecord = false;
                isStop = true;
                fileSave = true;
            }
            if (fileError)
            {
                isRecord = false;
                isStop = true;
                lv_label_set_text(file, "Recording Error");
                lv_obj_send_event(file, LV_EVENT_REFRESH, NULL);
            }
        }
        yield();
    }
}

/**
 * @brief Init Core tasks
 *
 */
void initTasks()
{
    xTaskCreatePinnedToCore(keysTask, "Keys Read Task", 2048, NULL, 1, NULL, 1);
    delay(500);
    xTaskCreatePinnedToCore(audioTask, "Audio Task", 16384, NULL, 1, NULL, 1);
    delay(500);
}

#endif