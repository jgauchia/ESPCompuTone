/**
 * @file lv_funcs.c
 * @author Jordi Gauchía
 * @brief  LVGL custom functions
 * @version 0.3
 * @date 2024-03
 */

#include "lv_funcs.h"

/**
 * @brief Custom LVGL function to select tape key
 *
 * @param obj
 */
void selectTapeKey(lv_obj_t *obj, bool sel)
{
    if (sel)
    {
        static lv_style_t style_btn;
        lv_style_init(&style_btn);
        lv_style_set_border_color(&style_btn, lv_color_white());
        lv_style_set_border_opa(&style_btn, LV_OPA_50);
        lv_style_set_border_width(&style_btn, 2);
        lv_style_set_bg_color(&style_btn, lv_color_white());
        lv_style_set_bg_opa(&style_btn, LV_OPA_50);
        lv_obj_add_style(obj, &style_btn, 0);
    }
    else
    {
        static lv_style_t style_btn2;
        lv_style_init(&style_btn2);
        lv_style_set_border_color(&style_btn2, lv_color_white());
        lv_style_set_border_opa(&style_btn2, LV_OPA_50);
        lv_style_set_border_width(&style_btn2, 2);
        lv_style_set_bg_color(&style_btn2, lv_color_white());
        lv_style_set_bg_opa(&style_btn2, LV_OPA_0);
        lv_obj_add_style(obj, &style_btn2, 0);
    }
}


/**
 * @brief Clear LVGL table
 *
 * @param obj
 */
void lv_table_clear(lv_obj_t *obj)
{
    if (lv_table_get_row_count(obj) != 0)
        for (int i = 0; i < lv_table_get_row_count(obj); i++)
            lv_table_set_cell_value(obj, i, 0, "");
    lv_table_set_row_count(obj, 0);
}
