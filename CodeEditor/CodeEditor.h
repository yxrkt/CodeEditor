#pragma once

class ICodeEditorRenderer;

class CodeEditor
{
public:
    CodeEditor(ICodeEditorRenderer* renderer);
    ~CodeEditor();

private:
    class CodeEditorImpl* m_impl;
};