#pragma once
#include "windows.h"
#include "tchar.h"

#include "main2DRander.h"
#include "./block/system.h"
#include "resource1.h"
#include "commctrl.h"

struct NETDATA
{
	char type;
	char message[10];
	int  Map[HEIGHT][WIDTH];
	int  AttackLine;
	NETDATA();

};

class mainFrame : public CSystem
{
protected:

	HINSTANCE		m_hInst;
	HWND			m_hWnd;
	bool			m_runFlag;
	main2DRander*	m_pMain2DRander;

	//사용될 이미지
	HBITMAP m_hbmImageBitmap[10];
	
	//키 버퍼
	int m_KeyBuffer[255];

	DWORD m_tick;


protected:
	mainFrame();
	~mainFrame();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	
public:
	DWORD m_IPAddress;

	int m_netMap[HEIGHT][WIDTH];

	char* m_PrintText;

	
	BOOL m_Sleep;


	//////////////////////////////////////////
	static mainFrame& getMainFrame();
	BOOL Create(HINSTANCE hInst);
	void Run();

	bool NetConnect();

	//////////////////////////////////////////
	LRESULT CALLBACK MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	

	//////////////////////////////////////////
	bool Fps(DWORD frame);
	void Draw(DWORD dTime);
	void Update(DWORD dTime);
	void LoadImageFile();

	void DrawImage(DWORD dTime);
	void DrawText(DWORD dTime);


	////////////////////////////////////////
	// cblock overloading
	void ChangeBlock(DWORD dTime);

	//////////////////////////////////////
	DWORD differentTime();

};



DWORD WINAPI ServerThreadProc(LPVOID lpParam);
DWORD WINAPI ClientThreadProc(LPVOID lpParam);

INT_PTR CALLBACK Connect(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	
