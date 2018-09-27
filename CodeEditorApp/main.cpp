#include "..\CodeEditor\CodeEditor.h"
#include "..\CodeEditor\ICodeEditorRenderer.h"

#include <Windows.h>
#include <Windowsx.h>

class CodeEditorRenderer : public ICodeEditorRenderer
{
public:
    CodeEditorRenderer(HWND window)
        : m_window(window)
        , m_hdc(CreateCompatibleDC(GetDC(window)))
    {
        RECT r;
        if (GetClientRect(window, &r))
        {
            m_bufferWidth = r.right;
            m_bufferHeight = r.bottom;

            OnResize();
        }
        else
        {
            m_bufferWidth = 0;
            m_bufferHeight = 0;
            m_bitmap = NULL;
        }
    }

    virtual bool GetViewRect(Rect& rect) override
    {
        RECT r;
        if (GetClientRect(m_window, &r))
        {
            rect = { r.left, r.top, r.right - r.left, r.bottom - r.top };
            return true;
        }

        return false;
    }

    virtual void Render(const void* buffer, int width, int height) override
    {
        if (width != m_bufferWidth || height != m_bufferHeight)
        {
            m_bufferWidth = width;
            m_bufferHeight = height;
            OnResize();
        }

        SetDIBits(m_hdc, m_bitmap, 0, height, buffer, &m_bitmapInfo, DIB_RGB_COLORS);
        InvalidateRect(m_window, NULL, FALSE);
    }

    int GetBufferWidth() const
    {
        return m_bufferWidth;
    }

    int GetBufferHeight() const
    {
        return m_bufferHeight;
    }

    HDC GetHdc() const
    {
        return m_hdc;
    }

private:
    void OnResize()
    {
        m_bitmapInfo.bmiHeader.biSize = sizeof(m_bitmapInfo.bmiHeader);
        m_bitmapInfo.bmiHeader.biWidth = m_bufferWidth;
        m_bitmapInfo.bmiHeader.biHeight = -m_bufferHeight;
        m_bitmapInfo.bmiHeader.biPlanes = 1;
        m_bitmapInfo.bmiHeader.biBitCount = 32;
        m_bitmapInfo.bmiHeader.biCompression = BI_RGB;
        m_bitmapInfo.bmiHeader.biSizeImage = 0;
        m_bitmapInfo.bmiHeader.biXPelsPerMeter = 3780;
        m_bitmapInfo.bmiHeader.biYPelsPerMeter = 3780;
        m_bitmapInfo.bmiHeader.biClrUsed = 0;
        m_bitmapInfo.bmiHeader.biClrImportant = 0;
        ZeroMemory(&m_bitmapInfo.bmiColors, sizeof(m_bitmapInfo.bmiColors));

        m_bitmap = CreateCompatibleBitmap(GetDC(m_window), m_bufferWidth, m_bufferHeight);

        SelectObject(m_hdc, m_bitmap);
    }

private:
    HWND m_window;
    HDC m_hdc;
    BITMAPINFO m_bitmapInfo;
    HBITMAP m_bitmap;
    int m_bufferWidth;
    int m_bufferHeight;
};

struct CodeEditorAndRenderer
{
    CodeEditor* m_codeEditor;
    CodeEditorRenderer* m_renderer;
};

LRESULT CALLBACK MessageCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    auto editorAndRenderer = (CodeEditorAndRenderer*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    auto editor = editorAndRenderer != nullptr ? editorAndRenderer->m_codeEditor : nullptr;
    auto renderer = editorAndRenderer != nullptr ? editorAndRenderer->m_renderer : nullptr;

    if (editor != nullptr &&
        message >= WM_MOUSEFIRST && message <= WM_MOUSELAST)
    {
        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);

        switch (message)
        {
        case WM_MOUSEMOVE:
            editor->OnMouseMove(x, y, false);
            return 0;
        case WM_LBUTTONUP:
            editor->OnMouseClick(x, y, MouseButton::Left, false, 0);
            return 0;
        case WM_LBUTTONDOWN:
            editor->OnMouseClick(x, y, MouseButton::Left, true, 1);
            return 0;
        case WM_LBUTTONDBLCLK:
            editor->OnMouseClick(x, y, MouseButton::Left, true, 2);
            return 0;
        case WM_RBUTTONUP:
            editor->OnMouseClick(x, y, MouseButton::Right, false, 0);
            return 0;
        case WM_RBUTTONDOWN:
            editor->OnMouseClick(x, y, MouseButton::Right, true, 1);
            return 0;
        case WM_RBUTTONDBLCLK:
            editor->OnMouseClick(x, y, MouseButton::Right, true, 2);
            return 0;
        case WM_MBUTTONUP:
            editor->OnMouseClick(x, y, MouseButton::Middle, false, 0);
            return 0;
        case WM_MBUTTONDOWN:
            editor->OnMouseClick(x, y, MouseButton::Middle, true, 1);
            return 0;
        case WM_MBUTTONDBLCLK:
            editor->OnMouseClick(x, y, MouseButton::Middle, true, 2);
            return 0;
        case WM_MOUSEHWHEEL:
            editor->OnMouseWheel(x, y, GET_WHEEL_DELTA_WPARAM(wParam), 0);
            return 0;
        case WM_MOUSEWHEEL:
            editor->OnMouseWheel(x, y, 0, GET_WHEEL_DELTA_WPARAM(wParam));
            return 0;
        }

        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    if (editor != nullptr &&
        message >= WM_KEYFIRST && message <= WM_KEYLAST)
    {
        editor->OnKey(message, (int)wParam, (int)lParam);
        return 0;
    }

    switch (message)
    {
    case WM_SIZE:
        if (editor != nullptr)
        {
            editor->OnWindowResized();
            InvalidateRect(hwnd, NULL, FALSE);
        }

        return 0;
    case WM_PAINT:
    {
        if (renderer != nullptr)
        {
            PAINTSTRUCT ps;
            auto hdc = BeginPaint(hwnd, &ps);
            BitBlt(
                hdc,
                0,
                0,
                renderer->GetBufferWidth(),
                renderer->GetBufferHeight(),
                renderer->GetHdc(),
                0,
                0,
                SRCCOPY);
            EndPaint(hwnd, &ps);
            return 0;
        }

        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASS window_class;
    ZeroMemory(&window_class, sizeof(window_class));
    window_class.lpfnWndProc = MessageCallback;
    window_class.hInstance = hInstance;
    window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    window_class.lpszClassName = "CodeEditorSampleWindow";
    window_class.style = CS_DBLCLKS;

    if (!RegisterClass(&window_class)) { return (int)GetLastError(); }

    HWND window_handle = CreateWindow(
        window_class.lpszClassName,
        "CodeEditorSample",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL);

    CodeEditorRenderer renderer(window_handle);
    CodeEditor editor(&renderer);

    CodeEditorAndRenderer editorAndRenderer = { &editor, &renderer };

    SetWindowLongPtr(window_handle, GWLP_USERDATA, (LONG_PTR)&editorAndRenderer);

    MSG message;
    ZeroMemory(&message, sizeof(message));
    while (GetMessage(&message, NULL, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return (int)message.wParam;
}