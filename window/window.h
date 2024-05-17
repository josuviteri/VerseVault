//
// Created by asier on 14/05/2024.
//
#ifndef WINDOW_H
#define WINDOW_H
#include <Windows.h>
#include <string>
#include <vector>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, const std::vector<std::string>& lineas);

class window {
public:
    window(const std::vector<std::string>& lineas);
    window(const window&) = delete;
    window& operator = (const window&) = delete;
    ~window();

    bool processMessages();
    std::vector<std::string> m_lineas;
private:
    HINSTANCE m_hIstance;
    HWND m_hWnd;

};



#endif //WINDOW_H
