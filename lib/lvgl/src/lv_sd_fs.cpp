/**
 * @file lv_sd_fs.cpp
 * @author Jordi Gauchía
 * @brief  SD filesystem for LVGL
 * @version 0.3
 * @date 2024-04
 */

#include <lv_sd_fs.hpp>


/**
 * @brief SD Open LVGL CallBack
 *
 * @param drv
 * @param path
 * @param mode
 * @return void*
 */
void *filesystemSDOpen(lv_fs_drv_t *drv, const char *path, lv_fs_mode_t mode)
{
    LV_UNUSED(drv);
    const char *flags = "";

    if (mode == LV_FS_MODE_WR)
        flags = FILE_WRITE;
    else if (mode == LV_FS_MODE_RD)
        flags = FILE_READ;
    else if (mode == (LV_FS_MODE_WR | LV_FS_MODE_RD))
        flags = FILE_WRITE;

    File f = SD.open(path, flags);
    
    if (!f)
    {
        log_e("Failed to open file!");
        return NULL;
    }

    File *lf = new File{f};

    // make sure at the beginning
    // fp->seek(0);

    return (void *)lf;
}

/**
 * @brief SD Close LVGL CallBack
 *
 * @param drv
 * @param file_p
 * @return lv_fs_res_t
 */
lv_fs_res_t filesystemSDClose(lv_fs_drv_t *drv, void *file_p)
{
    LV_UNUSED(drv);

    File *fp = (File *)file_p;

    fp->close();

    delete (fp); // when close
    return LV_FS_RES_OK;
}

/**
 * @brief SD Read LVGL CallBack
 *
 * @param drv
 * @param file_p
 * @param fileBuf
 * @param btr
 * @param br
 * @return lv_fs_res_t
 */
lv_fs_res_t filesystemSDRead(lv_fs_drv_t *drv, void *file_p, void *fileBuf, uint32_t btr, uint32_t *br)
{
    LV_UNUSED(drv);

    File *fp = (File *)file_p;

    *br = fp->read((uint8_t *)fileBuf, btr);

    return (int32_t)(*br) < 0 ? LV_FS_RES_UNKNOWN : LV_FS_RES_OK;
}

/**
 * @brief SD Write LVGL CallBack
 *
 * @param drv
 * @param file_p
 * @param buf
 * @param btw
 * @param bw
 * @return lv_fs_res_t
 */
lv_fs_res_t filesystemSDWrite(lv_fs_drv_t *drv, void *file_p, const void *buf, uint32_t btw, uint32_t *bw)
{
    LV_UNUSED(drv);

    File *fp = (File *)file_p;

    *bw = fp->write((const uint8_t *)buf, btw);

    return (int32_t)(*bw) < 0 ? LV_FS_RES_UNKNOWN : LV_FS_RES_OK;
}

/**
 * @brief SD Seek LVGL CallBack
 *
 * @param drv
 * @param file_p
 * @param pos
 * @param whence
 * @return lv_fs_res_t
 */
lv_fs_res_t filesystemSDSeek(lv_fs_drv_t *drv, void *file_p, uint32_t pos, lv_fs_whence_t whence)
{
    LV_UNUSED(drv);

    File *fp = (File *)file_p;

    SeekMode mode;
    if (whence == LV_FS_SEEK_SET)
        mode = SeekSet;
    else if (whence == LV_FS_SEEK_CUR)
        mode = SeekCur;
    else if (whence == LV_FS_SEEK_END)
        mode = SeekEnd;

    fp->seek(pos, mode);

    return LV_FS_RES_OK;
}

/**
 * @brief SD Tell LVGL CallBack
 *
 * @param drv
 * @param file_p
 * @param pos_p
 * @return lv_fs_res_t
 */
lv_fs_res_t filesystemSDTell(lv_fs_drv_t *drv, void *file_p, uint32_t *pos_p)
{
    LV_UNUSED(drv);

    File *fp = (File *)file_p;

    *pos_p = fp->position();

    return LV_FS_RES_OK;
}

/**
 * @brief SD Dir Open LVGL CallBack
 *
 * @param drv
 * @param dirpath
 * @return void*
 */
void *filesystemSDOpenDir(lv_fs_drv_t *drv, const char *dirpath)
{
    LV_UNUSED(drv);

    File root = SD.open(dirpath);
    
    if (!root)
    {
        log_e("Failed to open directory!");
        return NULL;
    }

    if (!root.isDirectory())
    {
        log_e("Not a directory!");
        return NULL;
    }

    File *lroot = new File{root};

    return (void *)lroot;
}

/**
 * @brief SD Dir Read LVGL CallBack
 *
 * @param drv
 * @param dir_p
 * @param fn
 * @param fn_lem
 * @return lv_fs_res_t
 */
lv_fs_res_t filesystemSDReadDir(lv_fs_drv_t *drv, void *dir_p, char *fn, uint32_t fn_len)
{
    LV_UNUSED(drv);

    File *root = (File *)dir_p;
    fn[0] = '\0';

    File file = root->openNextFile();
    while (file)
    {
        if (strcmp(file.name(), ".") == 0 || strcmp(file.name(), "..") == 0)
        {
            continue;
        }
        else
        {
            if (file.isDirectory())
            {
                log_i("  DIR : %s", file.name());
                fn[0] = '/';
                strcpy(&fn[1], file.name());
            }
            else
            {
                log_i("  FILE: %s", file.name());
                log_i("  SIZE: %d", file.size());
                strcpy(fn, file.name());
            }
            break;
        }
        file = root->openNextFile();
    }

    return LV_FS_RES_OK;
}

/**
 * @brief SD Dir Close LVGL CallBack
 *
 * @param drv
 * @param dir_p
 * @return lv_fs_res_t
 */
lv_fs_res_t filesystemSDCloseDir(lv_fs_drv_t *drv, void *dir_p)
{
    LV_UNUSED(drv);

    File *root = (File *)dir_p;

    root->close();

    delete (root); // when close

    return LV_FS_RES_OK;
}

/**
 * @brief SD Filesystem init
 * 
 */
void filesystemSDInit()
{
    /*---------------------------------------------------
     * Register the file system interface in LVGL
     *--------------------------------------------------*/

    /*Add a simple drive to open images*/
    static lv_fs_drv_t fs_drv;
    lv_fs_drv_init(&fs_drv);

    /*Set up fields...*/
    fs_drv.letter = 'S';
    fs_drv.cache_size = sizeof(File);

    fs_drv.open_cb = filesystemSDOpen;
    fs_drv.close_cb = filesystemSDClose;
    fs_drv.read_cb = filesystemSDRead;
    fs_drv.write_cb = filesystemSDWrite;
    fs_drv.seek_cb = filesystemSDSeek;
    fs_drv.tell_cb = filesystemSDTell;

    fs_drv.dir_close_cb = filesystemSDClose;
    fs_drv.dir_open_cb = filesystemSDOpenDir;
    fs_drv.dir_read_cb = filesystemSDReadDir;

    lv_fs_drv_register(&fs_drv);
}