#pragma once
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <set>
extern int SCREENWIDTH;
extern int SCREENHEIGHT;

BOOL SaveToFile(HBITMAP hBitmap, LPCTSTR lpszFileName);
extern bool operator < (const COORD A, const COORD B);
class ScreenShot
{
public:
	//Methods
	ScreenShot()
	{
		//get client DC
		desktophandle = GetDC(NULL);
		//create memory DC
		memHDC = CreateCompatibleDC(desktophandle);
		//create new HBITMAP compatible with client dc
		Teemo = CreateCompatibleBitmap(desktophandle, SCREENWIDTH, SCREENHEIGHT);
		//select bitmap into memory DC
		SelectObject(memHDC, Teemo);
		//copy from client DC to memory DC
		BitBlt(memHDC, 0, 0, SCREENWIDTH, SCREENHEIGHT, desktophandle, 0, 0, SRCCOPY);
		// GetBitmapBits
		long numBits = SCREENWIDTH*SCREENHEIGHT * 4;
		lpvBITBUF = new LPVOID;
		bmpBuffer = (BYTE*)GlobalAlloc(GPTR, SCREENWIDTH*SCREENHEIGHT * 4);
		GetBitmapBits(Teemo, numBits, bmpBuffer);
		//SaveToFile(Teemo, L"images\\Teemo.bmp");
	}
	~ScreenShot()
	{
		ReleaseDC(NULL, desktophandle);
		DeleteDC(memHDC);
		DeleteObject(Teemo);
		delete lpvBITBUF;
		GlobalFree(bmpBuffer);
	}
	std::set<COORD> FindBMP(LPCTSTR testBMP);
private:
	//Methods
	

	//Guts
	HDC desktophandle;
	HDC memHDC;
	HBITMAP Teemo;
	LPVOID lpvBITBUF;
	BYTE* bmpBuffer;
};