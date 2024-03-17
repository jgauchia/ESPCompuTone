/**
 * @file wifi_select.h
 * @author Jordi Gauchía
 * @brief LVGL Wifi selection screen
 * @version 0.3
 * @date 2024-03
 */

#ifndef WIFI_SELECT_H
#define WIFI_SELECT_H

#include <network.h>

static lv_obj_t *ip;
static lv_obj_t *wifiBtn;
static lv_obj_t *wifiSettings;
static lv_obj_t *wifiSettingCloseBtn;
static lv_obj_t *wifiSettingSwitch;
static lv_obj_t *wifiList;
static lv_obj_t *settinglabel;
static lv_obj_t *mboxConnect;
static lv_obj_t *mboxTitle;
static lv_obj_t *mboxPassword;
static lv_obj_t *mboxConnectBtn;
static lv_obj_t *mboxCloseBtn;
static lv_obj_t *keyboard;
static lv_obj_t *popupBox;
static lv_obj_t *popupBoxCloseBtn;
static lv_timer_t *timer;

/**
 * @brief Wifi List selection event
 *
 * @param e
 */
static void list_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = (lv_obj_t *)lv_event_get_target(e);
    uint32_t row;
    uint32_t col;

    if (code == LV_EVENT_LONG_PRESSED)
    {
        lv_table_get_selected_cell(obj, &row, &col);
        String selectedItem = String(lv_table_get_cell_value(obj, row, col));
        for (int i = 0; i < selectedItem.length() - 1; i++)
        {
            if (selectedItem.substring(i, i + 2) == " (")
            {
                ssidName = selectedItem.substring(5, i);
                lv_label_set_text_fmt(mboxTitle, "Selected WiFi SSID:\n%s", ssidName.c_str());
                lv_obj_move_foreground(mboxConnect);
                break;
            }
        }
        lv_obj_clear_flag(mboxConnect, LV_OBJ_FLAG_HIDDEN);
    }
}

/**
 * @brief Password input event
 *
 * @param e
 */
static void text_input_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta = (lv_obj_t *)lv_event_get_target(e);

    if (code == LV_EVENT_FOCUSED)
    {
        lv_obj_move_foreground(keyboard);
        lv_keyboard_set_textarea(keyboard, ta);
        lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    }

    if (code == LV_EVENT_DEFOCUSED)
    {
        lv_keyboard_set_textarea(keyboard, NULL);
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    }
}

static void makeKeyboard()
{
    keyboard = lv_keyboard_create(lv_scr_act());
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
}
/**
 * @brief Show Wifi List
 *
 */
static void showingFoundWiFiList()
{
    if (foundWifiList.size() == 0 || foundNetworks == foundWifiList.size())
        return;

    lv_table_clear(wifiList);
    totalWificount = 0;
    for (std::vector<String>::iterator item = foundWifiList.begin(); item != foundWifiList.end(); ++item)
    {
        lv_table_set_cell_value_fmt(wifiList, totalWificount, 0, LV_SYMBOL_WIFI "  %s", (*item).c_str());
        totalWificount++;
        delay(1);
    }

    foundNetworks = foundWifiList.size();
}

/**
 * @brief Timer for Network search
 *
 * @param timer
 */
static void timerForNetwork(lv_timer_t *timer)
{
    LV_UNUSED(timer);

    switch (networkStatus)
    {

    case NETWORK_SEARCHING:
        showingFoundWiFiList();
        break;

    case NETWORK_CONNECTED_POPUP:
        // popupMsgBox("WiFi Connected!", "Now you'll get the current time soon.");
        networkStatus = NETWORK_CONNECTED;
        lv_label_set_text_fmt(ip, "%s", WiFi.localIP().toString());
        lv_obj_send_event(ip, LV_EVENT_REFRESH, NULL);
        break;

    case NETWORK_CONNECTED:
        showingFoundWiFiList();
        break;

    case NETWORK_CONNECT_FAILED:
        networkStatus = NETWORK_SEARCHING;
        // popupMsgBox("Oops!", "Please check your wifi password and try again.");
        break;

    default:
        break;
    }
}

/**
 * @brief Wifi settings events
 *
 * @param e
 */
