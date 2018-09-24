#pragma once

#include "include\cef_client.h"
#include "include\cef_render_handler.h"

class CefZipArchive;
class CefStreamReader;
class ICodeEditorRenderer;

class CodeEditorImpl
    : public CefClient
    , public CefKeyboardHandler
    , public CefLifeSpanHandler
    , public CefRenderHandler
{
    IMPLEMENT_REFCOUNTING(CodeEditorImpl);

public:
    CodeEditorImpl(ICodeEditorRenderer* renderer);

    // CefClient
    virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override;
    virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() override;

    // CefKeyboardHandler

    // CefLifeSpanHandler
    virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
    virtual bool DoClose(CefRefPtr<CefBrowser> browser) override;
    virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

    // CefRenderHandler
    virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;
    virtual void OnPaint(
        CefRefPtr<CefBrowser> browser,
        PaintElementType type,
        const RectList& dirtyRects,
        const void* buffer,
        int width,
        int height) override;

private:
    ICodeEditorRenderer* m_renderer;
    CefRefPtr<CefBrowser> m_browser;
};