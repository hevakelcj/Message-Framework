#ifndef COUNTER_H
#define COUNTER_H

#include "Object.h"

class Counter : public Object
{
public:
    Counter(int id, int value, int step);
protected:
    virtual int processMessage(DWORD msgId, DWORD param1, DWORD param2);
private:
    int m_nStep;
    int m_nCount;
};

#endif // COUNTER_H
