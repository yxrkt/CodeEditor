#pragma once

#include "include\cef_app.h"
#include "include\cef_browser_process_handler.h"
#include "include\cef_scheme.h"

class CefZipArchive;

class CodeEditorApp
    : public CefApp
    , public CefBrowserProcessHandler
    , public CefSchemeHandlerFactory
{
    IMPLEMENT_REFCOUNTING(CodeEditorApp);

public:
    CodeEditorApp();

    // CefApp
    virtual void OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) override;
    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;

    // CefBrowserProcessHandler
    virtual void OnContextInitialized() override;

    // CefSchemeHandlerFactory
    virtual CefRefPtr<CefResourceHandler> Create(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        const CefString& scheme_name,
        CefRefPtr<CefRequest> request) override;

private:
    void LoadResourcesArchive();
    CefRefPtr<CefStreamReader> GetResource(const char* url);

private:
    CefRefPtr<CefZipArchive> m_resourcesArchive;
};