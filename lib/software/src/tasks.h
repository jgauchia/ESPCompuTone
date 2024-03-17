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
#include <events.h>

WAV audioWAV;

/**
 * @brief Keys Read task
 *
 * @param pvParameters
 */
void Keys_task(void *pvParameters)
{
    log_i("Task running on core %d", xPortGetCoreID());
    for (;;)
    {
        if (keys_delay.update())
            Check_keys();
        delay(1);
    }
}

void Audio_task(void *pvParameters)
{
    bool err = false;
    log_i("Task running on core %d", xPortGetCoreID());
    for (;;)
    {
        if (is_play)
        {
            char file_info[LV_FILE_EXPLORER_PATH_MAX_LEN];
            strcpy(file_info, "/sdcard");
            strcat(file_info, file_path + 3);
            strcat(file_info, file_name);
            log_i("%s", file_info);

            err = audioWAV.play(file_info, TapeEvent);
            // audioWAV.play("/sdcard/temp.wav", KeyEvent);

            if (TapeEvent() == tapeEvent::STOP || !err)
            {
                is_stop = true;
                is_play = false;
            }
            if (err)
            {
                lv_label_set_text(file, "Playback Error");
                lv_obj_send_event(file, LV_EVENT_REFRESH, NULL);
            }
        }
        if (is_record)
        {
            err = audioWAV.rec("/sdcard/temp.wav", sample_rate, num_channels, bit_depth, TapeEvent);
            if (TapeEvent() == tapeEvent::STOP)
            {
                is_record = false;
                is_stop = true;
                filesave = true;
            }
            if (err)
            {
                lv_label_set_text(file, "Recording Error");
                lv_obj_send_event(file, LV_EVENT_REFRESH, NULL);
            }
        }
        delay(1);
    }
}
/**
 * @brief Init Core tasks
 *
 */
void init_tasks()
{
    xTaskCreatePinnedToCore(Keys_task, "Keys Read", 16384, NULL, 1, NULL, 1);
    delay(500);
    xTaskCreatePinnedToCore(Audio_task, "Play Audio", 16384, NULL, 1, NULL, 1);
    delay(500);
}
#endif