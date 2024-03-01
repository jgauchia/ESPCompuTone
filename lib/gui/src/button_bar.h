/**
 * @file button_bar.h
 * @author Jordi Gauchía
 * @brief LVGL - Button Bar
 * @version 0.1
 * @date 2024-03-01
 */

static lv_obj_t *buttonBar;

/**
 * @brief Create button bar screen
 *
 */
void create_button_bar_scr()
{
    // Button Bar
    buttonBar = lv_obj_create(mainScr);
    lv_obj_set_size(buttonBar, TFT_WIDTH, 50);
    lv_obj_set_pos(buttonBar, 0, TFT_HEIGHT - 50);
    lv_obj_set_flex_flow(buttonBar, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(buttonBar, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(buttonBar, LV_OBJ_FLAG_SCROLLABLE);
    static lv_style_t style_bar;
    lv_style_init(&style_bar);
    lv_style_set_bg_opa(&style_bar, LV_OPA_0);
    lv_style_set_border_opa(&style_bar, LV_OPA_0);
    lv_obj_add_style(buttonBar, &style_bar, LV_PART_MAIN);
    lv_obj_set_style_pad_column(buttonBar, 5, 0);

    // Rec Button
    lv_obj_t *recBtn = lv_img_create(buttonBar);
    lv_img_set_src(recBtn, "F:/rec.bin");
    lv_obj_add_flag(recBtn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(recBtn, 48, 48);
    select_obj(recBtn,false);

    // Play Button
    lv_obj_t *playBtn = lv_img_create(buttonBar);
    lv_img_set_src(playBtn, "F:/play.bin");
    lv_obj_add_flag(playBtn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(playBtn, 48, 48);
    select_obj(playBtn,false);

    // Rew Button
    lv_obj_t *rewBtn = lv_img_create(buttonBar);
    lv_img_set_src(rewBtn, "F:/rew.bin");
    lv_obj_add_flag(rewBtn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(rewBtn, 48, 48);
    select_obj(rewBtn,false);

    // Ffw Button
    lv_obj_t *ffwBtn = lv_img_create(buttonBar);
    lv_img_set_src(ffwBtn, "F:/ffw.bin");
    lv_obj_add_flag(ffwBtn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(ffwBtn, 48, 48);
    select_obj(ffwBtn,false);

    // Stop Button
    lv_obj_t *stopBtn = lv_img_create(buttonBar);
    lv_img_set_src(stopBtn, "F:/stop.bin");
    lv_obj_add_flag(stopBtn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(stopBtn, 48, 48);
    select_obj(stopBtn,false);

    // Pause Button
    lv_obj_t *pauseBtn = lv_img_create(buttonBar);
    lv_img_set_src(pauseBtn, "F:/pause.bin");
    lv_obj_add_flag(pauseBtn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(pauseBtn, 48, 48);
    select_obj(pauseBtn,false);
}