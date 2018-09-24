#include "..\CodeEditor\CodeEditor.h"
#include "..\CodeEditor\ICodeEditorRenderer.h"

#include <Windows.h>

#include <iomanip>
#include <iostream>

class CodeEditorRenderer : public ICodeEditorRenderer
{
    virtual bool GetViewRect(Rect& rect) override
    {
        rect = { 0, 0, 1280, 720 };
        return true;
    }

    virtual void Render(const void* buffer, int width, int height) override
    {
        std::cout << "color: #" << std::hex << std::setfill('0') << std::setw(8) << ((int*)buffer)[0] << std::endl << std::dec;
        std::cout << "width: " << width << std::endl;
        std::cout << "height: " << height << std::endl;
    }
};

int main()
{
    CodeEditorRenderer renderer;
    CodeEditor editor(&renderer);

    MSG message;
    ZeroMemory(&message, sizeof(message));
    while (GetMessage(&message, NULL, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}