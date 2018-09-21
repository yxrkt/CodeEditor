#pragma once

#include "include\cef_app.h"
#include "include\cef_browser_process_handler.h"
#include "include\cef_render_handler.h"
#include "include\cef_scheme.h"

class BrowserApp;
class CefZipArchive;
class CefStreamReader;
class ICodeEditorRenderer;

class CodeEditorImpl
    : public CefApp
    , public CefBrowserProcessHandler
    , public CefRenderHandler
    , public CefSchemeHandlerFactory
{
    IMPLEMENT_REFCOUNTING(CodeEditorImpl);

public:
    CodeEditorImpl(ICodeEditorRenderer* renderer);

    // CefBrowserProcessHandler
    virtual void OnContextInitialized() override;
    virtual void OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) override;

    // CefRenderHandler
    virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;
    virtual void OnPaint(
        CefRefPtr<CefBrowser> browser,
        PaintElementType type,
        const RectList& dirtyRects,
        const void* buffer,
        int width,
        int height) override;

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
    ICodeEditorRenderer* m_renderer;
    CefRefPtr<CefZipArchive> m_resourcesArchive;
};