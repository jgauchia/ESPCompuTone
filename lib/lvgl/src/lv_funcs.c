/**
 * @file lv_funcs.c
 * @author Jordi Gauchía
 * @brief  LVGL custom functions
 * @version 0.3.1
 * @date 2024-04
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
        static lv_style_t styleBtn;
        lv_style_init(&styleBtn);
        lv_style_set_border_color(&styleBtn, lv_color_white());
        lv_style_set_border_opa(&styleBtn, LV_OPA_50);
        lv_style_set_border_width(&styleBtn, 2);
        lv_style_set_bg_color(&styleBtn, lv_color_white());
        lv_style_set_bg_opa(&styleBtn, LV_OPA_50);
        lv_obj_add_style(obj, &styleBtn, 0);
    }
    else
    {
        static lv_style_t styleBtn2;
        lv_style_init(&styleBtn2);
        lv_style_set_border_color(&styleBtn2, lv_color_white());
        lv_style_set_border_opa(&styleBtn2, LV_OPA_50);
        lv_style_set_border_width(&styleBtn2, 2);
        lv_style_set_bg_color(&styleBtn2, lv_color_white());
        lv_style_set_bg_opa(&styleBtn2, LV_OPA_0);
        lv_obj_add_style(obj, &styleBtn2, 0);
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

