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
	//����� ������ ���
	HDC hdc = GetDC(hWnd);

	//Ŭ���̾�Ʈ ȭ���� ���Ѵ�
	GetClientRect(hWnd,&m_rectClient);
	
	//ȭ���� �̿��ؼ� �׸� ����� ���Ѵ�
	m_intWidth = m_rectClient.right - m_rectClient.left;
	m_intHeight = m_rectClient.bottom - m_rectClient.top;

	//DDB DIB�� Ư���� ���� DIB Section�� �����Ѵ�
	BITMAPINFO bminfo;
	memset(&bminfo , 0 , sizeof(bminfo));

	bminfo.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	bminfo.bmiHeader.biWidth		= m_intWidth;
	bminfo.bmiHeader.biHeight		= -m_intHeight;
	bminfo.bmiHeader.biPlanes		= 1;
	bminfo.bmiHeader.biBitCount		= 32;
	bminfo.bmiHeader.biCompression	= BI_RGB;
	
	//����� �����Ѵ�
	m_hbmBackBuffer =CreateDIBSection(	hdc ,
										(BITMAPINFO*) &bminfo , 
										DIB_RGB_COLORS,
										(VOID**)&m_hbmBackBuffer,
										NULL,
										0);
	
	// scanline 4�� �������
	int ScanLine = ((m_intWidth * 3 + 3) & ~3);

	//����� ����
	
	//ȣȯ DC�� �����Ѵ�
	m_hdcBackbuffer = CreateCompatibleDC(hdc);
	
	//DC�� backbuffer�� ������ ���¸������Ѵ� ���߿� �����ٰ�
	m_hbmBackbufferOld = (HBRUSH)SelectObject(m_hdcBackbuffer , m_hbmBackBuffer);
	
	//backbuffer���� ����� �귯���� �����Ѵ�
	m_hbrBackbuffer = (HBRUSH)CreateSolidBrush(RGB(0,0,0));
	
	//DC�� �귯���� ��� ���߿� �����ٰ� 
	m_hbrBackbufferOld =(HBRUSH)SelectObject(m_hdcBackbuffer, m_hbrBackbuffer);

	//DC����
	ReleaseDC(hWnd,hdc);

}

void main2DRander::Destroy()
{
	//�귯��
	if(m_hbrBackbuffer)
	{
		//DC�� ���� ����ϴ� �귯���� �����ش�
		SelectObject(m_hdcBackbuffer, m_hbrBackbufferOld);
		
		//�����Ѵ�
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
	//����۸� �̰����� ����Ѵ�
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
	//�̰��� ������ DC�� �����ϰ�
	HDC hImageDC = CreateCompatibleDC(m_hdcBackbuffer);

	//�����ϰ�
	HBITMAP oldBitmap = (HBITMAP)SelectObject(hImageDC,drawBitmap);

	//�̹����� �׸����ֵ����Ѵ�
	BitBlt(m_hdcBackbuffer ,x, y, width, hight , hImageDC ,0,0,SRCCOPY);

	//�ٽ� dc�� �����ش�
	SelectObject(hImageDC,oldBitmap);

	//dc�� �����Ѵ�
	DeleteDC(hImageDC);
}

void main2DRander::DrawImageResize(HBITMAP drawBitmap, int x ,int y, int width , int hight , int cWidth , int cHight)
{

	//�̰��� ������ DC�� �����ϰ�
	HDC hImageDC = CreateCompatibleDC(m_hdcBackbuffer);

	//�����ϰ�
	HBITMAP oldBitmap = (HBITMAP)SelectObject(hImageDC,drawBitmap);

	//�̹����� �׸����ֵ����Ѵ�
	StretchBlt(m_hdcBackbuffer ,x, y, cWidth , cHight  , hImageDC ,0,0, width,hight, SRCCOPY);

	//�ٽ� dc�� �����ش�
	SelectObject(hImageDC,oldBitmap);

	//dc�� �����Ѵ�
	DeleteDC(hImageDC);

}


void main2DRander::DrawImageAlpha(HBITMAP drawBitmap, int x ,int y, int width , int hight , int cWidth , int cHight,int alpha)
{

	BLENDFUNCTION ftn;
					 ftn.AlphaFormat = 0;
					 ftn.BlendFlags = 0;
					 ftn.BlendOp = AC_SRC_OVER;
					 ftn.SourceConstantAlpha = alpha;


	//�̰��� ������ DC�� �����ϰ�
	HDC hImageDC = CreateCompatibleDC(m_hdcBackbuffer);

	//�����ϰ�
	HBITMAP oldBitmap = (HBITMAP)SelectObject(hImageDC,drawBitmap);

	//�̹����� �׸����ֵ����Ѵ�
	AlphaBlend(m_hdcBackbuffer ,x, y, cWidth , cHight  , hImageDC ,0,0, width,hight, ftn);

	//�ٽ� dc�� �����ش�
	SelectObject(hImageDC,oldBitmap);

	//dc�� �����Ѵ�
	DeleteDC(hImageDC);

}

void main2DRander::Clear(RECT* prc)
{
	//ȭ���� �����..
	if( prc == NULL )
	{	
		FillRect(m_hdcBackbuffer , &m_rectClient, NULL);
	}
	else
	{
		FillRect(m_hdcBackbuffer , prc, NULL);
	}
}

