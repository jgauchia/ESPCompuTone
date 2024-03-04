/**
 * @file lvgl_setup.h
 * @author Jordi Gauchía
 * @brief LVGL Screen implementation
 * @version 0.1
 * @date 2024-02-29
 */

#include <lvgl.h>

static lv_obj_t *mainScr;

#include <lv_sd_fs.h>
#include <lv_spiffs_fs.h>
#include <lv_funcs.h>
#include <button_bar.h>
#include <notify_bar.h>
#include <file_info.h>
#include <file_open.h>


/**
 * @brief Default display driver definition
 *
 */

static lv_display_t *display;
static lv_indev_t *indev_drv;
#define DRAW_BUF_SIZE (TFT_WIDTH * TFT_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

/**
 * @brief LVGL display update
 *
 */
void disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
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
void touchpad_read(lv_indev_t *indev_driver, lv_indev_data_t *data)
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
void init_LVGL()
{
    lv_init();

    lv_port_sd_fs_init();
    lv_port_spiffs_fs_init();

    display = lv_display_create(TFT_WIDTH, TFT_HEIGHT);
    lv_display_set_flush_cb(display, disp_flush);
    lv_display_set_buffers(display, draw_buf, NULL, sizeof(draw_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

    indev_drv = lv_indev_create();
    lv_indev_set_type(indev_drv, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_drv, touchpad_read);

    mainScr = lv_obj_create(NULL);
    lv_obj_set_size(mainScr, TFT_WIDTH, TFT_HEIGHT);

    create_notify_bar();
    create_button_bar_scr();
    create_file_explorer();
    create_file_info_bar();

    lv_screen_load(mainScr);
}
