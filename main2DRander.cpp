#include "main2DRander.h"
#include "windows.h"
#include "tchar.h"

main2DRander::main2DRander()
{

}

main2DRander::~main2DRander()
{
}


void main2DRander::Create(HWND hWnd)
{
	//백버퍼 사이즈 계산
	HDC hdc = GetDC(hWnd);

	//클라이언트 화면을 구한다
	GetClientRect(hWnd,&m_rectClient);
	
	//화면을 이용해서 그릴 사이즈를 구한다
	m_intWidth = m_rectClient.right - m_rectClient.left;
	m_intHeight = m_rectClient.bottom - m_rectClient.top;

	//DDB DIB의 특성을 가진 DIB Section을 생성한다
	BITMAPINFO bminfo;
	memset(&bminfo , 0 , sizeof(bminfo));

	bminfo.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	bminfo.bmiHeader.biWidth		= m_intWidth;
	bminfo.bmiHeader.biHeight		= -m_intHeight;
	bminfo.bmiHeader.biPlanes		= 1;
	bminfo.bmiHeader.biBitCount		= 32;
	bminfo.bmiHeader.biCompression	= BI_RGB;
	
	//백버를 생성한다
	m_hbmBackBuffer =CreateDIBSection(	hdc ,
										(BITMAPINFO*) &bminfo , 
										DIB_RGB_COLORS,
										(VOID**)&m_hbmBackBuffer,
										NULL,
										0);
	
	// scanline 4의 배수로함
	int ScanLine = ((m_intWidth * 3 + 3) & ~3);

	//백버퍼 관리
	
	//호환 DC를 생성한다
	m_hdcBackbuffer = CreateCompatibleDC(hdc);
	
	//DC에 backbuffer을 설정한 상태를저장한다 나중에 돌려줄것
	m_hbmBackbufferOld = (HBRUSH)SelectObject(m_hdcBackbuffer , m_hbmBackBuffer);
	
	//backbuffer에서 사용할 브러쉬를 생성한다
	m_hbrBackbuffer = (HBRUSH)CreateSolidBrush(RGB(0,0,0));
	
	//DC에 브러쉬를 사용 나중에 돌려줄것 
	m_hbrBackbufferOld =(HBRUSH)SelectObject(m_hdcBackbuffer, m_hbrBackbuffer);

	//DC해제
	ReleaseDC(hWnd,hdc);

}

void main2DRander::Destroy()
{
	//브러쉬
	if(m_hbrBackbuffer)
	{
		//DC에 기존 사용하던 브러쉬를 돌려준다
		SelectObject(m_hdcBackbuffer, m_hbrBackbufferOld);
		
		//제거한다
		DeleteObject(m_hbrBackbuffer);

		m_hbrBackbuffer = NULL;
	}

	//DC
	if(m_hdcBackbuffer) 
	{
		SelectObject(m_hdcBackbuffer, m_hbmBackbufferOld);
		DeleteDC(m_hdcBackbuffer);
		m_hdcBackbuffer = NULL;
	}

	//backbuffer
	if( m_hbmBackBuffer )
	{
		DeleteObject(m_hbmBackBuffer);
		m_hbmBackBuffer = NULL;
	}	
}

void main2DRander::Present(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);

	Present(hdc);

	ReleaseDC(hWnd, hdc);
}


void main2DRander::Present(HDC hdc)
{
	//백버퍼를 이곳에서 출력한다
	BitBlt(hdc,0,0,m_intWidth,m_intHeight,m_hdcBackbuffer,0,0,SRCCOPY);
}


void main2DRander::DrawTextRander(int x, int y, LPCTSTR str )
{
		int len =(int) _tcslen(str);
		TextOut(m_hdcBackbuffer, x,y, str, len);
}

void main2DRander::DrawTextRander(int x, int y, char* str )
{
		int len = (int)strlen(str);
		TextOutA(m_hdcBackbuffer, x,y, str, len);
}




void main2DRander::DrawImage(HBITMAP drawBitmap,int x,int y, int width , int hight )
{
	//이것을 관리할 DC를 생성하고
	HDC hImageDC = CreateCompatibleDC(m_hdcBackbuffer);

	//연결하고
	HBITMAP oldBitmap = (HBITMAP)SelectObject(hImageDC,drawBitmap);

	//이미지를 그릴수있도록한다
	BitBlt(m_hdcBackbuffer ,x, y, width, hight , hImageDC ,0,0,SRCCOPY);

	//다시 dc를 돌려준다
	SelectObject(hImageDC,oldBitmap);

	//dc를 해제한다
	DeleteDC(hImageDC);
}

void main2DRander::DrawImageResize(HBITMAP drawBitmap, int x ,int y, int width , int hight , int cWidth , int cHight)
{

	//이것을 관리할 DC를 생성하고
	HDC hImageDC = CreateCompatibleDC(m_hdcBackbuffer);

	//연결하고
	HBITMAP oldBitmap = (HBITMAP)SelectObject(hImageDC,drawBitmap);

	//이미지를 그릴수있도록한다
	StretchBlt(m_hdcBackbuffer ,x, y, cWidth , cHight  , hImageDC ,0,0, width,hight, SRCCOPY);

	//다시 dc를 돌려준다
	SelectObject(hImageDC,oldBitmap);

	//dc를 해제한다
	DeleteDC(hImageDC);

}


void main2DRander::DrawImageAlpha(HBITMAP drawBitmap, int x ,int y, int width , int hight , int cWidth , int cHight,int alpha)
{

	BLENDFUNCTION ftn;
					 ftn.AlphaFormat = 0;
					 ftn.BlendFlags = 0;
					 ftn.BlendOp = AC_SRC_OVER;
					 ftn.SourceConstantAlpha = alpha;


	//이것을 관리할 DC를 생성하고
	HDC hImageDC = CreateCompatibleDC(m_hdcBackbuffer);

	//연결하고
	HBITMAP oldBitmap = (HBITMAP)SelectObject(hImageDC,drawBitmap);

	//이미지를 그릴수있도록한다
	AlphaBlend(m_hdcBackbuffer ,x, y, cWidth , cHight  , hImageDC ,0,0, width,hight, ftn);

	//다시 dc를 돌려준다
	SelectObject(hImageDC,oldBitmap);

	//dc를 해제한다
	DeleteDC(hImageDC);

}

void main2DRander::Clear(RECT* prc)
{
	//화면을 지운다..
	if( prc == NULL )
	{	
		FillRect(m_hdcBackbuffer , &m_rectClient, NULL);
	}
	else
	{
		FillRect(m_hdcBackbuffer , prc, NULL);
	}
}

