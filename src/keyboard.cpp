#include "keyboard.h"
#include <iostream>
#include "capture.h"
#include "options.h"

static bool capturekeyPressed = false;
static bool captureCoordinate = false;

static HHOOK hook;

extern Options options;

LRESULT CALLBACK keyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		KBDLLHOOKSTRUCT *kb = (KBDLLHOOKSTRUCT *)lParam;

		if (wParam == WM_KEYDOWN)
		{
			unsigned char key = MapVirtualKeyA(kb->vkCode, MAPVK_VK_TO_CHAR);

			if (key == options.executeKey[0] && (!options.executeKeyShiftModifier || GetKeyState(VK_SHIFT) & 0x8000))
			{
				capturekeyPressed = true;
			}
			else if (key == options.recordKey[0] && (!options.recordKeyShiftModifier || GetKeyState(VK_SHIFT) & 0x8000))
			{
				captureCoordinate = true;
			}
		}
	}

	return CallNextHookEx(0, nCode, wParam, lParam);
}

bool getScreenCaptureKeypressAndReset()
{
	bool tmp = capturekeyPressed;
	capturekeyPressed = false;
	return tmp;
}

bool getCoordCaptureKeypressAndReset()
{
	bool tmp = captureCoordinate;
	captureCoordinate = false;
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
