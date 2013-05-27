/**
 * \file Object.h
 * \brief Define Object class
 * \author Chunjun Li
 * \date 2013-05-19
 * \version 1.0
 */

#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "type.h"
#include "Application.h"

#define MSG_ID_INIT 1

/**
 * Object class has a processMessage() virtual\n
 * function which is call by Application::exec().
 */
class Object
{
friend class Application;
public:

    Object(DWORD id);
    ~Object();

    DWORD   getId() { return m_nId; }

protected:
    virtual int processMessage(DWORD msgId, DWORD param1, DWORD param2);

private:
    DWORD   m_nId;  /**< Object ID */
};

#endif  //__OBJECT_H__
