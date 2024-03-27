#include "mainFrame.h"
#include "main2DRander.h"

#include "./block/system.h"
#include "stdio.h"
NETDATA::NETDATA()
{
	//char type;
	//char message[10];
	//int  Map[HEIGHT][WIDTH];
	AttackLine = 0;
	
}

mainFrame::mainFrame()
{
	m_Sleep = FALSE;
	m_PrintText = "�ؽ�Ʈ ���â";
	

	memset(m_netMap, 0 , sizeof(m_netMap) );
	 

	//Ű���� ���۸� �⺻���� -1���� ����
	memset(m_KeyBuffer,-1,sizeof(m_KeyBuffer));
}

mainFrame::~mainFrame()
{
}

mainFrame& mainFrame::getMainFrame()
{
	static mainFrame frame;
	return frame;
}

BOOL mainFrame::Create(HINSTANCE hInst)
{
	m_hInst = hInst;

	TCHAR* szTitle = TEXT("Title");						// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
	TCHAR* szWindowClass  = TEXT("Class Name");			// �⺻ â Ŭ���� �̸��Դϴ�.

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInst;
	wcex.hIcon			= 0; //LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RANDER2D));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0; //MAKEINTRESOURCE(IDC_RANDER2D);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= 0; //LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassEx(&wcex);

	// ������ ������
	RECT rc = {0,0,550,600};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	int w = rc.right - rc.left;
	int h = rc.bottom - rc.top;
	
	// ������ ����	   
	m_hWnd = CreateWindow(szWindowClass, szTitle, 
							WS_OVERLAPPEDWINDOW,
							(GetSystemMetrics(SM_CXSCREEN) - w) / 2,
							(GetSystemMetrics(SM_CYSCREEN) - h) / 2,
							w, h,
							NULL, NULL, hInst, NULL);

	if (m_hWnd == NULL)
	{
		return FALSE;
	}

	ShowWindow(m_hWnd,SW_SHOW );
	UpdateWindow(m_hWnd);

	//������ ���� �Ҵ�
	m_pMain2DRander = new main2DRander();

	//�Ҵ翡 �����Ͽ��ٸ� ����������
	if(m_pMain2DRander == NULL)
	{
		return FALSE;
	}

	//�������� ����
	m_pMain2DRander->Create(m_hWnd);
	
	return TRUE;
}


void mainFrame::Run()
{

	MSG msg;
	m_runFlag = true;
	while (m_runFlag)
	{
		
		if( PeekMessage(&msg, m_hWnd, 0,0, PM_NOREMOVE) )
		{
			GetMessage(&msg, NULL, 0, 0);
			
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{	
			//�ð� ���� ����
			DWORD dTime = differentTime();
			if(m_Sleep)
			{
				Update(dTime);
			}
			Draw(dTime);
		}
	}

}


////////////////////////////////////////////////////////////////////////////////////////



LRESULT CALLBACK mainFrame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return mainFrame::getMainFrame().MainProc(hWnd, message, wParam,  lParam);

}


LRESULT CALLBACK mainFrame::MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		{
			m_KeyBuffer[wParam] = 1;
		}
		break;

	case WM_KEYUP:
		{
			//m_KeyBuffer[wParam] = 0;
		}
		break;

	case WM_CREATE:
		{

	
			//�̹����� �ε��Ѵ�
			LoadImageFile();
			
			//������ ƽ�� �����Ѵ�
			m_tick = GetTickCount();

			//������ �õ尪
			srand(m_tick);
			

		}
		break;

	case WM_COMMAND:
		break;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc;
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		break;

	case WM_DESTROY:
		
		m_pMain2DRander->Destroy();
		PostQuitMessage(0);
		m_runFlag=false;
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void mainFrame::Update(DWORD dTime)
{
	//���¸� ������Ʈ �Ѵ�
	
	//������ ���� �Ǿ��°�?
	if(CheckGameOver())
	{	
		//�޼����� ����Ѵ�
		int select = MessageBox(m_hWnd,TEXT("���� ���� �Դϴ� \n �ٽ� �Ͻðڽ��ϱ�? "),TEXT("GameOver"),MB_YESNO);	
		
		switch(select)
		{
		case IDYES:
			
			//retry game
			Init();
			
			break;
		case IDNO:

			//end game
			exit(0);

			break;
		
		}
	}

	//������ �����Ѵ�
	LevelManager();

	//����� �׾��ִٸ�...
	if(!BlockAlive())
	{
		//����� ����..
		//�������̶�� ��� ����..
		CreateBlock();
		
	}

	//Ű���� �޾Ƽ� ���� ����
	ChangeBlock(dTime);


	//������ ����� �ִٸ� �����Ѵ�
	static DWORD checkTime =0 ; 
	checkTime += dTime;
		
	//�ð� ���� �ξ� ���� �Ͽ� �ѹ��� ������°��� ����
	if(checkTime > 200)
	{
		DeleteBlock();
		checkTime =0;
	}

}

