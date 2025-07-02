#include "stdafx.h"
#include "resource.h"
#include "DataServer.h"
#include "AllowableIpList.h"
#include "BadSyntax.h"
#include "GuildManager.h"
#include "MiniDump.h"
#include "QueryManager.h"
#include "ServerDisplayer.h"
#include "SocketManager.h"
#include "ThemidaSDK.h"
#include "Util.h"

HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
HWND hWnd;
WORD CustomerLicenseId;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow) // OK
{
	VM_START

	CMiniDump::Start();

	LoadString(hInstance,IDS_APP_TITLE,szTitle,MAX_LOADSTRING);
	LoadString(hInstance,IDC_DATASERVER,szWindowClass,MAX_LOADSTRING);

	MyRegisterClass(hInstance);

	if(InitInstance(hInstance,nCmdShow) == 0)
	{
		return 0;
	}

	CustomerLicenseId = GetPrivateProfileInt("DataServerInfo","CustomerLicenseId",0,".\\DataServer.ini");

	char buff[256];

	wsprintf(buff,DATASERVER_VERSION,DATASERVER_UPDATE,PROTOCOL_NAME,"STANDBY");

	SetWindowText(hWnd,buff);

	gServerDisplayer.Init(hWnd);

	WSADATA wsa;

	if(WSAStartup(MAKEWORD(2,2),&wsa) == 0)
	{
		char DataServerODBC[32] = {0};

		char DataServerUSER[32] = {0};

		char DataServerPASS[32] = {0};

		GetPrivateProfileString("DataServerInfo","DataServerODBC","",DataServerODBC,sizeof(DataServerODBC),".\\DataServer.ini");

		GetPrivateProfileString("DataServerInfo","DataServerUSER","",DataServerUSER,sizeof(DataServerUSER),".\\DataServer.ini");

		GetPrivateProfileString("DataServerInfo","DataServerPASS","",DataServerPASS,sizeof(DataServerPASS),".\\DataServer.ini");

		WORD DataServerPort = GetPrivateProfileInt("DataServerInfo","DataServerPort",55960,".\\DataServer.ini");

		if(gQueryManager.Connect(DataServerODBC,DataServerUSER,DataServerPASS) == 0)
		{
			LogAdd(LOG_RED,"Could not connect to database");
		}
		else
		{
			if(gSocketManager.Start(DataServerPort) == 0)
			{
				gQueryManager.Disconnect();
			}
			else
			{
				gAllowableIpList.Load("AllowableIpList.txt");

				gBadSyntax.Load("BadSyntax.txt");

				SetTimer(hWnd,TIMER_1000,1000,0);

				gGuildManager.Init();
			}
		}
	}
	else
	{
		LogAdd(LOG_RED,"WSAStartup() failed with error: %d",WSAGetLastError());
	}

	SetTimer(hWnd,TIMER_2000,2000,0);

	HACCEL hAccelTable = LoadAccelerators(hInstance,(LPCTSTR)IDC_DATASERVER);

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
	wcex.hIcon = LoadIcon(hInstance,(LPCTSTR)IDI_DATASERVER);
	wcex.hCursor = LoadCursor(0,IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = (LPCSTR)IDC_DATASERVER;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance,(LPCTSTR)IDI_DATASERVER);

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance,int nCmdShow) // OK
{
	hInst = hInstance;

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, 0, 600, 600, 0, 0, hInstance, 0);

	if(hWnd == 0)
	{
		return 0;
	}

	HWND hWndStatusBar;
	hWndStatusBar = CreateWindowEx(0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, (HMENU)IDC_STATUSBAR, hInstance, NULL);
	ShowWindow(hWndStatusBar, SW_HIDE);

	int iQueueBarWidths[] = { 122,-1 };

	SendMessage(hWndStatusBar, SB_SETPARTS, 2, (LPARAM)iQueueBarWidths);

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
					if(CheckServersOpened() != 0)
					{
						MessageBox(hWnd, "Can not close data server with opened gameservers, close all gameservers first!", "Danger of data loss!", MB_OK | MB_ICONEXCLAMATION);
						break;
					}

					if(MessageBox(hWnd,"Are you sure to terminate DataServer?","Ask terminate server",MB_YESNO | MB_ICONQUESTION) == IDYES)
					{
						DestroyWindow(hWnd);
					}
					break;
				default:
					return DefWindowProc(hWnd,message,wParam,lParam);
			}
			break;
		case WM_TIMER:
			switch(wParam)
			{
				case TIMER_1000:
					break;
				case TIMER_2000:
					gServerDisplayer.Run();
					break;
				default:
					break;
			}
			break;
		case WM_CLOSE:
			if(CheckServersOpened() != 0)
			{
				MessageBox(hWnd, "Can not close DataServer with opened GameServer, close all GameServers first!", "Danger of data loss!", MB_OK | MB_ICONEXCLAMATION);
				break;
			}

			if (MessageBox(hWnd, "Are you sure to terminate DataServer?", "Ask terminate server", MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				DestroyWindow(hWnd);
			}
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
		case WM_DESTROY:
			PostQuitMessage(0);
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
