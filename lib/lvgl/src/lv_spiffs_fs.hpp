/**
 * @file lv_spiffs_fs.hpp
 * @author Jordi Gauchía
 * @brief  SPIFFS filesystem for LVGL
 * @version 0.3
 * @date 2024-04
 */

#ifndef LV_SPIFFS_FS_HPP
#define LV_SPIFFS_FS_HPP

#include "lvgl.h"
#include <FS.h>
#include <SPIFFS.h>

void *filesystemSPIFFSOpen(lv_fs_drv_t *drv, const char *path, lv_fs_mode_t mode);
lv_fs_res_t filesystemSPIFFSClose(lv_fs_drv_t *drv, void *file_p);
lv_fs_res_t filesystemSPIFFSRead(lv_fs_drv_t *drv, void *file_p, void *fileBuf, uint32_t btr, uint32_t *br);
lv_fs_res_t filesystemSPIFFSWrite(lv_fs_drv_t *drv, void *file_p, const void *buf, uint32_t btw, uint32_t *bw);
lv_fs_res_t filesystemSPIFFSSeek(lv_fs_drv_t *drv, void *file_p, uint32_t pos, lv_fs_whence_t whence);
lv_fs_res_t filesystemSPIFFSTell(lv_fs_drv_t *drv, void *file_p, uint32_t *pos_p);
void *filesystemSPIFFSOpenDir(lv_fs_drv_t *drv, const char *dirpath);
lv_fs_res_t filesystemSPIFFSReadDir(lv_fs_drv_t *drv, void *dir_p, char *fn, uint32_t fn_len);
lv_fs_res_t filesystemSPIFFSCloseDir(lv_fs_drv_t *drv, void *dir_p);
void filesystemSPIFFSInit();

#endif