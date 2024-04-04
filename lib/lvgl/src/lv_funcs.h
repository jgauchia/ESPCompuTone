/**
 * @file lv_funcs.h
 * @author Jordi Gauchía
 * @brief  LVGL custom functions
 * @version 0.3.1
 * @date 2024-04
 */

#ifndef LV_FUNCS_H
#define LV_FUNCS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <Arduino.h>
#include <lvgl.h>

    /**
     * @brief Custom LVGL function to select tape key
     *
     * @param obj
     */
    void selectTapeKey(lv_obj_t *obj, bool sel);

    /**
     * @brief Clear LVGL table
     *
     * @param obj
     */
    void lv_table_clear(lv_obj_t *obj);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif