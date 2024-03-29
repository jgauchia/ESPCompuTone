/**
 * @file tasks.h
 * @author Jordi Gauchía
 * @brief Core Tasks functions
* @version 0.2
 * @date 2024-03
 *
 * @copyright Copyright (c) 2024
 *
 */

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
            play_wav(file_info);
            //play_wav("/sdcard/temp.wav");
            select_obj(playBtn, false);
            lv_obj_send_event(playBtn, LV_EVENT_REFRESH, NULL);
        }
        if (is_record)
        {
            rec_wav("/sdcard/temp.wav",sample_rate,num_channels,bit_depth);      
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