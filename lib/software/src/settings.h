/**
 * @file settings.h
 * @author Jordi Gauchía
 * @brief Settings 
 * @version 0.3.1
 * @date 2024-04
 */

#ifndef SETTINGS_H
#define SETTINGS_H 

#include <Preferences.h>

extern Preferences settings;
extern String wifiSSID;
extern String wifiPswd;

static void loadSettings()
{
    settings.begin("ESPCompuTone",false);
    wifiSSID = settings.getString("network_ssid","");
    wifiPswd = settings.getString("network_pswd","");
    settings.end();
}

#endif