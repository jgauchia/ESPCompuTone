/**
 * @file notify_bar.h
 * @author Jordi Gauchía
 * @brief LVGL - Notify Bar
 * @version 0.3.1
 * @date 2024-04
 */

#ifndef NOTIFY_BAR_H
#define NOTIFY_BAR_H

#define UPDATE_NOTIFY_PERIOD 3000

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
static lv_obj_t *battery;

/**
 * @brief Update notify bar info timer
 *
 */
void updateNotifyBar(lv_timer_t *t)
{
    battLevel = batteryRead();
    if (battLevel != battLevelOld)
    {
        lv_obj_send_event(battery, LV_EVENT_VALUE_CHANGED, NULL);
        battLevelOld = battLevel;
    }
}

/**
 * @brief Battery update event
 *
 */
static void updateBatt(lv_event_t *event)
{
    if (battLevel <= 160 && battLevel > 125)
        lv_label_set_text_static(battery, "  " LV_SYMBOL_CHARGE);
    else if (battLevel <= 125 && battLevel > 80)
        lv_label_set_text_static(battery, LV_SYMBOL_BATTERY_FULL);
    else if (battLevel <= 80 && battLevel > 60)
        lv_label_set_text_static(battery, LV_SYMBOL_BATTERY_3);
    else if (battLevel <= 60 && battLevel > 40)
        lv_label_set_text_static(battery, LV_SYMBOL_BATTERY_2);
    else if (battLevel <= 40 && battLevel > 20)
        lv_label_set_text_static(battery, LV_SYMBOL_BATTERY_1);
    else if (battLevel <= 20)
        lv_label_set_text(battery, LV_SYMBOL_BATTERY_EMPTY);
}


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
    lv_timer_t *timerNotifybar = lv_timer_create(updateNotifyBar, UPDATE_NOTIFY_PERIOD, NULL);
    lv_timer_ready(timerNotifybar);

    // IP
    ip = lv_label_create(notifyBar);
    lv_label_set_text_fmt(ip, "%s", WiFi.localIP().toString());

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

    // Battery Icon
    battery = lv_label_create(notifyBar);
    lv_label_set_text_static(battery, LV_SYMBOL_BATTERY_EMPTY);
    lv_obj_add_event_cb(battery, updateBatt, LV_EVENT_VALUE_CHANGED, NULL);
}

#endif