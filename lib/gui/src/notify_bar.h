/**
 * @file notify_bar.h
 * @author Jordi Gauchía
 * @brief LVGL - Notify Bar
 * @version 0.3.1
 * @date 2024-04
 */

#ifndef NOTIFY_BAR_H
#define NOTIFY_BAR_H

/**
 * @brief Notify Bar Screen
 *
 */
static lv_obj_t *notifyBar;

/**
 * @brief Notify Bar screen objects
 *
 */
static lv_obj_t *sdCard;

/**
 * @brief Create a notify bar
 *
 */
static void createNotifyBar()
{
    notifyBar = lv_obj_create(mainScreen);
    lv_obj_set_size(notifyBar, TFT_WIDTH, 22);
    lv_obj_set_pos(notifyBar, 0, 0);
    lv_obj_set_flex_flow(notifyBar, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(notifyBar, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(notifyBar, LV_OBJ_FLAG_SCROLLABLE);
    static lv_style_t styleBar;
    lv_style_init(&styleBar);
    lv_style_set_bg_opa(&styleBar, LV_OPA_0);
    lv_style_set_border_opa(&styleBar, LV_OPA_0);
    lv_obj_add_style(notifyBar, &styleBar, LV_PART_MAIN);

    // IP
    ip = lv_label_create(notifyBar);
    lv_label_set_text_fmt(ip, "%s",WiFi.localIP().toString());
        
    // Wifi Icon
    wifiBtn = lv_label_create(notifyBar);
    lv_label_set_text(wifiBtn, LV_SYMBOL_WIFI);
    lv_obj_add_flag(wifiBtn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(wifiBtn, wifiEvent, LV_EVENT_ALL, NULL);

    // SD Icon
    sdCard = lv_label_create(notifyBar);
    if (sdLoaded)
        lv_label_set_text_static(sdCard, LV_SYMBOL_SD_CARD);
    else
        lv_label_set_text_static(sdCard, " ");
}

#endif