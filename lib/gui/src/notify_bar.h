/**
 * @file notify_bar.h
 * @author Jordi Gauchía
 * @brief LVGL - Notify Bar
 * @version 0.1
 * @date 2024-02-29
 */

/**
 * @brief Notify Bar Screen
 *
 */
static lv_obj_t *notifyBar;

/**
 * @brief Notify Bar screen objects
 *
 */
static lv_obj_t *sdcard;

/**
 * @brief Create a notify bar
 *
 */
void create_notify_bar()
{
    notifyBar = lv_obj_create(mainScr);
    lv_obj_set_size(notifyBar, TFT_WIDTH, 22);
    lv_obj_set_pos(notifyBar, 0, 0);
    lv_obj_set_flex_flow(notifyBar, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(notifyBar, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(notifyBar, LV_OBJ_FLAG_SCROLLABLE);
    static lv_style_t style_bar;
    lv_style_init(&style_bar);
    lv_style_set_bg_opa(&style_bar, LV_OPA_0);
    lv_style_set_border_opa(&style_bar, LV_OPA_0);
    lv_obj_add_style(notifyBar, &style_bar, LV_PART_MAIN);

    lv_obj_t *label;

    label = lv_label_create(notifyBar);
    if (sdloaded)
        lv_label_set_text_static(label, LV_SYMBOL_SD_CARD);
    else
        lv_label_set_text_static(label, " ");
}