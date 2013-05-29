
#ifndef __TIMER_H__
#define __TIMER_H__

#include "Object.h"
#include <map>

#define OBJ_ID_TIMER    1
#define MSG_ID_TIMER    1

typedef int HTimer;

class TimerMgr : public Object
{
    struct Context
    {
        DWORD   counter;
        ///////////////////
        HTimer    tmr_id;
        DWORD   obj_id;
        DWORD   period;
        DWORD   start_delay;
        bool    one_shot;
        bool    immediate;

        Context()
            : tmr_id(0), obj_id(0), period(0), start_delay(0)
            , one_shot(false), immediate(false) {}

        Context(HTimer tid, DWORD oid, DWORD pd, DWORD sd, bool os, bool im)
            : tmr_id(tid), obj_id(oid), period(pd), start_delay(sd)
            , one_shot(os), immediate(im) {}
    };

public:
    static TimerMgr* instance();

    HTimer create(DWORD owner, DWORD period, DWORD startDelay = 0,
                  bool oneShot = false, bool immediate = false);
    bool start(HTimer tmr);
    bool setPeriod(HTimer tmr, DWORD period, bool now = false);
    bool reload(HTimer tmr);
    bool stop(HTimer tmr);
    bool destory(HTimer tmr);

protected:
    TimerMgr();
    virtual int processMessage(DWORD msgId, DWORD param1, DWORD param2);

    void onInit();
    void onTick();

private:
    std::map<HTimer, Context> m_Context;
    int m_nIdCnt;
};

#define pTimerMgr   (TimerMgr::instance())

#endif  //__TIMER_H__

