#include "stdafx.h"
#include "resource.h"
#include "BlackList.h"
#include "ConnectServer.h"
#include "MiniDump.h"
#include "ServerDisplayer.h"
#include "ServerList.h"
#include "SocketManager.h"
#include "SocketManagerUdp.h"
#include "ThemidaSDK.h"
#include "Util.h"

HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
HWND hWnd;
WORD CustomerLicenseId;
int MaxConnectionPerIP;
int MaxPacketPerSecond;
int MaxConnectionIdle;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow) // OK
{
	VM_START

	CMiniDump::Start();

	LoadString(hInstance,IDS_APP_TITLE,szTitle,MAX_LOADSTRING);
	LoadString(hInstance,IDC_CONNECTSERVER,szWindowClass,MAX_LOADSTRING);

	MyRegisterClass(hInstance);

	if(InitInstance(hInstance,nCmdShow) == 0)
	{
		return 0;
	}

	CustomerLicenseId = GetPrivateProfileInt("ConnectServerInfo","CustomerLicenseId",0,".\\ConnectServer.ini");

	char buff[256];

	wsprintf(buff,CONNECTSERVER_VERSION,CONNECTSERVER_UPDATE,PROTOCOL_NAME,"STANDBY");

	SetWindowText(hWnd,buff);

	gServerDisplayer.Init(hWnd);

	WSADATA wsa;

	if(WSAStartup(MAKEWORD(2,2),&wsa) == 0)
	{
		WORD ConnectServerPortTCP = GetPrivateProfileInt("ConnectServerInfo","ConnectServerPortTCP",44405,".\\ConnectServer.ini");
		WORD ConnectServerPortUDP = GetPrivateProfileInt("ConnectServerInfo","ConnectServerPortUDP",55557,".\\ConnectServer.ini");

		MaxConnectionPerIP = GetPrivateProfileInt("ConnectServerInfo","MaxConnectionPerIP",0,".\\ConnectServer.ini");
		MaxPacketPerSecond = GetPrivateProfileInt("ConnectServerInfo","MaxPacketPerSecond",0,".\\ConnectServer.ini");
		MaxConnectionIdle = GetPrivateProfileInt("ConnectServerInfo","MaxConnectionIdle",0,".\\ConnectServer.ini");

		if(gSocketManager.Start(ConnectServerPortTCP) != 0)
		{
			if(gSocketManagerUdp.Start(ConnectServerPortUDP) != 0)
			{
				gBlackList.Load("BlackList.txt");
				gServerList.Load("ServerList.dat");

				SetTimer(hWnd,TIMER_1000,1000,0);
			}
		}
	}
	else
	{
		LogAdd(LOG_RED,"WSAStartup() failed with error: %d",WSAGetLastError());
	}

	SetTimer(hWnd,TIMER_2000,2000,0);

	HACCEL hAccelTable = LoadAccelerators(hInstance,(LPCTSTR)IDC_CONNECTSERVER);

	MSG msg;

	while(GetMessage(&msg,0,0,0) != 0)
	{
		if(TranslateAccelerator(msg.hwnd,hAccelTable,&msg) == 0)
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}

	CMiniDump::Clean();

	VM_END

	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) // OK
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance,(LPCTSTR)IDI_CONNECTSERVER);
	wcex.hCursor = LoadCursor(0,IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = (LPCSTR)IDC_CONNECTSERVER;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance,(LPCTSTR)IDI_CONNECTSERVER);

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance,int nCmdShow) // OK
{
	hInst = hInstance;

	hWnd = CreateWindow(szWindowClass,szTitle,WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,CW_USEDEFAULT,0,600,600,0,0,hInstance,0);

	if(hWnd == 0)
	{
		return 0;
	}

	HWND hWndStatusBar;
	hWndStatusBar = CreateWindowEx(0,STATUSCLASSNAME,0,WS_CHILD | WS_VISIBLE,0,0,0,0,hWnd,(HMENU)IDC_STATUSBAR,hInstance,0);
	ShowWindow(hWndStatusBar,SW_HIDE);

	int iQueueBarWidths[] = { 110,230,355,-1 };

	SendMessage(hWndStatusBar,SB_SETPARTS,4,(LPARAM)iQueueBarWidths);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);
	return 1;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) // OK
{
	switch(message)
	{
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDM_ABOUT:
					DialogBox(hInst,(LPCTSTR)IDD_ABOUTBOX,hWnd,(DLGPROC)About);
					break;
				case IDM_EXIT:
					if(MessageBox(hWnd,"Are you sure to terminate ConnectServer?","Ask terminate server",MB_YESNO | MB_ICONQUESTION) == IDYES)
					{
						DestroyWindow(hWnd);
					}
					break;
				case IDM_RELOAD_RELOADBLACKLIST:
					gBlackList.Load("BlackList.txt");
					LogAdd(LOG_BLUE,"BlackList reloaded successfully");
					break;
				case IDM_RELOAD_RELOADCONFIG:
					MaxConnectionPerIP = GetPrivateProfileInt("ConnectServerInfo","MaxConnectionPerIP",0,".\\ConnectServer.ini");
					MaxPacketPerSecond = GetPrivateProfileInt("ConnectServerInfo","MaxPacketPerSecond",0,".\\ConnectServer.ini");
					MaxConnectionIdle = GetPrivateProfileInt("ConnectServerInfo","MaxConnectionIdle",0,".\\ConnectServer.ini");
					LogAdd(LOG_BLUE,"Config reloaded successfully");
					break;
				case IDM_RELOAD_RELOADSERVERLIST:
					gServerList.Load("ServerList.dat");
					LogAdd(LOG_BLUE, "ServerList reloaded successfully");
					break;
				default:
					return DefWindowProc(hWnd,message,wParam,lParam);
			}
			break;
		case WM_TIMER:
			switch(wParam)
			{
				case TIMER_1000:
					gServerList.MainProc();
					ConnectServerTimeoutProc();
					break;
				case TIMER_2000:
					gServerDisplayer.Run();
					break;
				default:
					break;
			}
			break;
		case WM_CLOSE:
			if (MessageBox(hWnd,"Are you sure to terminate ConnectServer?","Ask terminate server",MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				DestroyWindow(hWnd);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_PAINT:
		{
			PAINTSTRUCT hPaintStruct;
			HDC hdc = BeginPaint(hWnd,&hPaintStruct);
			HDC hMemDC = CreateCompatibleDC(hdc);
			HBITMAP OldBmp = (HBITMAP)SelectObject(hMemDC,LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP1)));
			BitBlt(hdc,0,0,594,80,hMemDC,0,0,SRCCOPY);
			SelectObject(hMemDC,OldBmp);
			DeleteDC(hMemDC);
			EndPaint(hWnd,&hPaintStruct);
		}
		break;
		default:
			return DefWindowProc(hWnd,message,wParam,lParam);
	}

	return 0;
}

LRESULT CALLBACK About(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam) // OK
{
	switch(message)
	{
		case WM_INITDIALOG:
			return 1;
		case WM_COMMAND:
			if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg,LOWORD(wParam));
				return 1;
			}
			break;
	}

	return 0;
}
