/**
 * @file    Application.h
 * @brief   define application class
 * @version 1.0
 * @author  Hevake Lee
 * @date    2013-05-19
 */

#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "type.h"
#include <map> 
#include <queue>

class Object;

class Application
{
friend class Object;
public:
    static Application* instance();
    Application(int argc = 0, char **argv = NULL);

    int exec();
    void quit(int retVal = 0);

    void postMessage(DWORD objId, DWORD msgId, DWORD param1 = 0, DWORD param2 = 0);
    void sendMessage(DWORD objId, DWORD msgId, DWORD param1 = 0, DWORD param2 = 0);

protected:
    bool insertObject(Object *obj);
    bool removeObject(Object *obj);

private:
    /**
     * Message item, Include base information of :\n
     * ^ Object Id, Whose message.\n
     * ^ Message Id, What message it is.\n
     * ^ Param 1
     * ^ Param 2
     */
    struct MessageItem {
        DWORD   objId, msgId, param1, param2;
        MessageItem(DWORD obj, DWORD msg, DWORD p1, DWORD p2)
            : objId(obj), msgId(msg), param1(p1), param2(p2)
        {}
    };
    static Application *s_self;
    std::queue<MessageItem>     m_msgQueue; /**< Message queue */
    std::map<DWORD, Object*>    m_objMap;   /**< Objects map */
    
    bool m_isQuit;  /**< Quit tag, it will set ture after quit() is called */
    bool m_isWakeup;    /**< Wakeup tag, it will be set true once postMessage() is called */
    int m_nRetValue;    /**< Return value, set by quit() */

    int m_argc;
    char **m_argv;
};

#define pApp    (Application::instance())
#define PostMessage pApp->postMessage
#define SendMessage pApp->sendMessage

#endif  //__APPLICATION_H__
