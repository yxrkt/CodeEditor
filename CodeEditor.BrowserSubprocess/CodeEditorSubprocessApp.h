#pragma once

#include "include\cef_app.h"

class CodeEditorSubprocessApp
    : public CefApp
    , public CefRenderProcessHandler
    , public CefV8Handler
{
    IMPLEMENT_REFCOUNTING(CodeEditorSubprocessApp);

public:
    // CefApp
    virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;

    // CefRenderProcessHandler
    virtual void OnWebKitInitialized() override;
    virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser) override;
    virtual void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) override;

    // CefV8Handler
    virtual bool Execute(
        const CefString& name,
        CefRefPtr<CefV8Value> object,
        const CefV8ValueList& arguments,
        CefRefPtr<CefV8Value>& retval,
        CefString& exception) override;

private:
    CefRefPtr<CefBrowser> m_browser;
};