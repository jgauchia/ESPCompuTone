/**
 * @file notify_bar.h
 * @author Jordi Gauchía
 * @brief LVGL - Notify Bar
 * @version 0.3
 * @date 2024-03
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

    // IP
    ip = lv_label_create(notifyBar);
    lv_label_set_text_fmt(ip, "%s",WiFi.localIP().toString());
        
    // Wifi Icon
    wifiBtn = lv_label_create(notifyBar);
    lv_label_set_text(wifiBtn, LV_SYMBOL_WIFI);
    lv_obj_add_flag(wifiBtn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(wifiBtn, wifi_event, LV_EVENT_ALL, NULL);

    // SD Icon
    sdcard = lv_label_create(notifyBar);
    if (sdloaded)
        lv_label_set_text_static(sdcard, LV_SYMBOL_SD_CARD);
    else
        lv_label_set_text_static(sdcard, " ");
}