static void wifi_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);

    if (isStop)
    {
        if (code == LV_EVENT_CLICKED)
        {
            if (btn == wifiBtn)
            {
                // Open Wifi settings label
                isConfig = true;
                lv_obj_clear_flag(wifiSettings, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(fileBar, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(buttonBar, LV_OBJ_FLAG_HIDDEN);
            }
            else if (btn == wifiSettingCloseBtn)
            {
                // Close Wifi settings label
                isConfig = false;
                lv_obj_add_flag(wifiSettings, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(mboxConnect, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(fileBar, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(buttonBar, LV_OBJ_FLAG_HIDDEN);
            }
            else if (btn == mboxConnectBtn)
            {
                ssidPW = String(lv_textarea_get_text(mboxPassword));

                networkConnector();
                lv_obj_move_background(mboxConnect);
                // popupMsgBox("Connecting!", "Attempting to connect to the selected network.");
            }
            else if (btn == mboxCloseBtn)
            {
                lv_obj_move_background(mboxConnect);
            }
            else if (btn == popupBoxCloseBtn)
            {
                lv_obj_move_background(popupBox);
            }
        }
        else if (code == LV_EVENT_VALUE_CHANGED)
        {
            if (btn == wifiSettingSwitch)
            {

                if (lv_obj_has_state(btn, LV_STATE_CHECKED))
                {

                    if (ntScanTaskHandler == NULL)
                    {
                        networkStatus = NETWORK_SEARCHING;
                        networkScanner();
                        timer = lv_timer_create(timerForNetwork, 1000, wifiList);
                    }
                }
                else
                {
                    if (ntScanTaskHandler != NULL)
                    {
                        networkStatus = NONE;
                        vTaskDelete(ntScanTaskHandler);
                        ntScanTaskHandler = NULL;
                        lv_timer_del(timer);
                        lv_table_clear(wifiList);
                    }

                    if (WiFi.status() == WL_CONNECTED)
                    {
                        WiFi.disconnect(true);
                        lv_label_set_text_fmt(ip, "%s", WiFi.localIP().toString());
                        lv_obj_send_event(ip, LV_EVENT_REFRESH, NULL);
                    }
                }
            }
        }
    }
}

/**
 * @brief Create wifi settings screen
 *
 */
static void create_wifi_screen()
{
    wifiSettings = lv_obj_create(mainScr);
    lv_obj_set_size(wifiSettings, tft.width(), tft.height() - 35);
    lv_obj_align(wifiSettings, LV_ALIGN_TOP_RIGHT, 0, 25);
    lv_obj_add_flag(wifiSettings, LV_OBJ_FLAG_HIDDEN);

    settinglabel = lv_label_create(wifiSettings);
    lv_label_set_text(settinglabel, LV_SYMBOL_POWER " Wifi ");
    lv_obj_align(settinglabel, LV_ALIGN_TOP_LEFT, 0, 0);

    wifiSettingCloseBtn = lv_label_create(wifiSettings);
    lv_obj_align(wifiSettingCloseBtn, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_add_flag(wifiSettingCloseBtn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(wifiSettingCloseBtn, wifi_event, LV_EVENT_ALL, NULL);
    lv_label_set_text(wifiSettingCloseBtn, LV_SYMBOL_CLOSE);

    wifiSettingSwitch = lv_switch_create(wifiSettings);
    lv_obj_add_event_cb(wifiSettingSwitch, wifi_event, LV_EVENT_ALL, NULL);
    lv_obj_align_to(wifiSettingSwitch, settinglabel, LV_ALIGN_TOP_RIGHT, 60, -10);

    wifiList = lv_table_create(wifiSettings);
    lv_obj_set_size(wifiList, tft.width() - 30, tft.height() - 95);
    lv_table_set_column_width(wifiList, 0, tft.width());
    lv_obj_set_scroll_dir(wifiList, LV_DIR_TOP | LV_DIR_BOTTOM);
    lv_obj_align_to(wifiList, wifiSettings, LV_ALIGN_TOP_LEFT, 0, 30);
    lv_obj_add_event_cb(wifiList, list_event_handler, LV_EVENT_ALL, NULL);

    mboxConnect = lv_obj_create(mainScr);
    lv_obj_set_size(mboxConnect, tft.width(), tft.height() - 35);
    lv_obj_align(mboxConnect, LV_ALIGN_TOP_RIGHT, 0, 25);

    mboxTitle = lv_label_create(mboxConnect);
    lv_label_set_text(mboxTitle, "Selected WiFi SSID:");
    lv_obj_align(mboxTitle, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_add_flag(mboxConnect, LV_OBJ_FLAG_HIDDEN);

    mboxPassword = lv_textarea_create(mboxConnect);
    lv_obj_set_size(mboxPassword, tft.width() - 60, 40);
    lv_obj_align_to(mboxPassword, mboxTitle, LV_ALIGN_TOP_LEFT, 0, 30);
    lv_textarea_set_placeholder_text(mboxPassword, "Password?");
    lv_obj_add_event_cb(mboxPassword, text_input_event_cb, LV_EVENT_ALL, keyboard);

    mboxConnectBtn = lv_btn_create(mboxConnect);
    lv_obj_add_event_cb(mboxConnectBtn, wifi_event, LV_EVENT_ALL, NULL);
    lv_obj_align(mboxConnectBtn, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_t *btnLabel = lv_label_create(mboxConnectBtn);
    lv_label_set_text(btnLabel, "Connect");
    lv_obj_center(btnLabel);

    mboxCloseBtn = lv_btn_create(mboxConnect);
    lv_obj_add_event_cb(mboxCloseBtn, wifi_event, LV_EVENT_ALL, NULL);
    lv_obj_align(mboxCloseBtn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_obj_t *btnLabel2 = lv_label_create(mboxCloseBtn);
    lv_label_set_text(btnLabel2, "Cancel");
    lv_obj_center(btnLabel2);

    keyboard = lv_keyboard_create(mboxConnect);
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
}

#endif