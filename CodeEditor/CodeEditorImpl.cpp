#include "CodeEditorImpl.h"

#include "CodeEditorApp.h"
#include "ICodeEditorRenderer.h"

#include <iostream>
#include <string>

static CefRefPtr<CodeEditorApp> s_app;

template <typename T, size_t N>
constexpr size_t count_of(T (&)[N])
{
    return N;
}

static void InitializeCef()
{
    static bool s_initialized = false;
    if (!s_initialized)
    {
        CefEnableHighDPISupport();

        CefSettings settings;

        static const wchar_t browserSubprocessPath[] = L"CodeEditor.BrowserSubprocess.exe";
        cef_string_set(
            browserSubprocessPath,
            count_of(browserSubprocessPath),
            &settings.browser_subprocess_path,
            false);

        settings.multi_threaded_message_loop = true;
        settings.windowless_rendering_enabled = true;
        settings.no_sandbox = true;

        s_app = new CodeEditorApp;
        CefInitialize({}, settings, s_app, nullptr);

        s_initialized = true;
    }
}

CodeEditorImpl::CodeEditorImpl(ICodeEditorRenderer* renderer)
    : m_renderer(renderer)
{
    InitializeCef();

    CefWindowInfo windowInfo;
    windowInfo.SetAsWindowless(NULL);

    CefBrowserHost::CreateBrowser(windowInfo, this, "app://web/index.html", {}, nullptr);
}

CefRefPtr<CefBrowser> CodeEditorImpl::GetBrowser() const
{
    return m_browser;
}

CefRefPtr<CefRenderHandler> CodeEditorImpl::GetRenderHandler()
{
    return this;
}

CefRefPtr<CefKeyboardHandler> CodeEditorImpl::GetKeyboardHandler()
{
    return this;
}

CefRefPtr<CefLifeSpanHandler> CodeEditorImpl::GetLifeSpanHandler()
{
    return this;
}

bool CodeEditorImpl::OnProcessMessageReceived(
    CefRefPtr<CefBrowser> browser,
    CefProcessId source_process,
    CefRefPtr<CefProcessMessage> message)
{
    auto args = message->GetArgumentList();
    std::cout << "from render process: " << args->GetString(0).ToString() << std::endl;

    return false;
}

bool CodeEditorImpl::OnPreKeyEvent(
    CefRefPtr<CefBrowser> browser,
    const CefKeyEvent& event,
    CefEventHandle os_event,
    bool* is_keyboard_shortcut)
{
    return false;
}

bool CodeEditorImpl::OnKeyEvent(
    CefRefPtr<CefBrowser> browser,
    const CefKeyEvent& event,
    CefEventHandle os_event)
{
    return false;
}

void CodeEditorImpl::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
    if (!m_browser)
    {
        m_browser = browser;
    }
}

bool CodeEditorImpl::DoClose(CefRefPtr<CefBrowser> /*browser*/)
{
    return false;
}

void CodeEditorImpl::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    if (m_browser == browser)
    {
        m_browser = nullptr;
    }
}

bool CodeEditorImpl::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
    Rect viewRect;
    if (m_renderer->GetViewRect(viewRect))
    {
        rect = { viewRect.x, viewRect.y, viewRect.width, viewRect.height };
        return true;
    }

    return false;
}

void CodeEditorImpl::OnPaint(
    CefRefPtr<CefBrowser> /*browser*/,
    PaintElementType /*type*/,
    const RectList& /*dirtyRects*/,
    const void* buffer,
    int width,
    int height)
{
    m_renderer->Render(buffer, width, height);
}