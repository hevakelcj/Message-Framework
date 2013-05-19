
#include "Application.h"
#include "Object.h"

extern int Main(int argc, char **argv);

Application* Application::s_self = NULL;

Application* Application::instance()
{
    return s_self;
}

Application::Application(int argc, char **argv)
    : m_isQuit(false)
    , m_isWakeup(false)
    , m_argc(argc)
    , m_argv(argv)
{
    s_self = this;
}

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

void Application::postMessage(DWORD objId, DWORD msgId, DWORD param1, DWORD param2)
{
    /// Enter critical
    m_msgQueue.push(MessageItem(objId, msgId, param1, param2));
    m_isWakeup = true;
    /// Exit critical
}

bool Application::insertObject(Object *obj)
{
    // assert(obj != NULL);
    DWORD id = obj->getId();

    std::map<DWORD, Object*>::const_iterator c_iter = m_objMap.find(id);
    if (c_iter == m_objMap.end()) {
        m_objMap[id] = obj;
        return true;
    } else {
        return false;
    }
}

bool Application::removeObject(Object *obj)
{
    DWORD id = obj->getId();

    std::map<DWORD, Object*>::iterator iter = m_objMap.find(id);
    if (iter != m_objMap.end()) {
        m_objMap.erase(iter);
        return true;
    } else {
        return false;
    }
}

void Application::quit(int retVal)
{
    m_nRetValue = retVal;
    m_isQuit = true;
    m_isWakeup = true;
}
