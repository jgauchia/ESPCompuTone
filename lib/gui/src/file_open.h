/**
 * @file file_open.h
 * @author Jordi Gauchía
 * @brief LVGL - File Explorer
 * @version 0.1
 * @date 2024-03-01
 */

/**
 * @brief Notify Bar Screen
 *
 */
static lv_obj_t *fileExplorer;

/**
 * @brief Create a file explorer screen
 * 
 */
void create_file_explorer()
{
       
    fileExplorer = lv_file_explorer_create(NULL);
    lv_file_explorer_set_sort(fileExplorer, LV_EXPLORER_SORT_NONE);
    lv_file_explorer_open_dir(fileExplorer, "S:/");
}