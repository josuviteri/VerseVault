//
// Created by asier on 14/05/2024.
//

#include "window.h"
#include <string>

std::vector<std::string> lineas;


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    window* pWindow = reinterpret_cast<window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    std::vector<LPCSTR> lpcstrLineas;

    // Convierte cada línea a LPCSTR y guárdala en el vector
    for (const auto& linea : pWindow-> m_lineas) {
        // Convierte la cadena de caracteres de std::string a LPCSTR usando c_str()
        LPCSTR lpcstrLinea = linea.c_str();
        // Agrega el LPCSTR al vector
        lpcstrLineas.push_back(lpcstrLinea);
    }
    // Concatena todas las líneas en una sola cadena
    std::string textoConcatenado;
    for (const auto& lpcstrLinea : lpcstrLineas) {
        textoConcatenado += lpcstrLinea;
        textoConcatenado += "\n"; // Agrega un salto de línea entre cada línea
    }
    LPCSTR lpcstrTexto = textoConcatenado.c_str();

    switch (uMsg) {
        case WM_CREATE:
            CreateWindow(TEXT("STATIC"), TEXT(lpcstrTexto),
                WS_VISIBLE | WS_CHILD,
                10, 10, 100, 25,
                hWnd, (HMENU) NULL, NULL, NULL);
            break;
        case WM_CLOSE:
            DestroyWindow(hWnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

window::window(const std::vector<std::string>& lineas) : m_hIstance(GetModuleHandle(nullptr))
{
    this->m_lineas = lineas;
    LPCSTR CLASS_NAME = "VerseVault Window Class";

    WNDCLASS wndClass = {};
    wndClass.lpszClassName = CLASS_NAME;
    wndClass.hInstance = m_hIstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.lpfnWndProc = WindowProc;

    RegisterClass(&wndClass);

    DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

    int width = 640;
    int height = 480;

    RECT rect;
    rect.left = 250;
    rect.top = 250;
    rect.right = rect.left + width;
    rect.bottom = rect.top + height;

    AdjustWindowRect(&rect, style, false);

    m_hWnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "VerseVault",
        style,
        rect.left,
        rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top,
        NULL,
        NULL,
        m_hIstance,
        NULL
    );

    SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    ShowWindow(m_hWnd, SW_SHOW);
}

window::~window() {
    LPCSTR CLASS_NAME = "VerseVault Window Class";
    UnregisterClass(CLASS_NAME, m_hIstance);
}

bool window::processMessages() {
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) {

        if (msg.message == WM_QUIT) {
            return false;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);

    }
    return true;
}
