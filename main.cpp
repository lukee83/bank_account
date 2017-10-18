#include <iostream>
#include <thread> // c++11 thread support
#include <memory> // smart ptrs
#include <windows.h> // ctrl+c handler
#include "bank_account.h"

using namespace std;

static bool g_run = false;

BOOL WINAPI consoleHandler(DWORD signal)
{
    if (signal == CTRL_C_EVENT)
    {
        cout << "Ctrl-C handled" << endl; // do cleanup
        g_run = false;
    }
    return TRUE;
}

// https://stackoverflow.com/questions/15278343/c11-thread-safe-queue

int main()
{
    cout << "Hello world!" << endl;

    if (!SetConsoleCtrlHandler(consoleHandler, TRUE))
    {
        cout << "ERROR: Could not set control handler" << endl;
        return 1;
    }

    cout << "Create banks!" << endl;

    BankAccountHelper bah;

    bah.initialize_banks();

    cout << "Generate accounts!" << endl;

    bah.generate_accounts(5);

    cout << "Fill work queue!" << endl;

    cout << "Start consuming threads!" << endl;

    cout << "Generate VIBER transfers!" << endl;




    g_run = true;
    while(g_run);

    cout << "Bye!" << endl;
    return 0;
}
