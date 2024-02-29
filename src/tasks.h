/**
 * @file tasks.h
 * @author Jordi Gauchía
 * @brief Core Tasks functions
 * @version 0.1
 * @date 2024-02-29
 *
 * @copyright Copyright (c) 2024
 *
 */

/**
 * @brief Keys Read task
 *
 * @param pvParameters
 */
void Keys_task(void *pvParameters)
{
    log_i("Task running on core %d", xPortGetCoreID());
    for (;;)
    {
        if (keys_delay.update())
            Check_keys();
        delay(1);
    }
}

/**
 * @brief Init Core tasks
 *
 */
void init_tasks()
{
    xTaskCreatePinnedToCore(Keys_task, "Keys Read", 16384, NULL, 1, NULL, 1);
    delay(500);
}