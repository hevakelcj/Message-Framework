
#include "Timer.h"
#include "define.h"
#include <iterator>

TimerMgr* TimerMgr::instance()
{
    static TimerMgr sigleton;
    return &sigleton;
}

TimerMgr::TimerMgr()
    : Object(OBJ_ID_TIMER)
    , m_nIdCnt(0)
{}

HTmr TimerMgr::create(DWORD owner, DWORD period, DWORD startDelay, bool one_shot)
{
    Context *p = new Context(++m_nIdCnt, owner, period, startDelay, one_shot);
    ASSERT(p != NULL);

    m_Context[m_nIdCnt] = p;
    return m_nIdCnt;
}

bool TimerMgr::start(HTmr tmr)
{
    // find Context, and get pointer. if not return false
    std::map<HTmr, Context*>::iterator iter = m_Context.find(tmr);
    if (iter != m_Context.end()) {
        Context *p = iter->second;
        ASSERT(p != NULL);
        if (p->start_delay) {
            p->counter = p->start_delay; 
        } else {
            p->counter = p->period; 
        }
        insert_into(p);
        return true;
    }
    return false;
}

bool TimerMgr::setPeriod(DWORD tmr, DWORD period, bool now)
{
    std::map<HTmr, Context*>::iterator iter = m_Context.find(tmr);
    if (iter != m_Context.end()) {
        Context *p = iter->second;
        ASSERT(p != NULL);
        p->period = period;

        if (now) {
            stop(tmr);
            start(tmr);
        }
        return true;
    }
    return false;
}

bool TimerMgr::stop(HTmr tmr)
{
    if (m_Runlist.empty())
        return false;

    std::list<Context*>::iterator iter = m_Runlist.begin();
    for (; iter != m_Runlist.end(); ++iter) {
        Context *p = *iter; 
        ASSERT(p != NULL);
        if (p->tmr_id == tmr) {
            std::list<Context*>::iterator nextIter = iter;
            nextIter ++;
            if (nextIter != m_Runlist.end()) {
                Context *pn = *nextIter;
                ASSERT(pn != NULL);
                pn->counter += p->counter; 
                m_Runlist.erase(iter);
            }
            return true; 
        }
    }
    return false;
}

int TimerMgr::destory(HTmr tmr)
{
    stop(tmr);
    Context *p = m_Context[tmr];
    ASSERT(p != NULL);
    m_Context.erase(tmr);
    delete p;
    return 0;
}

int TimerMgr::processMessage(DWORD msgId, DWORD param1, DWORD param2)
{
    param2 = param1;
    if (msgId == MSG_ID_TICK)
        do_tick();
    return 0;
}

void TimerMgr::do_tick()
{
    if (m_Runlist.empty())
        return;

    std::list<Context*>::iterator iter = m_Runlist.begin();
    Context *p = *iter;
    ASSERT(p != NULL);
    p->counter --;

    for (; iter != m_Runlist.end(); ++iter) {
        if (p->counter)
            break; 

        if (p->start_delay) {
            p->counter = p->start_delay;
            p->start_delay = 0;
            insert_into(p);
        } else {
            SendMessage(p->obj_id, MSG_ID_TIMER, p->tmr_id); 
            m_Runlist.erase(iter);
            if (!p->one_shot) {
                p->counter = p->period; 
                insert_into(p);
            }
        }
    }
}

void TimerMgr::insert_into(Context *ct)
{
    std::list<Context*>::iterator iter = m_Runlist.begin();

    for (; iter != m_Runlist.end(); ++iter) {
        Context *p = *iter;
        ASSERT(p != NULL);
        if (p->counter < ct->counter) {
            ct->counter -= p->counter;
        } else {
            break; 
        }
    }
    if (iter == m_Runlist.end()) {
        m_Runlist.push_back(ct);
    } else {
        m_Runlist.insert(iter, ct);
    }
}
