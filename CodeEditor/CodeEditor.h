#pragma once

class ICodeEditorRenderer;

enum class MouseButton
{
    Left,
    Right, 
    Middle
};

enum class KeyEventType
{
    Down,
    Up,
    Character
};

enum class Modifiers
{
    None = 0,
    Ctrl = 1 << 0,
    Alt = 1 << 1,
    Shift = 1 << 2
};

class CodeEditor
{
public:
    CodeEditor(ICodeEditorRenderer* renderer);
    ~CodeEditor();

    void OnWindowResized();
    void OnMouseMove(int x, int y, bool mouseLeave);
    void OnMouseClick(int x, int y, MouseButton button, bool down, int clickCount);
    void OnMouseWheel(int x, int y, int deltaX, int deltaY);

    void OnKey(int message, int wParam, int lParam);

private:
    class CodeEditorImpl* m_impl;
};