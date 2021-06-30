#include "overlay.h"

#include <iostream>

static char swgName[] = "Star Wars Galaxies";

static HWND SwgTargetWindow = nullptr;
static HWND overlayWindow = nullptr;

static bool backgroundCleared = false;
static HBRUSH backgroundBrush; 

extern WNDCLASSEX wc;

static std::string componentString;

static bool overlayChanged = true;

// Win32 message handler
LRESULT WINAPI overlayWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

void findSWGWindow()
{
	SwgTargetWindow = GetForegroundWindow();

	if (!SwgTargetWindow)
	{
		return;
	}

	char buffer[255];

	memset(buffer, 0, 255);

	GetWindowTextA(SwgTargetWindow, buffer, 255);

	if (strncmp(buffer, swgName, sizeof(swgName) - 1) != 0)
	{
		SwgTargetWindow = nullptr;
	}

}

bool createOverlay()
{
	int width = 200;
	int height = 200;

	backgroundBrush = CreateSolidBrush(RGB(0, 0, 0));

	WNDCLASSEX wcex = {0};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = overlayWindowProc;
	wcex.hInstance = wc.hInstance;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = backgroundBrush;
	wcex.lpszClassName = "overlay";
	wcex.lpszMenuName = "overlay";

	if (RegisterClassEx(&wcex) == 0)
	{
		return false;
	}

	overlayWindow = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW, "overlay", "overlay", WS_POPUP, 1, 1, width, height, nullptr, nullptr, wc.hInstance, nullptr);

	if (!overlayWindow)
	{
		return false;
	}

	SetLayeredWindowAttributes(overlayWindow, RGB(0, 0, 0), 0, LWA_COLORKEY);

	ShowWindow(overlayWindow, true);

	return true;
}

void updateOverlay()
{
	findSWGWindow();

	if (!overlayChanged && SwgTargetWindow)
	{
		return;
	}

	if (SwgTargetWindow)
	{
		RECT rect;
		GetWindowRect(SwgTargetWindow, &rect);
		MoveWindow(overlayWindow, rect.left, rect.top, rect.right - rect.left, (rect.bottom - rect.top) / 2, true);

		HDC dc = GetDC(overlayWindow);
		RECT rc;
		GetClientRect(overlayWindow, &rc);

		DrawText(dc, componentString.c_str(), -1, &rc, DT_TOP | DT_LEFT);

		ReleaseDC(overlayWindow, dc);
		backgroundCleared = false;
	}
	else if (!backgroundCleared)
	{	
		HDC dc = GetDC(overlayWindow);
		RECT rc;
		GetClientRect(overlayWindow, &rc);
		FillRect(dc, &rc, backgroundBrush);
		ReleaseDC(overlayWindow, dc);
		backgroundCleared = true;
	}
}

void changedOverlay()
{
	overlayChanged = true;
}

void setComponentStringOverlay(const std::string& str)
{
	componentString = str;
}
