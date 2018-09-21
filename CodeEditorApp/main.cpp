#include "..\CodeEditor\CodeEditor.h"
#include "..\CodeEditor\ICodeEditorRenderer.h"

#include <iostream>

class CodeEditorRenderer : public ICodeEditorRenderer
{
    virtual void Render(const void* buffer, int width, int height) override
    {
        std::cout << "width: " << width << std::endl;
        std::cout << "height: " << height << std::endl;
    }
};

int main()
{
    CodeEditorRenderer renderer;

    CodeEditor editor(&renderer);

    getchar();
}