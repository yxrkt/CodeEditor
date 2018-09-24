#include "CodeEditorSubprocessApp.h"

CefRefPtr<CefRenderProcessHandler> CodeEditorSubprocessApp::GetRenderProcessHandler()
{
    return this;
}

void CodeEditorSubprocessApp::OnWebKitInitialized()
{
    //std::string appCode =
    //    "console.log('doing something');"
    //    "var app;"
    //    "if (!app) {"
    //    "    app = {};"
    //    "}"
    //    ""
    //    "(function () {"
    //    "    app.DoSomething = function() {"
    //    "        native function DoSomething;"
    //    "        DoSomething();"
    //    "    }"
    //    "})();"
    //    ""
    //    "app.DoSomething();";

    //CefRegisterExtension("v8/app", appCode, this);
}

bool CodeEditorSubprocessApp::Execute(
    const CefString& name,
    CefRefPtr<CefV8Value> object,
    const CefV8ValueList& arguments,
    CefRefPtr<CefV8Value>& retval,
    CefString& exception)
{
    if (name == "DoSomething" && arguments.empty())
    {
        //auto frame = CefV8Context::GetCurrentContext()->GetBrowser()->GetMainFrame();
        //frame->ExecuteJavaScript()
        MessageBox(nullptr, "hello, javascript? yes, this is c++", "subprocess", MB_ICONINFORMATION);
        return true;
    }

    return false;
}