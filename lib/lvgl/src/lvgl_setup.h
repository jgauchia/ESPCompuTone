/**
 * @file lvgl_setup.h
 * @author Jordi Gauchía
 * @brief LVGL Screen implementation
 * @version 0.3
 * @date 2024-04
 */

#ifndef LVGL_SETUP_H
#define LVGL_SETUP_H

#include <lvgl.h>
#include <tft.h>

static lv_obj_t *mainScreen;

#include <lv_sd_fs.hpp>
#include <lv_spiffs_fs.hpp>
#include <lv_funcs.h>
#include <button_bar.h>
#include <file_info.h>
#include <wifi_select.h>
#include <notify_bar.h>
#include <file_open.h>
#include <file_save.h>


/**
 * @brief Default display driver definition
 *
 */

static lv_display_t *display;
static lv_indev_t *indev_drv;
#define DRAW_BUF_SIZE (TFT_WIDTH * TFT_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))
static uint32_t drawBuf[DRAW_BUF_SIZE / 4];


/**
 * @brief LVGL display update
 *
 */
static void dispFlush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushPixels((uint16_t *)px_map, w * h, true);
    tft.endWrite();
    lv_display_flush_ready(disp);
}

/**
 * @brief LVGL touch read
 *
 */
static void touchRead(lv_indev_t *indev_driver, lv_indev_data_t *data)
{
    uint16_t touchX, touchY;
    bool touched = tft.getTouch(&touchX, &touchY);
    if (!touched)
        data->state = LV_INDEV_STATE_RELEASED;
    else
    {
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = touchX;
        data->point.y = touchY;
    }
}

/**
 * @brief Init LVGL
 *
 */
static void initLVGL()
{
    lv_init();

    filesystemSDInit();
    filesystemSPIFFSInit();

    display = lv_display_create(TFT_WIDTH, TFT_HEIGHT);
    lv_display_set_flush_cb(display, dispFlush);
    lv_display_set_buffers(display, drawBuf, NULL, sizeof(drawBuf), LV_DISPLAY_RENDER_MODE_PARTIAL);

    indev_drv = lv_indev_create();
    lv_indev_set_type(indev_drv, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_drv, touchRead);

    mainScreen = lv_obj_create(NULL);
    lv_obj_set_size(mainScreen, TFT_WIDTH, TFT_HEIGHT);

    createNotifyBar();
    createWifiScreen();
    createButtonBar();
    createFileExplorerScreen();
    createFileSaveScreen();
    createFileInfoBar();

    lv_screen_load(mainScreen);
}

#endif