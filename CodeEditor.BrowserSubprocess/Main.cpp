#include <windows.h>

#include "include\cef_app.h"
#include "include\internal\cef_win.h"

int APIENTRY WinMain(
    HINSTANCE hInstance,
    HINSTANCE /*hPrevInstance*/,
    LPTSTR /*lpCmdLine*/,
    int /*nCmdShow*/)
{
    CefMainArgs mainArgs(hInstance);

    return CefExecuteProcess(mainArgs, nullptr, nullptr);
}
