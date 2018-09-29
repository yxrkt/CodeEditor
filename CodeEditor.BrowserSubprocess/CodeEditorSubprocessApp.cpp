#include "CodeEditorSubprocessApp.h"

#include "include\cef_browser.h"
#include "include\cef_frame.h"

CefRefPtr<CefRenderProcessHandler> CodeEditorSubprocessApp::GetRenderProcessHandler()
{
    return this;
}

void CodeEditorSubprocessApp::OnWebKitInitialized()
{
    std::string appCode = R"(
var app = {
    getInitialValue: function() {
        native function getInitialValue();
        return getInitialValue();
    },
    save: function(text) {
        native function save();
        save(text);
    }
};
)";

    CefRegisterExtension("v8/app", appCode, this);
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

bool CodeEditorSubprocessApp::OnProcessMessageReceived(
    CefRefPtr<CefBrowser> browser,
    CefProcessId /*source_process*/,
    CefRefPtr<CefProcessMessage> message)
{
    if (message->GetName() == "load")
    {
        auto args = message->GetArgumentList();

        m_editorInitialValue = args->GetString(0);

        std::string js = R"(
if (typeof editor !== 'undefined') {
    editor.model.setValue(app.getInitialValue());
}
)";

        browser->GetMainFrame()->ExecuteJavaScript(js, {}, 0);
    }

    return false;
}

bool CodeEditorSubprocessApp::Execute(
    const CefString& name,
    CefRefPtr<CefV8Value> object,
    const CefV8ValueList& arguments,
    CefRefPtr<CefV8Value>& retval,
    CefString& exception)
{
    if (name == "save")
    {
        if (arguments.size() == 1)
        {
            auto message = CefProcessMessage::Create("save");
            auto args = message->GetArgumentList();
            args->SetString(0, arguments.front()->GetStringValue());

            m_browser->SendProcessMessage(PID_BROWSER, message);

            return true;
        }
    }
    else if (name == "getInitialValue")
    {
        if (arguments.empty())
        {
            retval = CefV8Value::CreateString(m_editorInitialValue);
            return true;
        }
    }

    return false;
}