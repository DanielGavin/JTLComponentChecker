#include "keyboard.h"
#include <iostream>
#include "capture.h"

static bool keyPressed = false;

static HHOOK hook;

LRESULT CALLBACK keyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		KBDLLHOOKSTRUCT *kb = (KBDLLHOOKSTRUCT *)lParam;

		if (wParam == WM_KEYDOWN)
		{
			unsigned char key = MapVirtualKeyA(kb->vkCode, MAPVK_VK_TO_CHAR);

			if (key == '.')
			{
				keyPressed = true;
			}
		}
	}

	return CallNextHookEx(0, nCode, wParam, lParam);
}

bool getKeypressAndReset()
{
	bool tmp = keyPressed;
	keyPressed = false;
	return tmp;
}

void installKeyboardHook()
{
	hook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardProc, NULL, 0);
}

void uninstallKeyboardHook()
{
	UnhookWindowsHookEx(hook);
}
