/**
 * @file file_open.h
 * @author Jordi Gauchía
 * @brief LVGL - File Explorer
 * @version 0.1
 * @date 2024-03-01
 */

/**
 * @brief Notify Bar Screen
 *
 */
static lv_obj_t *fileExplorer;

/**
 * @brief fileExplorer events
 *
 * @param e
 */
static void get_file(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = (lv_obj_t *)lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        file_path = (char *)lv_file_explorer_get_current_path(obj);
        file_name = (char *)lv_file_explorer_get_selected_file_name(obj);
        log_i("PATH: %s , FILE: %s", file_path, file_name);
        is_mainscreen = true;
        is_stop = true;
        is_eject = false;
        fileopen = false;
        fileload = true;
        lv_screen_load(mainScr);
        lv_label_set_text(file, file_name);
        lv_obj_send_event(file,LV_EVENT_REFRESH,NULL);
    }
}

/**
 * @brief Create a file explorer screen
 *
 */
void create_file_explorer()
{

    fileExplorer = lv_file_explorer_create(NULL);
    lv_file_explorer_set_sort(fileExplorer, LV_EXPLORER_SORT_NONE);
    lv_file_explorer_open_dir(fileExplorer, "S:/");
    lv_obj_add_event_cb(fileExplorer, get_file, LV_EVENT_ALL, NULL);
}