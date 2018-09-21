#pragma once

class ICodeEditorRenderer
{
public:
    virtual void Render(const void* buffer, int width, int height) = 0;
};