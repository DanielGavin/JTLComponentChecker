#pragma once

#include <Windows.h>

extern "C" __declspec(dllexport) LRESULT CALLBACK keyboardProc(int nCode, _In_ WPARAM wParam, LPARAM lParam);


bool getScreenCaptureKeypressAndReset();
bool getCoordCaptureKeypressAndReset();

void installKeyboardHook();
void uninstallKeyboardHook();