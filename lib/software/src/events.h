/**
 * @file events.h
 * @author Jordi Gauchía
 * @brief Tape events
 * @version 0.3
 * @date 2024-03
 */

#ifndef EVENTS_H
#define EVENTS_H

//#include <vars.h>

extern bool isRecord;
extern bool isPlay;
extern bool isRewind;
extern bool isAdvance;
extern bool isStop;
extern bool isEject;
extern bool isPause;

enum class tapeEvent 
{
    PLAY,
    PAUSE,
    STOP,
    RECORD,
    NO_KEY
};

inline tapeEvent getTapeEvent()
{
    if (isPause)
        return tapeEvent::PAUSE;
    if (isPlay)
        return tapeEvent::PLAY;
    if (isStop)
        return tapeEvent::STOP;
    if (isRecord)
        return tapeEvent::RECORD;
    return tapeEvent::NO_KEY;
};

#endif