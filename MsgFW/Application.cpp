/**
 * @file    Application.cpp
 * @brief   define application function
 * @version 1.0
 * @author  Hevake Lee
 * @date    2013-05-20
 */

#include "Application.h"
#include "Object.h"
#include "define.h"

/** sigleton object pointer */
Application* Application::s_self = NULL;

Application* Application::instance()
{
    ASSERT(s_self != NULL);
    return s_self;
}

/**
 * Application constructor
 * \param argc
 * \param argv
 */
Application::Application(int argc, char **argv)
    : m_isQuit(false)
    , m_isWakeup(false)
    , m_argc(argc)
    , m_argv(argv)
{
    ASSERT(s_self == NULL);
    s_self = this;
}

/**
 * Application message process function.\n
 * Fetch message from queue and deliver to object.
 */
int Application::exec()
{
    while (!m_isQuit) {
        while (!m_isWakeup); 

        while (!m_msgQueue.empty()) {
            /// Enter critical
            MessageItem msg = m_msgQueue.front();
            m_msgQueue.pop();
            /// Exit critical

            Object *obj = m_objMap[msg.objId];
            if (obj)
                obj->processMessage(msg.msgId, msg.param1, msg.param2);
        }
        m_isWakeup = false;
    }
    return m_nRetValue;
}

/**
 * Put message into message queue and wakeup exec() to deal with it.
 * \param objId whose message
 * \param msgId message ID
 * \param param1
 * \param param2
 */
void Application::postMessage(DWORD objId, DWORD msgId, DWORD param1, DWORD param2)
{
    /// Enter critical
    m_msgQueue.push(MessageItem(objId, msgId, param1, param2));
    m_isWakeup = true;
    /// Exit critical
}

/**
 * Find object by ID and call its processMessage()
 * \param objId whose message
 * \param msgId message ID
 * \param param1
 * \param param2
 */
void Application::sendMessage(DWORD objId, DWORD msgId, DWORD param1, DWORD param2)
{
    Object *obj = m_objMap[objId];
    if (obj)
        obj->processMessage(msgId, param1, param2);
}

/**
 * Register object to Application::m_objMap;
 * \param obj   address of object
 * \return true:success; false:fail
 */
bool Application::insertObject(Object *obj)
{
    ASSERT(obj != NULL);
    DWORD id = obj->getId();

    std::map<DWORD, Object*>::const_iterator c_iter = m_objMap.find(id);
    if (c_iter == m_objMap.end()) {
        m_objMap[id] = obj;
        return true;
    } else {
        return false;
    }
}

/**
 * Unregister object from Application::m_objMap;
 * \param obj   address of object
 * \return true:success; false:fail
 */
bool Application::removeObject(Object *obj)
{
    ASSERT(obj != NULL);
    DWORD id = obj->getId();

    std::map<DWORD, Object*>::iterator iter = m_objMap.find(id);
    if (iter != m_objMap.end()) {
        m_objMap.erase(iter);
        return true;
    } else {
        return false;
    }
}

/**
 * Ask Application to stop message loop
 * \param retVal Application::exec() return value
 */
void Application::quit(int retVal)
{
    m_nRetValue = retVal;
    m_isQuit = true;
    m_isWakeup = true;
}