void mainFrame::Draw(DWORD dTime)
{

	//���� ���۸� ����ϱ� ���� ȭ�� DC�� �����´�
	HDC hdc = GetDC(m_hWnd);

	//���ۿ��ִ� ������ �����Ѵ�
	m_pMain2DRander->Clear(NULL);

	//�̹����� �׸���
	DrawImage(dTime);
	
	//�۾��� ����Ѵ�
	DrawText(dTime);
	
	//���¸� �ݿ��Ѵ�
	m_pMain2DRander->Present(m_hWnd);
	
	//DC�� �����Ѵ�
	ReleaseDC(m_hWnd,hdc);
}

void mainFrame::LoadImageFile()
{
	///////////////////////////
	//�̹����� �ε��Ѵ�

	m_hbmImageBitmap[0] = (HBITMAP)LoadImage(NULL ,TEXT("./image/BlackBlock.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	
	m_hbmImageBitmap[1] = (HBITMAP)LoadImage(NULL ,TEXT("./image/1Block.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_hbmImageBitmap[2] = (HBITMAP)LoadImage(NULL ,TEXT("./image/2Block.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_hbmImageBitmap[3] = (HBITMAP)LoadImage(NULL ,TEXT("./image/3Block.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_hbmImageBitmap[4] = (HBITMAP)LoadImage(NULL ,TEXT("./image/4Block.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_hbmImageBitmap[5] = (HBITMAP)LoadImage(NULL ,TEXT("./image/5Block.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_hbmImageBitmap[6] = (HBITMAP)LoadImage(NULL ,TEXT("./image/6Block.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_hbmImageBitmap[7] = (HBITMAP)LoadImage(NULL ,TEXT("./image/7Block.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

	m_hbmImageBitmap[9] = (HBITMAP)LoadImage(NULL ,TEXT("./image/9Block.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

}
void mainFrame::DrawText(DWORD dTime)
{

	//Fps�� ���Ѵ�
	static DWORD checkTime =0 ; 
	static DWORD fpsCount =0;
	static DWORD fps =0;
	checkTime += dTime;
	fpsCount++;
	if(checkTime > 1000)
	{
		fps = fpsCount;
		checkTime =0;
		fpsCount = 0;
	}


	//ȭ�� ����� ���Ѵ�
	RECT rc;
	GetClientRect(m_hWnd, &rc);

	//���۸� �����Ѵ�
	TCHAR buf1[128];
	TCHAR buf2[128];
	
	//����� ���ڿ��� �����Ѵ�
	_stprintf(buf1, TEXT("Viewport Size = %d x %d FPS = %d "), rc.right, rc.bottom , fps);
	_stprintf(buf2, TEXT("Score = %d  Line = %d Level = %d Speed = %d"), m_Score , m_DeleteLine , m_Level , m_Speed);
	

	//�������� ���ڸ� ����ϵ��� �Ѵ�
	//m_pMain2DRander->DrawText(10,500, buf1);
	m_pMain2DRander->DrawTextRander(90,520, buf2);


	m_pMain2DRander->DrawTextRander(110,540, m_PrintText);	

	


	

}

void mainFrame::DrawImage(DWORD dTime)
{

	//ȭ�� ��� ���� ��ǥ��
	int DrawX = 90;
	int DrawY = 0;

	int NextDrawX =0;
	int NextDrawY =0;

	int DrawX_Net = 390;
	int DrawY_Net = 40;


	//��ӹ��� ����� ���������� ȭ�鿡 ����Ѵ�
	int imageSize = 20;

	for(int i = 0 ; i < HEIGHT ; i++)
	{
		for(int j = 0 ; j< WIDTH ; j++)
		{		
			//�迭�� �������� �� ����� ����Ѵ�		
			m_pMain2DRander->DrawImageResize(m_hbmImageBitmap[ m_Map[i][j]] ,DrawX+(j*imageSize),DrawY+(i*imageSize),28,28,imageSize,imageSize);
		}
	}

	//���� �̵����� ���
	if(BlockAlive())
	{
		//ȭ�鿡 �׸� �̹��� ������
		int imageSize = 20;
		for(int i = 0 ; i < 4 ; i++)
		{
			for(int j = 0 ; j < 4 ;j++)
			{
				if(m_pNewBlock->GetBlockValue(i,j) !=0)
				{
					m_pMain2DRander->DrawImageResize( m_hbmImageBitmap[m_pNewBlock->GetBlockValue(i,j)], DrawX+(m_pNewBlock->GetPosY() +j)*imageSize,DrawY+(m_pNewBlock->GetPosX() +i)*imageSize,28,28,imageSize,imageSize);			
				}
			}
		}
	}

	//���� ���
	if(BlockAlive())
	{
		//ȭ�鿡 �׸� �̹��� ������
		int imageSize = 20;
		for(int i = 0 ; i < 4 ; i++)
		{
			for(int j = 0 ; j < 4 ;j++)
			{
				if(m_pNewBlock->GetBlockValue(i,j) !=0)
				{
					m_pMain2DRander->DrawImageAlpha(m_hbmImageBitmap[m_pNewBlock->GetBlockValue(i,j) ], DrawX+(m_pNewBlock->GetPosY() +j)*imageSize,(m_pNewBlock->GetSubPosX() +i)*imageSize,28,28,imageSize,imageSize,100);			
				}
			}
		}
	}

	
	//���� ���
	if(m_pBlockBuffer != 0)
	{
		if(m_pBlockBuffer != 0)
		{
			//ȭ�鿡 �׸� �̹��� ������
			int imageSize = 20;
			for(int i = 0 ; i < 4 ; i++)
			{
				for(int j = 0 ; j < 4 ;j++)
				{
					m_pMain2DRander->DrawImageResize(m_hbmImageBitmap[m_pBlockBuffer->GetBlockValue(i,j)], NextDrawX+(j*imageSize),NextDrawY+(i*imageSize),28,28,imageSize,imageSize);			
				}
			}
		}
	}

	//��Ʈ��ũ ���
	int imageSize_Net = 10;

	for(int i = 0 ; i < HEIGHT ; i++)
	{
		for(int j = 0 ; j< WIDTH ; j++)
		{		
			//�迭�� �������� �� ����� ����Ѵ�		
			m_pMain2DRander->DrawImageResize(m_hbmImageBitmap[ m_netMap[i][j] ]  ,DrawX_Net+(j*imageSize_Net),DrawY_Net+(i*imageSize_Net),28,28,imageSize_Net,imageSize_Net);
		}
	}
}

bool mainFrame::Fps(DWORD frame)
{

	static DWORD nowTick = 0;
	nowTick= GetTickCount();

	//1�ʵ��� ó���Ҽ��ִ� ������ ���� �����Ѵ�
	if(nowTick - m_tick  > 1000/frame)
	{
		m_tick = nowTick;
		return true;
	}else{
		return false;
	}
}

void mainFrame::ChangeBlock(DWORD dTime)
{

	static DWORD checkTime =0 ; 
	checkTime += dTime;

	//Ű�� �������� �ݿ��� �ð�
	if(checkTime > 30)
	{
		if(m_KeyBuffer[VK_F1] == 1)
		{
			m_AttackBlock+=1;
			m_KeyBuffer[VK_F1] = -1;			
		}


		if(m_KeyBuffer[VK_UP] == 1)
		{
			//���� �����̰� �ִ� ����� ����ִ� ��Ϥ�����
			if( BlockAlive())
			{
				m_pNewBlock->RotateRight();
				if(colBlock())
				{
					m_pNewBlock->RotateLeft();
				}
			}
			m_KeyBuffer[VK_UP] = -1;
		}

		if(m_KeyBuffer[VK_RIGHT] == 1)
		{
			if(BlockAlive())
			{
				m_pNewBlock->MoveRight();
				if(colBlock())
				{
					m_pNewBlock->MoveLeft();
				}
			}
			m_KeyBuffer[VK_RIGHT] = -1;
		}

		if(m_KeyBuffer[VK_LEFT]== 1)
		{
			if(BlockAlive())
			{
				m_pNewBlock->MoveLeft();
				if(colBlock())
				{
					m_pNewBlock->MoveRight();
				}
			}
			m_KeyBuffer[VK_LEFT] = -1;
		}

		if(m_KeyBuffer[VK_DOWN] == 1)
		{
			if(BlockAlive())
			{	
				m_pNewBlock->MoveDown();
				if(colBlock())
				{
					m_pNewBlock->MoveUp();
					NotMoveBlock();
				}
			}
			m_KeyBuffer[VK_DOWN] = -1;
		}

		
		if(m_KeyBuffer[VK_SPACE] ==1)
		{

			if( BlockAlive() )
			{
				while(1)
				{
					m_pNewBlock->MoveDown();
					if(colBlock())
					{
						m_pNewBlock->MoveUp();
						NotMoveBlock();
						break;
					}
				}
			}
			m_KeyBuffer[VK_SPACE] = -1;
		}

		checkTime =0;
	}


	//helpblock ���� �ٴڰ��
	if( BlockAlive() )
	{
		//���������� ����� x�κ��� ����
		m_pNewBlock->SetSubPosX(m_pNewBlock->GetPosX());
		
		//�ٴۿ� ����������..
		while(1)
		{
			m_pNewBlock->MoveSubDown();		
			if(colSubBlock())
			{
				m_pNewBlock->MoveSubUp();
				break;
			}		
		}
	}
	

	static DWORD blockDownTick = 0;
	blockDownTick += dTime;

	//���� ���� ����� �ӵ��� �����Ѵ� ������ ��������
	if( blockDownTick > m_Speed)
	{
		if( BlockAlive() )
		{
			m_pNewBlock->MoveDown();
			if(colBlock())
			{
				m_pNewBlock->MoveUp();
				NotMoveBlock();
			}
		}
		blockDownTick = 0;
	}
	
}

//�ð��� �����Ѵ�
DWORD mainFrame::differentTime()
{
	DWORD NowTime = GetTickCount();
	DWORD returnTime = NowTime - m_tick;
	m_tick = NowTime;
	return returnTime;
}


DWORD WINAPI ServerThreadProc(LPVOID lpParam)
{
	
	mainFrame* frame = (mainFrame*)lpParam;

	WSADATA wsaData;
	SOCKET hServSock;
	SOCKET hClntSock;

	NETDATA message ;

	int strLen;

	SOCKADDR_IN servAddr;
	SOCKADDR_IN clntAddr;
	int clntAddrSize;

	//���⿡ ������� ó���� �ڵ��Է�.....
	char* port = "777";



	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		frame-> m_PrintText = "WSAStartup() error!" ; 
	}

		hServSock=socket(PF_INET, SOCK_STREAM, 0);   
	if(hServSock == INVALID_SOCKET)
	{
		frame-> m_PrintText = "socket() error";
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family=AF_INET;
	servAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servAddr.sin_port=htons(atoi(port));

	if(bind(hServSock, (SOCKADDR*) &servAddr, sizeof(servAddr))==SOCKET_ERROR)
	{
		frame-> m_PrintText = "bind() error";
	}

	if(listen(hServSock, 5)==SOCKET_ERROR)
	{
		frame-> m_PrintText = "listen() error";
	}

	clntAddrSize=sizeof(clntAddr);    
	hClntSock=accept(hServSock, (SOCKADDR*)&clntAddr,&clntAddrSize);
	if(hClntSock==INVALID_SOCKET)
	{
		frame-> m_PrintText = "accept() error";
	}

	

	while(1)
	{ 
		recv(hClntSock, (char*)&message, sizeof(NETDATA), 0);
		//�޼����� �ޱ������� �ý����� ����...
		
		if(message.type == 'S')
		{
			frame->m_PrintText = "OK";
			frame->m_Sleep = TRUE;
		}

		memcpy( frame-> m_netMap , message.Map , sizeof(message.Map));
		if(message.AttackLine !=0)
		{
			frame->m_AttackBlock += message.AttackLine;
			message.AttackLine =0;
		}

		///////////////////////////////////////////////////////////

		Sleep(10);		

		////////////////////////////////////////////////////////////


		message.type= 'O';
		if(frame->m_AttackLine != 0)
		{
			message.AttackLine =+ frame->m_AttackLine;
			frame->m_AttackLine =0;
		}
		memcpy( message.Map , frame->m_Map, sizeof(message.Map)); 
		send(hClntSock, (char*)&message, sizeof(NETDATA), 0);
		
		
	}

	closesocket(hClntSock);
	WSACleanup();

	return 0;

}



DWORD WINAPI ClientThreadProc(LPVOID lpParam)
{
	mainFrame* frame = (mainFrame*)lpParam;

	char* ip ;

	DWORD IPAddress =  mainFrame::getMainFrame().m_IPAddress;

	if(IPAddress == 0)
	{
		ip = "127.0.0.1";
	}
	else
	{
		char Result[16];
		sprintf(Result,"%d.%d.%d.%d",					
		FIRST_IPADDRESS(IPAddress),
		SECOND_IPADDRESS(IPAddress),
		THIRD_IPADDRESS(IPAddress),
		FOURTH_IPADDRESS(IPAddress));

		ip = Result;
	}

	char* port = "777";

	NETDATA message;

	WSADATA wsaData;
	SOCKET hSocket;
	//char message[BUFSIZE];
	int strLen;

	SOCKADDR_IN servAddr;



	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		frame->m_PrintText = "WSAStartup() error!"; 
	}

	hSocket=socket(PF_INET, SOCK_STREAM, 0);   
	
	if(hSocket == INVALID_SOCKET)
	{
		frame->m_PrintText = "socket() error";
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family=AF_INET;
	servAddr.sin_addr.s_addr=inet_addr(ip);
	servAddr.sin_port=htons(atoi(port));

	if(connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr))==SOCKET_ERROR)
	{
		frame->m_PrintText = "connect() error!";
	}


	strcpy(message.message,"ABCDEFGHIJKLMN");

	

	while(1)
	{
		if(frame->m_AttackLine != 0)
		{
			message.AttackLine =+ frame->m_AttackLine;
			frame->m_AttackLine =0;
		}
		memcpy( message.Map ,frame->m_Map , sizeof(frame->m_Map));
		
		//���۽�ȣ�� ������
		message.type = 'S';

		send(hSocket, (char*)&message, sizeof(NETDATA), 0); /* �޽��� ���� */
		
		//////////////////////////////////////////////////////////////

		Sleep(10);

		////////////////////////////////////////////////////////////////
		
		recv(hSocket, (char*)&message, sizeof(NETDATA), 0);
		//���� OK��ȣ�� �ޱ������� ���..

		if(message.type == 'O')
		{
			frame->m_PrintText = "OK";
			frame->m_Sleep = TRUE;
		}

		memcpy( frame->m_netMap , message.Map , sizeof(frame->m_Map)); 
		if(message.AttackLine !=0)
		{
			frame->m_AttackBlock += message.AttackLine;
			message.AttackLine =0;
		}
	
	}
 
	closesocket(hSocket);
	WSACleanup();
	return 0;

}





INT_PTR CALLBACK Connect(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		//MessageBox(hDlg,TEXT("a"),TEXT("b"),NULL);
		
		CheckRadioButton(hDlg , IDC_SINGLE , IDC_CLIENT , IDC_SINGLE );

		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam) )
		{
			/*
			case IDC_CLIENT:
				
				break;
			case IDC_HOST:
			case IDC_SINGLE:
				break;
			*/

			case IDOK:
				if(IsDlgButtonChecked(hDlg,IDC_HOST)== BST_CHECKED)
				{

					HANDLE thd = CreateThread(NULL, 0, ServerThreadProc, &mainFrame::getMainFrame()  , 0, NULL);
					if(thd==NULL) ExitProcess(0);
					mainFrame::getMainFrame().m_PrintText ="���Ӵ����...";
				}
				
				if(IsDlgButtonChecked(hDlg,IDC_CLIENT)== BST_CHECKED)
				{
					SendMessage( GetDlgItem(hDlg,IDC_IPADDRESS1) ,IPM_GETADDRESS,NULL, (LPARAM) &mainFrame::getMainFrame().m_IPAddress);
					
					HANDLE thd2 = CreateThread(NULL, 0, ClientThreadProc, &mainFrame::getMainFrame(), 0, NULL);
					if(thd2==NULL) ExitProcess(0);
					mainFrame::getMainFrame().m_PrintText ="������...";
				}

				if(IsDlgButtonChecked(hDlg,IDC_SINGLE)== BST_CHECKED)
				{
					mainFrame::getMainFrame().m_Sleep = TRUE;
				}
				
				EndDialog(hDlg, LOWORD(wParam));	
			
			break;

		}
	}
	return (INT_PTR)FALSE;
}