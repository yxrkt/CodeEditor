#include "CodeEditor.h"

#include "CodeEditorImpl.h"

CodeEditor::CodeEditor(ICodeEditorRenderer* renderer)
    : m_impl(new CodeEditorImpl(renderer))
{
}

CodeEditor::~CodeEditor()
{
    delete m_impl;
}
