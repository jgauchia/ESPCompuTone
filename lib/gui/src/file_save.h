/**
 * @file file_save.h
 * @author Jordi Gauchía
 * @brief LVGL - File Save
 * @version 0.3.1
 * @date 2024-04
 */

#ifndef FILE_SAVE_H
#define FILE_SAVE_H

/**
 * @brief File save Screen
 *
 */
static lv_obj_t *fileSaveScreen;
static lv_obj_t *textArea;

/**
 * @brief File Save event
 *
 * @param e
 */
static void saveEvent(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_t *kb = (lv_obj_t *)lv_event_get_user_data(e);

    if (code == LV_EVENT_FOCUSED)
    {
        lv_keyboard_set_textarea(kb, ta);
        lv_obj_remove_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }

    if (code == LV_EVENT_DEFOCUSED)
    {
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }

    if (code == LV_EVENT_READY)
    {
        fileName = (char *)lv_textarea_get_text(ta);
        if (fileName != NULL)
        {
            char fileInfo[LV_FILE_EXPLORER_PATH_MAX_LEN];
            strcpy(fileInfo, "/sdcard/");
            strcat(fileName, ".wav");
            strcat(fileInfo, fileName);
            isMainScreen = true;
            isStop = true;
            isEject = false;
            isPlay = false;
            fileOpen = false;
            fileSave = false;
            rename("/sdcard/temp.wav", fileInfo);
            lv_file_explorer_open_dir(fileExplorer, "S:/");
            lv_label_set_text(file, fileName);
            lv_obj_send_event(file, LV_EVENT_REFRESH, NULL);
            lv_screen_load(mainScreen);
        }
    }

    if (code == LV_EVENT_CANCEL)
    {
        isMainScreen = true;
        isStop = true;
        isEject = false;
        isPlay = false;
        fileOpen = false;
        fileSave = false;
        remove("/sdcard/temp.wav");
        lv_file_explorer_open_dir(fileExplorer, "S:/");
        lv_screen_load(mainScreen);
    }
}

/**
 * @brief Create a file save object
 *
 */
static void createFileSaveScreen()
{
    fileSaveScreen = lv_obj_create(NULL);

    lv_obj_t *keyboard = lv_keyboard_create(fileSaveScreen);
    textArea = lv_textarea_create(fileSaveScreen);
    lv_obj_align(textArea, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_set_size(textArea, lv_pct(90), 80);
    lv_obj_add_state(textArea, LV_STATE_FOCUSED);
    lv_obj_add_event_cb(textArea, saveEvent, LV_EVENT_ALL, fileSaveScreen);

    lv_keyboard_set_textarea(keyboard, textArea);
}
#endif
