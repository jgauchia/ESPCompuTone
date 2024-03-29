/**
 * @file lv_funcs.h
 * @author Jordi Gauchía
 * @brief  LVGL custom functions
* @version 0.2
 * @date 2024-03
 */

/**
 * @brief Custom LVGL function to select button
 *
 * @param obj
 */
void select_obj(_lv_obj_t *obj, bool sel)
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
