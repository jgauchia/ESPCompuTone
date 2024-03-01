/**
 * @file keys.h
 * @author Jordi Gauchia
 * @brief Keys imput routines
 * @version 0.1
 * @date 2024-02-26
 */

/**
 * @brief Read keys
 *
 * @return int -> enum structure keys index
 */
int Read_Keys()
{
    keys.read8();
    switch (keys.value())
    {
    case 254:
        return REC;
        break;
    case 253:
        return PLAY;
        break;
    case 252:
        return REC_PLAY;
        break;
    case 251:
        return REW;
        break;
    case 247:
        return ADV;
        break;
    case 239:
        return STOP_EJ;
        break;
    case 223:
        return PAUSE;
        break;
    default:
        return NONE;
        break;
    }
}

/**
 * @brief Keys actions
 *
 *
 */
void Check_keys()
{
    key_pressed = Read_Keys();

    switch (key_pressed)
    {
    case PLAY:
        if (is_stop && !is_pause)
        {
            log_i("PLAY");
            is_record = false;
            is_play = true;
            is_rewind = false;
            is_advance = false;
            is_stop = false;
            is_eject = false;
            is_pause = false;
            maxGifDuration = 5000;
        }
        break;
    case REC_PLAY:
        if (is_stop && !is_pause)
        {
            log_i("RECORD");
            is_record = true;
            is_play = false;
            is_rewind = false;
            is_advance = false;
            is_stop = false;
            is_eject = false;
            is_pause = false;
            maxGifDuration = 5000;
        }
        break;
    case REW:
        if (is_stop && !is_pause)
        {
            log_i("REWIND");
            is_record = false;
            is_play = false;
            is_rewind = true;
            is_advance = false;
            is_stop = false;
            is_eject = false;
            is_pause = false;
            maxGifDuration = 5000;
        }
        break;
    case ADV:
        if (is_stop && !is_pause)
        {
            log_i("ADVANCE");
            is_record = false;
            is_play = false;
            is_rewind = false;
            is_advance = true;
            is_stop = false;
            is_eject = false;
            is_pause = false;
            maxGifDuration = 5000;
        }
        break;
    case STOP_EJ:
        if (is_stop)
        {
            log_i("EJECT");
            is_stop = false;
            is_eject = true;
            fileopen = true;
        }
        else
        {
            log_i("STOP");
            is_stop = true;
            is_eject = false;
            fileopen = false;
        }
        is_record = false;
        is_play = false;
        is_rewind = false;
        is_advance = false;
        is_pause = false;
        maxGifDuration = 0;
        break;
    case PAUSE:
        if (!is_pause)
        {
            log_i("PAUSE");
            is_pause = true;
            maxGifDuration = 0;
        }
        else
        {
            log_i("NO PAUSE");
            is_pause = false;
            if (is_play || is_record || is_rewind || is_advance)
                maxGifDuration = 5000;
        }

        break;
    default:
        break;
    }
}