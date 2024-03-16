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

enum class Tape_event
{
    PLAY,
    PAUSE,
    STOP,
    RECORD,
    NO_KEY,
};

static Tape_event TapeEvent()
{
    if (is_pause)
        return Tape_event::PAUSE;
    if (is_play)
        return Tape_event::PLAY;
    if (is_stop)
        return Tape_event::STOP;
    if (is_record)
        return Tape_event::RECORD;
    return Tape_event::NO_KEY;
};

#endif