#include "CodeEditorSubprocessApp.h"

CefRefPtr<CefRenderProcessHandler> CodeEditorSubprocessApp::GetRenderProcessHandler()
{
    return this;
}

void CodeEditorSubprocessApp::OnWebKitInitialized()
{
    //std::string appCode =
    //    "var app;"
    //    "if (!app) {"
    //    "    app = {};"
    //    "    app.message = 'i am extension. i am consciousness.'"
    //    "}"
    //    ""
    //    "(function () {"
    //    "    app.doSomething = function() {"
    //    "        native function doSomething();"
    //    "        doSomething();"
    //    "    }"
    //    "})();";

    //CefRegisterExtension("v8/app", appCode, this);
}

void CodeEditorSubprocessApp::OnBrowserCreated(CefRefPtr<CefBrowser> browser)
{
    if (m_browser == nullptr)
    {
        m_browser = browser;
    }
}

void CodeEditorSubprocessApp::OnBrowserDestroyed(CefRefPtr<CefBrowser> browser)
{
    if (browser == m_browser)
    {
        m_browser = nullptr;
    }
}

bool CodeEditorSubprocessApp::Execute(
    const CefString& name,
    CefRefPtr<CefV8Value> object,
    const CefV8ValueList& arguments,
    CefRefPtr<CefV8Value>& retval,
    CefString& exception)
{
    if (name == "doSomething" && arguments.empty())
    {
        //auto frame = CefV8Context::GetCurrentContext()->GetBrowser()->GetMainFrame();
        //frame->ExecuteJavaScript()
        //MessageBox(nullptr, "hello, javascript? yes, this is c++", "subprocess", MB_ICONINFORMATION);
        auto message = CefProcessMessage::Create("notify");
        auto args = message->GetArgumentList();
        args->SetString(0, "THAT'S your browser. INTERNET EXPLORER's your browser!");
        m_browser->SendProcessMessage(PID_BROWSER, message);
        return true;
    }

    return false;
}