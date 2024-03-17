/**
 * @file events.h
 * @author Jordi Gauchía
 * @brief Tape events
 * @version 0.3
 * @date 2024-03
 */

#ifndef EVENTS_H
#define EVENTS_H

#include <vars.h>

enum class tapeEvent
{
    PLAY,
    PAUSE,
    STOP,
    RECORD,
    NO_KEY,
};

static tapeEvent TapeEvent()
{
    if (is_pause)
        return tapeEvent::PAUSE;
    if (is_play)
        return tapeEvent::PLAY;
    if (is_stop)
        return tapeEvent::STOP;
    if (is_record)
        return tapeEvent::RECORD;
    return tapeEvent::NO_KEY;
};

#endif