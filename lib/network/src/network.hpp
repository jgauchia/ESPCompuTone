/**
 * @file network.hpp
 * @author Jordi Gauchía
 * @brief Network tasks
 * @version 0.3
 * @date 2024-03
 */

#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "WiFi.h"
#include <vector>
#include <vars.h>

extern String ssidName, ssidPW;
extern TaskHandle_t ntScanTaskHandler, ntConnectTaskHandler;
extern std::vector<String> foundWifiList;

void scanWIFITask(void *pvParameters);
void networkScanner();
void beginWIFITask(void *pvParameters);
void networkConnector();

#endif