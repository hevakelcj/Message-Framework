
#include "Timer.h"
#include "define.h"

TimerMgr* TimerMgr::instance()
{
    static TimerMgr sigleton;
    return &sigleton;
}

TimerMgr::TimerMgr()
    : Object(OBJ_ID_TIMER)
    , m_nIdCnt(0)
{}

HTimer TimerMgr::create(DWORD owner, DWORD period, DWORD start_delay,
                        bool one_shot, bool immediate)
{
    ++m_nIdCnt;
    m_Context[m_nIdCnt] = Context(m_nIdCnt, owner, period, start_delay, one_shot, immediate);
    return m_nIdCnt;
}

bool TimerMgr::start(HTimer tmr)
{
    std::map<HTimer, Context>::iterator iter = m_Context.find(tmr);
    if (iter != m_Context.end()) {
        Context *p = &iter->second;
        if (p->start_delay) {
            p->counter = p->start_delay; 
        } else {
            if (p->immediate)
                PostMessage(p->obj_id, MSG_ID_TIMER, p->tmr_id);
            p->counter = p->period; 
        }
        return true;
    }
    return false;
}

bool TimerMgr::setPeriod(HTimer tmr, DWORD period, bool now)
{
     std::map<HTimer, Context>::iterator iter = m_Context.find(tmr);
    if (iter != m_Context.end()) {
        Context *p = &iter->second;
        p->period = period;

        if (now)
            p->counter = p->period;

        return true;
    }   
    return false;
}

bool TimerMgr::reload(HTimer tmr) 
{
    std::map<HTimer, Context>::iterator iter = m_Context.find(tmr);
    if (iter != m_Context.end()) {
        Context *p = &iter->second;
        p->counter = p->period;
        return true;
    } 
    return false;
}

bool TimerMgr::stop(HTimer tmr)
{
    std::map<HTimer, Context>::iterator iter = m_Context.find(tmr);
    if (iter != m_Context.end()) {
        Context *p = &iter->second;
        p->counter = 0; 
        return true;
    }
    return false;
}

bool TimerMgr::destory(HTimer tmr)
{
    std::map<HTimer, Context>::iterator iter = m_Context.find(tmr);
    if (iter != m_Context.end()) {
        m_Context.erase(iter);
        return true;
    } 
    return false;
}

int TimerMgr::processMessage(DWORD msgId, DWORD param1, DWORD param2)
{
    param1 = param1;
    param2 = param2;

    if (msgId == MSG_ID_INIT) {
        onInit(); 
    } else if (msgId == MSG_ID_TIMER) {
        onTick(); 
    }
    return 0;
}

void TimerMgr::onTick()
{
    std::map<HTimer, Context>::iterator iter = m_Context.begin();
    for (; iter != m_Context.end(); ++iter) {
        Context *p = &iter->second;

        if (p->counter == 0)
            continue;

        p->counter--;
        if (p->counter != 0)
            continue;

        if (p->start_delay) {
            p->counter = p->period; 
            p->start_delay = 0;
            if (p->immediate)
                PostMessage(p->obj_id, MSG_ID_TIMER, p->tmr_id);
        } else {
            PostMessage(p->obj_id, MSG_ID_TIMER, p->tmr_id);
            if (!p->one_shot)
                p->counter = p->period;
        }
    } 
}

void TimerMgr::onInit()
{

}
