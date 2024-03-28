/**
 * @file file_info.h
 * @author Jordi Gauchía
 * @brief LVGL - File info
 * @version 0.3
 * @date 2024-03
 */

#ifndef FILE_INFO_H
#define FILE_INFO_H

/**
 * @brief File Info Bar Screen
 *
 */
static lv_obj_t *fileBar;

/**
 * @brief File name label
 * 
 */
static lv_obj_t *file;

/**
 * @brief Create a file info bar
 *
 */
static void createFileInfoBar()
{
    fileBar = lv_obj_create(mainScreen);
    lv_obj_set_size(fileBar, 256, 50);
    lv_obj_set_pos(fileBar, xOffset, yOffset - 5);
    lv_obj_clear_flag(fileBar, LV_OBJ_FLAG_SCROLLABLE);
    static lv_style_t styleFilebar;
    lv_style_init(&styleFilebar);
    lv_style_set_bg_color(&styleFilebar, lv_color_white());
    lv_style_set_bg_opa(&styleFilebar, LV_OPA_80);
    lv_style_set_border_opa(&styleFilebar, LV_OPA_0);
    lv_obj_add_style(fileBar, &styleFilebar, LV_PART_MAIN);

    lv_obj_t *sideA = lv_label_create(fileBar);
    lv_obj_set_size(sideA, 20, 20);
    lv_obj_set_pos(sideA, -5, -5);
    lv_label_set_text_static(sideA, "A");
    lv_obj_set_style_text_align(sideA, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(sideA, &lv_font_montserrat_20, 0);
    static lv_style_t styleSideA;
    lv_style_init(&styleSideA);
    lv_style_set_bg_color(&styleSideA, lv_color_black());
    lv_style_set_bg_opa(&styleSideA, LV_OPA_100);
    lv_style_set_border_opa(&styleSideA, LV_OPA_0);
    lv_obj_add_style(sideA, &styleSideA, LV_PART_MAIN);

    file = lv_label_create(fileBar);
    lv_obj_set_size(file, 200, 30);
    lv_obj_set_pos(file, 25, -10);
    lv_obj_set_style_text_color(file,lv_color_black(),0);
    lv_label_set_text(file, "No File");
    lv_label_set_long_mode(file, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_style_text_align(file, LV_TEXT_ALIGN_CENTER, 0);
    static lv_style_t styleFile;
    lv_style_init(&styleFile);
    lv_style_set_bg_opa(&styleFile, LV_OPA_0);
    lv_style_set_border_color(&styleFile, lv_color_black());
    lv_style_set_border_width(&styleFile, 1);
    lv_style_set_border_opa(&styleFile, LV_OPA_100);
    lv_obj_add_style(file, &styleFile, LV_PART_MAIN);
}
#endif