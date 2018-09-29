#pragma once

enum class MouseCursorType
{
    Pointer,
    Cross,
    Hand,
    IBeam,
    Wait,
    Help,
    ResizeEast,
    ResizeNorth,
    ResizeNorthEast,
    ResizeNorthWest,
    ResizeSouth,
    ResizeSouthEast,
    ResizeSouthWest,
    ResizeWest,
    ResizeNorthSouth,
    ResizeEastWest,
    ResizeNorthEastSouthWest,
    ResizeNorthWestSouthEast,
    ResizeColumn,
    ResizeRow,
    PanningMiddle,
    PanningEast,
    PanningNorth,
    PanningNorthEast,
    PanningNorthWest,
    PanningSouth,
    PanningSouthEast,
    PanningSouthWest,
    PanningWest,
    Move,
    VerticalText,
    Cell,
    ContextMenu,
    Alias,
    Progress,
    NoDrop,
    Copy,
    None,
    NotAllowed,
    Zoomin,
    Zoomout,
    Grab,
    Grabbing
};

class ICodeEditorEventHandler
{
public:
    virtual void OnMouseCursorChanged(MouseCursorType cursorType) = 0;
    virtual void OnSaved(const char* text) = 0;
};
