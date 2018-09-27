#include "CodeEditor.h"

#include "CodeEditorImpl.h"
#include "EnumHelper.h"

template <> struct is_flags_enum<cef_event_flags_t> { static const bool value = true; };
template <> struct is_flags_enum<Modifiers> { static const bool value = true; };

CodeEditor::CodeEditor(ICodeEditorRenderer* renderer)
    : m_impl(new CodeEditorImpl(renderer))
{
}

CodeEditor::~CodeEditor()
{
    delete m_impl;
}

void CodeEditor::OnWindowResized()
{
    if (CefRefPtr<CefBrowser> browser = m_impl->GetBrowser())
    {
        browser->GetHost()->WasResized();
    }
}

void CodeEditor::OnMouseMove(int x, int y, bool mouseLeave)
{
    if (CefRefPtr<CefBrowser> browser = m_impl->GetBrowser())
    {
        CefMouseEvent e;
        e.x = x;
        e.y = y;
        e.modifiers = 0;
        browser->GetHost()->SendMouseMoveEvent(e, mouseLeave);
    }
}

static cef_mouse_button_type_t GetCefMouseButton(MouseButton button)
{
    switch (button)
    {
    case MouseButton::Right:
        return cef_mouse_button_type_t::MBT_RIGHT;
    case MouseButton::Middle:
        return cef_mouse_button_type_t::MBT_MIDDLE;
    default:
        return cef_mouse_button_type_t::MBT_LEFT;
    }
}

void CodeEditor::OnMouseClick(int x, int y, MouseButton button, bool down, int clickCount)
{
    if (CefRefPtr<CefBrowser> browser = m_impl->GetBrowser())
    {
        CefMouseEvent e;
        e.x = x;
        e.y = y;
        e.modifiers = 0;

        browser->GetHost()->SendMouseClickEvent(e, GetCefMouseButton(button), !down, clickCount);
    }
}

void CodeEditor::OnMouseWheel(int x, int y, int deltaX, int deltaY)
{
    if (CefRefPtr<CefBrowser> browser = m_impl->GetBrowser())
    {
        CefMouseEvent e;
        e.x = x;
        e.y = y;
        e.modifiers = 0;

        browser->GetHost()->SendMouseWheelEvent(e, deltaX, deltaY);
    }
}

static cef_key_event_type_t GetCefKeyEventType(KeyEventType type)
{
    switch (type)
    {
    case KeyEventType::Up:
        return cef_key_event_type_t::KEYEVENT_KEYUP;
    case KeyEventType::Character:
        return cef_key_event_type_t::KEYEVENT_CHAR;
    default:
        return cef_key_event_type_t::KEYEVENT_KEYDOWN;
    }
}

static cef_event_flags_t GetCefModifiers(Modifiers modifiers)
{
    cef_event_flags_t flags = cef_event_flags_t::EVENTFLAG_NONE;

    if (has_flags(modifiers, Modifiers::Ctrl))
    {
        flags |= cef_event_flags_t::EVENTFLAG_CONTROL_DOWN;
    }

    if (has_flags(modifiers, Modifiers::Alt))
    {
        flags |= cef_event_flags_t::EVENTFLAG_ALT_DOWN;
    }

    if (has_flags(modifiers, Modifiers::Shift))
    {
        flags |= cef_event_flags_t::EVENTFLAG_SHIFT_DOWN;
    }

    return flags;
}

static bool IsKeyDown(int key)
{
    return (GetKeyState(key) & 0x8000) != 0;
}

