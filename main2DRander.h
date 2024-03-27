#pragma once
#pragma comment (lib, "Msimg32.lib")
#include "windows.h"

class main2DRander
{
protected:
	HBITMAP m_hbmBackBuffer;

	
	HBRUSH	m_hbmBackbufferOld;
	HBRUSH	m_hbrBackbuffer;
	HBRUSH	m_hbrBackbufferOld;
	HDC		m_hdcBackbuffer;

	int		m_intWidth;
	int		m_intHeight;

	RECT	m_rectClient;

public:
	
	main2DRander();
	~main2DRander();

	void Create(HWND hWnd);
	void Destroy();

	void Present(HWND hWnd);
	void Present(HDC hdc);

	void DrawTextRander(int x, int y, LPCTSTR str);
	void DrawTextRander(int x, int y, char* str);

	
	void DrawImage(HBITMAP drawBitmap, int x,int y , int width , int hight);
	void DrawImageResize(HBITMAP drawBitmap, int x,int y, int width , int hight , int cWidth , int cHight);
	void DrawImageAlpha(HBITMAP drawBitmap, int x ,int y, int width , int hight , int cWidth , int cHight , int alpha);

	void Clear(RECT* prc);

};