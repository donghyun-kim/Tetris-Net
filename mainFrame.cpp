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
	m_PrintText = "텍스트 출력창";
	

	memset(m_netMap, 0 , sizeof(m_netMap) );
	 

	//키보드 버퍼를 기본값을 -1으로 설정
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

	TCHAR* szTitle = TEXT("Title");						// 제목 표시줄 텍스트입니다.
	TCHAR* szWindowClass  = TEXT("Class Name");			// 기본 창 클래스 이름입니다.

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

	// 윈도우 사이즈
	RECT rc = {0,0,550,600};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	int w = rc.right - rc.left;
	int h = rc.bottom - rc.top;
	
	// 윈도우 생성	   
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

	//렌더의 동적 할당
	m_pMain2DRander = new main2DRander();

	//할당에 실피하였다면 빠져나간다
	if(m_pMain2DRander == NULL)
	{
		return FALSE;
	}

	//렌더러의 생성
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
			//시간 차를 생성
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

	
			//이미지를 로드한다
			LoadImageFile();
			
			//최초의 틱을 생성한다
			m_tick = GetTickCount();

			//렌덤의 시드값
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
	//상태를 업데이트 한다
	
	//게임이 종료 되었는가?
	if(CheckGameOver())
	{	
		//메세지를 출력한다
		int select = MessageBox(m_hWnd,TEXT("게임 오버 입니다 \n 다시 하시겠습니까? "),TEXT("GameOver"),MB_YESNO);	
		
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

	//레벨을 조절한다
	LevelManager();

	//블록이 죽어있다면...
	if(!BlockAlive())
	{
		//블록을 생성..
		//삭제중이라면 잠시 보류..
		CreateBlock();
		
	}

	//키값을 받아서 상태 변경
	ChangeBlock(dTime);


	//삭제할 블록이 있다면 삭제한다
	static DWORD checkTime =0 ; 
	checkTime += dTime;
		
	//시간 차를 두어 삭제 하여 한번에 사라지는것을 방지
	if(checkTime > 200)
	{
		DeleteBlock();
		checkTime =0;
	}

}

void mainFrame::Draw(DWORD dTime)
{

	//더블 버퍼를 출력하기 위해 화면 DC를 가져온다
	HDC hdc = GetDC(m_hWnd);

	//버퍼에있는 내용을 삭제한다
	m_pMain2DRander->Clear(NULL);

	//이미지를 그린다
	DrawImage(dTime);
	
	//글씨를 출력한다
	DrawText(dTime);
	
	//상태를 반영한다
	m_pMain2DRander->Present(m_hWnd);
	
	//DC를 해제한다
	ReleaseDC(m_hWnd,hdc);
}

void mainFrame::LoadImageFile()
{
	///////////////////////////
	//이미지를 로딩한다

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

	//Fps를 구한다
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


	//화면 사이즈를 구한다
	RECT rc;
	GetClientRect(m_hWnd, &rc);

	//버퍼를 설정한다
	TCHAR buf1[128];
	TCHAR buf2[128];
	
	//출력할 문자열을 생성한다
	_stprintf(buf1, TEXT("Viewport Size = %d x %d FPS = %d "), rc.right, rc.bottom , fps);
	_stprintf(buf2, TEXT("Score = %d  Line = %d Level = %d Speed = %d"), m_Score , m_DeleteLine , m_Level , m_Speed);
	

	//렌더에게 문자를 출력하도록 한다
	//m_pMain2DRander->DrawText(10,500, buf1);
	m_pMain2DRander->DrawTextRander(90,520, buf2);


	m_pMain2DRander->DrawTextRander(110,540, m_PrintText);	

	


	

}

void mainFrame::DrawImage(DWORD dTime)
{

	//화면 출력 기준 좌표값
	int DrawX = 90;
	int DrawY = 0;

	int NextDrawX =0;
	int NextDrawY =0;

	int DrawX_Net = 390;
	int DrawY_Net = 40;


	//상속받은 블록의 값을가지고 화면에 출력한다
	int imageSize = 20;

	for(int i = 0 ; i < HEIGHT ; i++)
	{
		for(int j = 0 ; j< WIDTH ; j++)
		{		
			//배열의 값에따라 각 블록을 출력한다		
			m_pMain2DRander->DrawImageResize(m_hbmImageBitmap[ m_Map[i][j]] ,DrawX+(j*imageSize),DrawY+(i*imageSize),28,28,imageSize,imageSize);
		}
	}

	//현재 이동중인 블록
	if(BlockAlive())
	{
		//화면에 그릴 이미지 사이즈
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

	//보조 블록
	if(BlockAlive())
	{
		//화면에 그릴 이미지 사이즈
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

	
	//다음 블록
	if(m_pBlockBuffer != 0)
	{
		if(m_pBlockBuffer != 0)
		{
			//화면에 그릴 이미지 사이즈
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

	//네트워크 상대
	int imageSize_Net = 10;

	for(int i = 0 ; i < HEIGHT ; i++)
	{
		for(int j = 0 ; j< WIDTH ; j++)
		{		
			//배열의 값에따라 각 블록을 출력한다		
			m_pMain2DRander->DrawImageResize(m_hbmImageBitmap[ m_netMap[i][j] ]  ,DrawX_Net+(j*imageSize_Net),DrawY_Net+(i*imageSize_Net),28,28,imageSize_Net,imageSize_Net);
		}
	}
}

bool mainFrame::Fps(DWORD frame)
{

	static DWORD nowTick = 0;
	nowTick= GetTickCount();

	//1초동안 처리할수있는 프레임 양을 결정한다
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

	//키의 움직임을 반영할 시간
	if(checkTime > 30)
	{
		if(m_KeyBuffer[VK_F1] == 1)
		{
			m_AttackBlock+=1;
			m_KeyBuffer[VK_F1] = -1;			
		}


		if(m_KeyBuffer[VK_UP] == 1)
		{
			//현재 움직이고 있는 블록이 살아있는 블록ㅇ린가
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


	//helpblock 위한 바닥계산
	if( BlockAlive() )
	{
		//시작지점은 블록의 x로부터 시작
		m_pNewBlock->SetSubPosX(m_pNewBlock->GetPosX());
		
		//바닦에 닿을때까지..
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

	//내려 오는 블록의 속도를 조절한다 레벨에 따라조졀
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

//시간을 측정한다
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

	//여기에 스레드로 처리할 코드입력.....
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
		//메세지를 받기전까지 시스템을 슬립...
		
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
		
		//시작신호를 보낸다
		message.type = 'S';

		send(hSocket, (char*)&message, sizeof(NETDATA), 0); /* 메시지 전송 */
		
		//////////////////////////////////////////////////////////////

		Sleep(10);

		////////////////////////////////////////////////////////////////
		
		recv(hSocket, (char*)&message, sizeof(NETDATA), 0);
		//시작 OK신호를 받기전까지 대기..

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
					mainFrame::getMainFrame().m_PrintText ="접속대기중...";
				}
				
				if(IsDlgButtonChecked(hDlg,IDC_CLIENT)== BST_CHECKED)
				{
					SendMessage( GetDlgItem(hDlg,IDC_IPADDRESS1) ,IPM_GETADDRESS,NULL, (LPARAM) &mainFrame::getMainFrame().m_IPAddress);
					
					HANDLE thd2 = CreateThread(NULL, 0, ClientThreadProc, &mainFrame::getMainFrame(), 0, NULL);
					if(thd2==NULL) ExitProcess(0);
					mainFrame::getMainFrame().m_PrintText ="접속중...";
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