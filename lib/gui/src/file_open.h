/**
 * @file file_open.h
 * @author Jordi Gauchía
 * @brief LVGL - File Explorer
 * @version 0.3.1
 * @date 2024-04
 */

#ifndef FILE_OPEN_H
#define FILE_OPEN_H

/**
 * @brief File explorer Screen
 *
 */
static lv_obj_t *fileExplorer;

/**
 * @brief fileExplorer events
 *
 * @param e
 */
static void getFileEvent(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = (lv_obj_t *)lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        filePath = (char *)lv_file_explorer_get_current_path(obj);
        fileName = (char *)lv_file_explorer_get_selected_file_name(obj);
        log_i("PATH: %s , FILE: %s", filePath, fileName);
        isMainScreen= true;
        isStop = true;
        isEject = false;
        fileOpen = false;
        fileLoad = true;
        isDirRefresh = false;
        lv_screen_load(mainScreen);
        lv_label_set_text(file, fileName);
        lv_obj_send_event(file,LV_EVENT_REFRESH,NULL);
    }
}

/**
 * @brief Create a file explorer screen
 *
 */
static void createFileExplorerScreen()
{
    fileExplorer = lv_file_explorer_create(NULL);
    lv_file_explorer_set_sort(fileExplorer, LV_EXPLORER_SORT_NONE);
    lv_file_explorer_open_dir(fileExplorer, "S:/");
    lv_obj_add_event_cb(fileExplorer, getFileEvent, LV_EVENT_ALL, NULL);
}

#endif