
#ifndef __TIMER_H__
#define __TIMER_H__

#include "Object.h"
#include <map>
#include <list>

#define OBJ_ID_TIMER    1

#define MSG_ID_TICK     1
#define MSG_ID_TIMER    2

typedef int HTmr;

class TimerMgr : public Object
{
    struct Context
    {
        DWORD   counter;
        ///////////////////
        HTmr    tmr_id;
        DWORD   obj_id;
        DWORD   period;
        DWORD   start_delay;
        bool    one_shot;

        Context()
            : tmr_id(-1), obj_id(-1), period(0), start_delay(0), one_shot(false)
        {}

        Context(HTmr tid, DWORD oid, DWORD pd, DWORD sd, bool os)
            : tmr_id(tid), obj_id(oid), period(pd), start_delay(sd), one_shot(os)
        {}
    };

public:
    static TimerMgr* instance();

    HTmr create(DWORD owner, DWORD period, DWORD startDelay = 0, bool oneShot = false);
    bool start(HTmr tmr);
    bool setPeriod(DWORD tmr, DWORD period, bool now = false);
    bool stop(HTmr tmr);
    int  destory(HTmr tmr);

protected:
    TimerMgr();
    virtual int processMessage(DWORD msgId, DWORD param1, DWORD param2);

    void do_tick();
    void insert_into(Context *ct);

private:
    std::map<HTmr, Context*> m_Context;
    std::list<Context*> m_Runlist;
    int m_nIdCnt;
};

#endif  //__TIMER_H__

