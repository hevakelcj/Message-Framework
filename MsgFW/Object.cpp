/**
 * \file Object.cpp
 * \brief define some member function of Object
 * \author Chunjun Li
 * \date 2013-05-19
 * \version 1.0
 */

#include "Object.h"
#include "Application.h"

/**
 * Constructor, Register to Application.
 * \param id ID of this object
 */
Object::Object(DWORD id)
    : m_nId(id)
{
    pApp->insertObject(this);
    pApp->postMessage(id, MSG_ID_INIT);
}

/**
 * Destructor, Unregister from Application.
 */
Object::~Object()
{
    pApp->removeObject(this);
}

/**
 * Process object message here
 * \param msgId
 * \param param1
 * \param param2
 * \return 0:success, other:some error
 * \see Application::exec()
 */
int Object::processMessage(DWORD msgId, DWORD param1, DWORD param2)
{
    msgId = msgId;
    param1 = param1;
    param2 = param2;
    return 0;
}
