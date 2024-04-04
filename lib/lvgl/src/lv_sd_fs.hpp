/**
 * @file lv_sd_fs.hpp
 * @author Jordi Gauchía
 * @brief  SD filesystem for LVGL
 * @version 0.3.1
 * @date 2024-04
 */

#ifndef LV_SD_FS_HPP
#define LV_SD_FS_HPP

#include "lvgl.h"
#include <FS.h>
#include <SD.h>

void *filesystemSDOpen(lv_fs_drv_t *drv, const char *path, lv_fs_mode_t mode);
lv_fs_res_t filesystemSDClose(lv_fs_drv_t *drv, void *file_p);
lv_fs_res_t filesystemSDRead(lv_fs_drv_t *drv, void *file_p, void *fileBuf, uint32_t btr, uint32_t *br);
lv_fs_res_t filesystemSDWrite(lv_fs_drv_t *drv, void *file_p, const void *buf, uint32_t btw, uint32_t *bw);
lv_fs_res_t filesystemSDSeek(lv_fs_drv_t *drv, void *file_p, uint32_t pos, lv_fs_whence_t whence);
lv_fs_res_t filesystemSDTell(lv_fs_drv_t *drv, void *file_p, uint32_t *pos_p);
void *filesystemSDOpenDir(lv_fs_drv_t *drv, const char *dirpath);
lv_fs_res_t filesystemSDReadDir(lv_fs_drv_t *drv, void *dir_p, char *fn, uint32_t fn_len);
lv_fs_res_t filesystemSDCloseDir(lv_fs_drv_t *drv, void *dir_p);
void filesystemSDInit();

#endif