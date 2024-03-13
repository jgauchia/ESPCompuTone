/**
 * @file wifi_select.h
 * @author Jordi Gauchía
 * @brief LVGL Wifi selection screen
 * @version 0.2
 * @date 2024-03
 */

static lv_obj_t *settings;
static lv_obj_t *wifiBtn;
static lv_obj_t *settingCloseBtn;
static lv_obj_t *settingWiFiSwitch;
static lv_obj_t *wfList;
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

static void list_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = (lv_obj_t *)lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {

        String selectedItem = String(lv_list_get_btn_text(wfList, obj));
        for (int i = 0; i < selectedItem.length() - 1; i++)
        {
            if (selectedItem.substring(i, i + 2) == " (")
            {
                ssidName = selectedItem.substring(0, i);
                lv_label_set_text_fmt(mboxTitle, "Selected WiFi SSID: %s", ssidName);
                lv_obj_move_foreground(mboxConnect);
                break;
            }
        }
    }
}

/**
 * @brief SHow Wifi List
 *
 */
static void showingFoundWiFiList()
{
    if (foundWifiList.size() == 0 || foundNetworks == foundWifiList.size())
        return;

    lv_obj_clean(wfList);
    int cont = 0;
    for (std::vector<String>::iterator item = foundWifiList.begin(); item != foundWifiList.end(); ++item)
    {
        // lv_obj_t *btn = lv_list_add_btn(wfList, LV_SYMBOL_WIFI, (*item).c_str());
        // lv_obj_add_event_cb(btn, list_event_handler, LV_EVENT_CLICKED, NULL);
        lv_table_set_cell_value_fmt(wfList, cont, 0, LV_SYMBOL_WIFI "  %s", (*item).c_str());
        cont++;
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

    if (is_stop)
    {
        if (code == LV_EVENT_CLICKED)
        {
            if (btn == wifiBtn)
            {
                // Open Wifi settings label
                is_config = true;
                lv_obj_clear_flag(settings, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(fileBar, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(buttonBar, LV_OBJ_FLAG_HIDDEN);
            }
            else if (btn == settingCloseBtn)
            {
                // Close Wifi settings label
                is_config = false;
                lv_obj_add_flag(settings, LV_OBJ_FLAG_HIDDEN);
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
            if (btn == settingWiFiSwitch)
            {

                if (lv_obj_has_state(btn, LV_STATE_CHECKED))
                {

                    if (ntScanTaskHandler == NULL)
                    {
                        networkStatus = NETWORK_SEARCHING;
                        networkScanner();
                        timer = lv_timer_create(timerForNetwork, 1000, wfList);
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
                        lv_obj_clean(wfList);
                    }

                    if (WiFi.status() == WL_CONNECTED)
                    {
                        WiFi.disconnect(true);
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
    settings = lv_obj_create(mainScr);
    lv_obj_set_size(settings, tft.width(), tft.height() - 35);
    lv_obj_align(settings, LV_ALIGN_TOP_RIGHT, 0, 25);
    lv_obj_add_flag(settings, LV_OBJ_FLAG_HIDDEN);

    settinglabel = lv_label_create(settings);
    lv_label_set_text(settinglabel, LV_SYMBOL_POWER " Wifi ");
    lv_obj_align(settinglabel, LV_ALIGN_TOP_LEFT, 0, 0);

    settingCloseBtn = lv_label_create(settings);
    lv_obj_align(settingCloseBtn, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_add_flag(settingCloseBtn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(settingCloseBtn, wifi_event, LV_EVENT_ALL, NULL);
    lv_label_set_text(settingCloseBtn, LV_SYMBOL_CLOSE);

    settingWiFiSwitch = lv_switch_create(settings);
    lv_obj_add_event_cb(settingWiFiSwitch, wifi_event, LV_EVENT_ALL, NULL);
    lv_obj_align_to(settingWiFiSwitch, settinglabel, LV_ALIGN_TOP_RIGHT, 60, -10);

    wfList = lv_table_create(settings);
    lv_obj_set_size(wfList, tft.width() - 30, tft.height() - 95);
    lv_table_set_column_width(wfList, 0, tft.width());
    lv_obj_set_flex_flow(wfList, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_scroll_dir(wfList, LV_DIR_TOP | LV_DIR_BOTTOM);
    lv_table_add_cell_ctrl(wfList, 0, 0, LV_TABLE_CELL_CTRL_TEXT_CROP);
    lv_obj_align_to(wfList, settings, LV_ALIGN_TOP_LEFT, 0, 30);
}