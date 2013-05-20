
#include "Application.h"
#include "Counter.h"
#include "MsgId.h"
#include "ObjectId.h"
#include <signal.h>
#include <iostream>

using namespace std;

void SignalHandle(int sig)
{
    switch (sig) {
    case SIGINT:
        PostMessage(OBJ_ID_COUNTER_1, MSG_ID_PRINT);
        PostMessage(OBJ_ID_COUNTER_2, MSG_ID_PRINT);
        break;
    case SIGALRM:
        PostMessage(OBJ_ID_COUNTER_1, MSG_ID_TICK);
        PostMessage(OBJ_ID_COUNTER_2, MSG_ID_TICK);
        alarm(1);
        break;
    }
}

int main (int argc, char *argv[])
{
    cout << "start ..." << endl;

    Application app(argc, argv);

    Counter c1(OBJ_ID_COUNTER_1, 2, 1);
    Counter c2(OBJ_ID_COUNTER_2, 100, -5);

    if (true) {
        Counter c3(OBJ_ID_COUNTER_3, 0, 1);
    }

    signal(SIGINT, SignalHandle);
    signal(SIGALRM, SignalHandle);
    alarm(1);

    int ret = app.exec();
    cout << "exit:" << ret << endl;

    return ret;
}
