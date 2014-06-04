#include <Windows.h>

HINSTANCE m_hinstance;
HWND m_hwnd;

bool Running = true;

void InitWindows(int width, int height, bool FULL_SCREEN);
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);