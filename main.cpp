
#include "windows.h"
#include "tchar.h"
#include "mainFrame.h"



int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	mainFrame& frame = mainFrame::getMainFrame();

	DialogBox(hInstance, MAKEINTRESOURCE(ID_SEL),  NULL, Connect   );

	if(frame.Create(hInstance))
	{
		frame.Run();
	}
	
		
	return 0;
}





