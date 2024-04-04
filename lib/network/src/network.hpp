/**
 * @file network.hpp
 * @author Jordi Gauchía
 * @brief Network tasks
 * @version 0.3
 * @date 2024-04
 */

#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "WiFi.h"
#include <vector>
#include <vars.h>
#include <settings.h>

extern String ssidName, ssidPW;
extern TaskHandle_t ntScanTaskHandler, ntConnectTaskHandler;
extern std::vector<String> foundWifiList;
extern Preferences settings;
extern String wifiSSID;
extern String wifiPswd;

void scanWIFITask(void *pvParameters);
void networkScanner();
void beginWIFITask(void *pvParameters);
void networkConnector();

#endif