// Author: Lawrence Bradley Howard
// FileName: main.cpp
/* Description: This is the entry point to the Feather Engine, Windows is instantiated
				here and the Main Loop is accessible.
				Windows is created in the main.h Header file.*/
#include "main.h"
#include "GFX.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	InitWindows(800, 600, false);

	GFX m_GFX = GFX(m_hwnd);

	// Main Loop
	MSG msg_Message;
	while (Running)
	{
		m_GFX.ClearScene(FColour(1.0f, 0.0f, 0.0f, 1.0f));
		m_GFX.Draw();

		//Check for quit
		if(PeekMessage(&msg_Message,m_hwnd,0,0,PM_REMOVE))
         {
             if(!IsDialogMessage(m_hwnd,&msg_Message))
             {
                 DispatchMessage(&msg_Message);
             }
         }
	}

	m_GFX.CleanD3D();
	DestroyWindow(m_hwnd);

	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
     {
         case WM_KEYDOWN:
         {
             Running = false;
             break;
         }
         break;
     }

	return DefWindowProc(hwnd, umessage, wparam, lparam);
}

void InitWindows(int width, int height, bool FULL_SCREEN)
{
	WNDCLASSEX wc;
	int posX, posY;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Setup the windows class with default settings.
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hinstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = L"Feather Engine";
	wc.cbSize        = sizeof(WNDCLASSEX);
	
	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	int screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Place the window in the middle of the screen.
	posX = (screenWidth - width)  / 2;
	posY = (screenHeight - height) / 2;

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_CONTROLPARENT, L"Feather Engine", L"Feather Engine",
							WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
							posX, posY, width, height, NULL, NULL, m_hinstance, NULL);

	//// Bring the window up on the screen and set it as main focus.
	//ShowWindow(m_hwnd, SW_SHOW);
	//SetForegroundWindow(m_hwnd);
	//SetFocus(m_hwnd);

	//// Hide the mouse cursor.
	//ShowCursor(false);

	return;
}