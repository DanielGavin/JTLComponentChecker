#include "capture.h"

#include <leptonica/allheaders.h>
#include <Windows.h>

void captureScreen(std::string screenName)
{
	(void)screenName;
	//Taken from the win32 example
	//What a mess

	HDC hdc = GetDC(0);
	HDC hScreenDC = CreateDC("DISPLAY", NULL, NULL, NULL);     
	HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

	int width = GetDeviceCaps(hScreenDC, HORZRES);
	int height = GetDeviceCaps(hScreenDC, VERTRES);

	HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);

	HBITMAP hOldBitmap = (HBITMAP) SelectObject(hMemoryDC, hBitmap);

	BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);

	hBitmap = (HBITMAP) SelectObject(hMemoryDC, hOldBitmap);

	BITMAP bmpScreen;

	GetObject(hBitmap, sizeof(BITMAP), &bmpScreen);

	BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmpScreen.bmWidth;
    bi.biHeight = bmpScreen.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

	int size = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

	char* data = new char[size];

	memset(data, 0, size);

	if (GetDIBits(hdc, hBitmap, 0, bmpScreen.bmHeight, (LPVOID)data, (BITMAPINFO*)&bi, DIB_RGB_COLORS) == 0)
	{
		return;
	}

	HANDLE hFile = CreateFile("capture.bmp",
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);

    // Add the size of the headers to the size of the bitmap to get the total file size.
    int dwSizeofDIB = size + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	BITMAPFILEHEADER   bmfHeader;

    // Offset to where the actual bitmap bits start.
    bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

    // Size of the file.
    bmfHeader.bfSize = dwSizeofDIB;

    // bfType must always be BM for Bitmaps.
    bmfHeader.bfType = 0x4D42; // BM.

	DWORD dwBytesWritten;

    WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)data, size, &dwBytesWritten, NULL);

	CloseHandle(hFile);

	DeleteObject(hScreenDC);
    DeleteObject(hMemoryDC);
    ReleaseDC(NULL, hdc);
}