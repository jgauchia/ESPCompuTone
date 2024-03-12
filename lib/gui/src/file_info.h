/**
 * @file file_info.h
 * @author Jordi Gauchía
 * @brief LVGL - File info
* @version 0.2
 * @date 2024-03
 */

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
void create_file_info_bar()
{
    fileBar = lv_obj_create(mainScr);
    lv_obj_set_size(fileBar, 256, 50);
    lv_obj_set_pos(fileBar, xOffset, yOffset - 5);
    lv_obj_clear_flag(fileBar, LV_OBJ_FLAG_SCROLLABLE);
    static lv_style_t style_filebar;
    lv_style_init(&style_filebar);
    lv_style_set_bg_color(&style_filebar, lv_color_white());
    lv_style_set_bg_opa(&style_filebar, LV_OPA_80);
    lv_style_set_border_opa(&style_filebar, LV_OPA_0);
    lv_obj_add_style(fileBar, &style_filebar, LV_PART_MAIN);

    lv_obj_t *sideA = lv_label_create(fileBar);
    lv_obj_set_size(sideA, 20, 20);
    lv_obj_set_pos(sideA, -5, -5);
    lv_label_set_text_static(sideA, "A");
    lv_obj_set_style_text_align(sideA, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(sideA, &lv_font_montserrat_20, 0);
    static lv_style_t style_sideA;
    lv_style_init(&style_sideA);
    lv_style_set_bg_color(&style_sideA, lv_color_black());
    lv_style_set_bg_opa(&style_sideA, LV_OPA_100);
    lv_style_set_border_opa(&style_sideA, LV_OPA_0);
    lv_obj_add_style(sideA, &style_sideA, LV_PART_MAIN);

    file = lv_label_create(fileBar);
    lv_obj_set_size(file, 200, 30);
    lv_obj_set_pos(file, 25, -10);
    lv_obj_set_style_text_color(file,lv_color_black(),0);
    lv_label_set_text(file, "No File");
    lv_label_set_long_mode(file, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_style_text_align(file, LV_TEXT_ALIGN_CENTER, 0);
    static lv_style_t style_file;
    lv_style_init(&style_file);
    lv_style_set_bg_opa(&style_file, LV_OPA_0);
    lv_style_set_border_color(&style_file, lv_color_black());
    lv_style_set_border_width(&style_file, 1);
    lv_style_set_border_opa(&style_file, LV_OPA_100);
    lv_obj_add_style(file, &style_file, LV_PART_MAIN);
}