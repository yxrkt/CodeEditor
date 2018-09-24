#pragma once

struct Rect
{
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
};

class ICodeEditorRenderer
{
public:
    virtual bool GetViewRect(Rect& rect) = 0;
    virtual void Render(const void* buffer, int width, int height) = 0;
};