/**
 * @file file_save.h
 * @author Jordi Gauchía
 * @brief LVGL - File Save
 * @version 0.2
 * @date 2024-03
 */

/**
 * @brief File save Screen
 *
 */
static lv_obj_t *fileSave;

static void save_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_t *kb = (lv_obj_t *)lv_event_get_user_data(e);
    log_i("%d", code);
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

    //     is_mainscreen = true;
    // is_stop = true;
    // is_eject = false;
    // fileopen = false;
    // filesave = false;
    // lv_screen_load(mainScr);
    // lv_label_set_text(file, file_name);
    // lv_obj_send_event(file,LV_EVENT_REFRESH,NULL);
}

/**
 * @brief Create a file save object
 *
 */
void create_file_save()
{
    fileSave = lv_obj_create(NULL);

    lv_obj_t *keyboard = lv_keyboard_create(fileSave);
    lv_obj_t *textarea;
    textarea = lv_textarea_create(fileSave);
    lv_obj_align(textarea, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_set_size(textarea, lv_pct(90), 80);
    lv_obj_add_state(textarea, LV_STATE_FOCUSED);
    lv_obj_add_event_cb(textarea, save_event, LV_EVENT_ALL, fileSave);

    lv_keyboard_set_textarea(keyboard, textarea);
}
