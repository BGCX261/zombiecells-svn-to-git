#include <ui/Window.hpp>

using namespace ui;

Window*        Window::m_pSingleInstance = nullptr;
unsigned int   Window::m_nReferences     = 0;
const wchar_t* Window::m_pClassName      = L"Window";

static LRESULT CALLBACK fnWindow(
    HWND   hwnd,
    UINT   eMsg,
    WPARAM wParam,
    LPARAM lParam
    )
{
    Window* pWindow = reinterpret_cast<Window*>(
        GetWindowLongPtrW(hwnd, GWLP_USERDATA)
        );
    if(nullptr != pWindow) {
        switch(eMsg)
        {
        case WM_RBUTTONDOWN:
            pWindow->breakLoop();
            return 0;
        }
    }
    return DefWindowProc(hwnd, eMsg, wParam, lParam);
}

Window::Window()
    : m_hinst   (GetModuleHandle(NULL))
    , m_hwnd    (NULL)
{
    WNDCLASSEXW wcex   = { sizeof(WNDCLASSEX) };
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = fnWindow;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = m_hinst;
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName  = NULL;
    wcex.hCursor       = LoadCursor(NULL, IDI_APPLICATION);
    wcex.lpszClassName = m_pClassName;

    RegisterClassExW(&wcex);

    unsigned int width  = GetSystemMetrics(SM_CXSCREEN);
    unsigned int height = GetSystemMetrics(SM_CYSCREEN);

    m_hwnd = CreateWindowExW(
        WS_EX_TOPMOST,
        m_pClassName,
        L"zombiecells",
        WS_POPUP | WS_VISIBLE,
        0, 0, width, height,
        GetDesktopWindow(),
        NULL,
        m_hinst,
        NULL
        );

    SetWindowLongPtrW(m_hwnd, GWLP_USERDATA, PtrToLong(this));

    m_graphics.m_hwnd = m_hwnd;
    m_graphics.initialize();
}

Window::~Window()
{
    m_graphics.uninitialize();
    UnregisterClass(m_pClassName, m_hinst);
}

Window* Window::instance()
{
    if(nullptr == m_pSingleInstance)
        m_pSingleInstance = new Window;
    m_nReferences++;
    return m_pSingleInstance;
}

void Window::free()
{
    if(0 == --m_nReferences) {
        m_pSingleInstance = nullptr;
        delete this;
    }
}

void Window::doLoop(Idler& idler)
{
    bool repeat = true;
    MSG  msg;

    while(repeat) {
        if(TRUE == PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            if(WM_QUIT == msg.message) {
                repeat = false;
            }
            else {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
        }
        else {
            idler();
        }
    }
}

void Window::breakLoop()
{
    PostQuitMessage(0);
}