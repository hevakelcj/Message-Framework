#include "Counter.h"
#include "MsgId.h"
#include <iostream>
using namespace std;

Counter::Counter(int id, int value, int step)
    : Object(id)
    , m_nStep(step)
    , m_nCount(value)
{
}

int Counter::processMessage(DWORD msgId, DWORD param1, DWORD param2)
{
    switch (msgId) {
    case MSG_ID_PRINT:
        cout << "Counter = " << m_nCount << endl;
        break;
    case MSG_ID_TICK:
        cout << "Tick" << endl;
        m_nCount += m_nStep;
        if (m_nCount <= 0)
            pApp->quit();
        break;
    }
    return 0;
}
