/**
 * @file network.cpp
 * @author Jordi Gauchía
 * @brief Network tasks
 * @version 0.3
 * @date 2024-03
 */


#include <network.hpp>

String ssidName, ssidPW;
TaskHandle_t ntScanTaskHandler, ntConnectTaskHandler;
std::vector<String> foundWifiList;

/**
 * @brief Network Tasks
 *
 */

/**
 * @brief Scan Wifi Task
 *
 * @param pvParameters
 */
void scanWIFITask(void *pvParameters)
{
    while (1)
    {
        foundWifiList.clear();
        int n = WiFi.scanNetworks();
        vTaskDelay(10);
        for (int i = 0; i < n; ++i)
        {
            String item = WiFi.SSID(i) + " (" + WiFi.RSSI(i) + ") " + ((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
            foundWifiList.push_back(item);
            vTaskDelay(10);
        }
        vTaskDelay(5000);
    }
}

void networkScanner()
{
    xTaskCreate(scanWIFITask, "ScanWIFITask", 4096, NULL, 1, &ntScanTaskHandler);
}

/**
 * @brief Connect Wifi Task
 *
 * @param pvParameters
 */
void beginWIFITask(void *pvParameters)
{

    unsigned long startingTime = millis();
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    vTaskDelay(100);

    log_i("%s", ssidName.c_str());
    WiFi.begin(ssidName.c_str(), ssidPW.c_str());

    while (WiFi.status() != WL_CONNECTED && (millis() - startingTime) < networkTimeout)
    {
        vTaskDelay(250);
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        networkStatus = NETWORK_CONNECTED_POPUP;
        log_i("%s", WiFi.localIP().toString());
        // saveWIFICredentialEEPROM(1, ssidName + " " + ssidPW);
    }
    else
    {
        networkStatus = NETWORK_CONNECT_FAILED;
        // saveWIFICredentialEEPROM(0, "");
    }

    vTaskDelete(NULL);
}

void networkConnector()
{
    xTaskCreate(beginWIFITask, "beginWIFITask", 2048, NULL, 1, &ntConnectTaskHandler);
}