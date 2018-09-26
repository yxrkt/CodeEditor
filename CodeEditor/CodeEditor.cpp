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

void CodeEditor::OnKey(KeyEventType type, Modifiers modifiers, int keyCode, char character)
{
    if (CefRefPtr<CefBrowser> browser = m_impl->GetBrowser())
    {
        CefKeyEvent e;
        e.type = GetCefKeyEventType(type);
        e.modifiers = GetCefModifiers(modifiers);
        e.windows_key_code = keyCode;
        e.native_key_code = keyCode;
        e.is_system_key = false;
        e.character = character;
        e.character = character;
        e.focus_on_editable_field = true;

        browser->GetHost()->SendKeyEvent(e);
    }
}