// Copied from
// https://bitbucket.org/chromiumembedded/branches-2062-cef3/src/a073e92426b3967f1fc2f1d3fd7711d809eeb03a/tests/cefclient/cefclient_osr_widget_win.cpp?at=master#cl-361
static cef_event_flags_t GetCefKeyboardModifiers(int lParam, int wParam)
{
    int modifiers = 0;
    if (IsKeyDown(VK_SHIFT))
        modifiers |= EVENTFLAG_SHIFT_DOWN;
    if (IsKeyDown(VK_CONTROL))
        modifiers |= EVENTFLAG_CONTROL_DOWN;
    if (IsKeyDown(VK_MENU))
        modifiers |= EVENTFLAG_ALT_DOWN;

    // Low bit set from GetKeyState indicates "toggled".
    if (::GetKeyState(VK_NUMLOCK) & 1)
        modifiers |= EVENTFLAG_NUM_LOCK_ON;
    if (::GetKeyState(VK_CAPITAL) & 1)
        modifiers |= EVENTFLAG_CAPS_LOCK_ON;

    switch (wParam)
    {
    case VK_RETURN:
        if ((lParam >> 16) & KF_EXTENDED)
            modifiers |= EVENTFLAG_IS_KEY_PAD;
        break;
    case VK_INSERT:
    case VK_DELETE:
    case VK_HOME:
    case VK_END:
    case VK_PRIOR:
    case VK_NEXT:
    case VK_UP:
    case VK_DOWN:
    case VK_LEFT:
    case VK_RIGHT:
        if (!((lParam >> 16) & KF_EXTENDED))
            modifiers |= EVENTFLAG_IS_KEY_PAD;
        break;
    case VK_NUMLOCK:
    case VK_NUMPAD0:
    case VK_NUMPAD1:
    case VK_NUMPAD2:
    case VK_NUMPAD3:
    case VK_NUMPAD4:
    case VK_NUMPAD5:
    case VK_NUMPAD6:
    case VK_NUMPAD7:
    case VK_NUMPAD8:
    case VK_NUMPAD9:
    case VK_DIVIDE:
    case VK_MULTIPLY:
    case VK_SUBTRACT:
    case VK_ADD:
    case VK_DECIMAL:
    case VK_CLEAR:
        modifiers |= EVENTFLAG_IS_KEY_PAD;
        break;
    case VK_SHIFT:
        if (IsKeyDown(VK_LSHIFT))
            modifiers |= EVENTFLAG_IS_LEFT;
        else if (IsKeyDown(VK_RSHIFT))
            modifiers |= EVENTFLAG_IS_RIGHT;
        break;
    case VK_CONTROL:
        if (IsKeyDown(VK_LCONTROL))
            modifiers |= EVENTFLAG_IS_LEFT;
        else if (IsKeyDown(VK_RCONTROL))
            modifiers |= EVENTFLAG_IS_RIGHT;
        break;
    case VK_MENU:
        if (IsKeyDown(VK_LMENU))
            modifiers |= EVENTFLAG_IS_LEFT;
        else if (IsKeyDown(VK_RMENU))
            modifiers |= EVENTFLAG_IS_RIGHT;
        break;
    case VK_LWIN:
        modifiers |= EVENTFLAG_IS_LEFT;
        break;
    case VK_RWIN:
        modifiers |= EVENTFLAG_IS_RIGHT;
        break;
    }

    return (cef_event_flags_t)modifiers;
}

void CodeEditor::OnKey(int message, int wParam, int lParam)
{
    if (CefRefPtr<CefBrowser> browser = m_impl->GetBrowser())
    {
        CefKeyEvent keyEvent;
        keyEvent.windows_key_code = wParam;
        keyEvent.native_key_code = lParam;
        keyEvent.is_system_key = message == WM_SYSCHAR ||
            message == WM_SYSKEYDOWN ||
            message == WM_SYSKEYUP;

        if (message == WM_KEYDOWN || message == WM_SYSKEYDOWN)
        {
            keyEvent.type = KEYEVENT_RAWKEYDOWN;
        }
        else if (message == WM_KEYUP || message == WM_SYSKEYUP)
        {
            keyEvent.type = KEYEVENT_KEYUP;
        }
        else
        {
            keyEvent.type = KEYEVENT_CHAR;
        }

        keyEvent.modifiers = GetCefKeyboardModifiers(wParam, lParam);

        browser->GetHost()->SendKeyEvent(keyEvent);
    }
}
