#include "stdafx.h"
#include "Resource.h"
#include "AcheronGuardian.h"
#include "ArcaBattle.h"
#include "BloodCastle.h"
#include "CastleDeep.h"
#include "CastleSiege.h"
#include "ChaosCastle.h"
#include "CommandManager.h"
#include "Crywolf.h"
#include "DevilSquare.h"
#include "GameServer.h"
#include "GameMain.h"
#include "IllusionTemple.h"
#include "InvasionManager.h"
#include "JSProtocol.h"
#include "MapManager.h"
#include "MapServerManager.h"
#include "Message.h"
#include "MiniDump.h"
#include "MossMerchant.h"
#include "Notice.h"
#include "QueueTimer.h"
#include "ServerDisplayer.h"
#include "ServerInfo.h"
#include "SocketManager.h"
#include "SocketManagerUdp.h"
#include "ThemidaSDK.h"
#include "Util.h"
#include "FakeOnline.h"

HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
HWND hWnd;
int gMapShowMonster;
int gMapShowNPC;
int gMapShowPlayer;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow) // OK
{
	VM_START

	CMiniDump::Start();

	LoadString(hInstance,IDS_APP_TITLE,szTitle,MAX_LOADSTRING);
	LoadString(hInstance,IDC_GAMESERVER,szWindowClass,MAX_LOADSTRING);

	MyRegisterClass(hInstance);

	if(InitInstance(hInstance,nCmdShow) == 0)
	{
		return 0;
	}

	SetLargeRand();

	gCloseServer = 0;

	gMapShowMonster = 0;

	gMapShowNPC = 0;

	gMapShowPlayer = 1;

	gServerInfo.ReadStartupInfo("GameServerInfo",".\\Data\\GameServerInfo - Common.dat");

	char buff[256];

	wsprintf(buff,GAMESERVER_VERSION,gServerInfo.m_ServerName,GAMESERVER_UPDATE,PROTOCOL_NAME,"STANDBY");

	SetWindowText(hWnd,buff);

	gServerDisplayer.Init(hWnd);

	WSADATA wsa;

	if(WSAStartup(MAKEWORD(2,2),&wsa) == 0)
	{
		if(gSocketManager.Start((WORD)gServerInfo.m_ServerPort) == 0)
		{
			LogAdd(LOG_RED,"Could not start GameServer");
		}
		else
		{
			GameMainInit(hWnd);

			JoinServerConnect(WM_JOIN_SERVER_MSG_PROC);

			DataServerConnect(WM_DATA_SERVER_MSG_PROC);

			gSocketManagerUdp.Connect(gServerInfo.m_ConnectServerAddress,(WORD)gServerInfo.m_ConnectServerPort);

			SetTimer(hWnd,WM_TIMER_1000,1000,0);

			SetTimer(hWnd,WM_TIMER_10000,10000,0);

			gQueueTimer.CreateTimer(QUEUE_TIMER_MONSTER,100,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_MONSTER_MOVE,100,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_MONSTER_AI,100,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_MONSTER_AI_MOVE,100,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_EVENT,100,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_VIEWPORT,1000,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_FIRST,1000,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_CLOSE,1000,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_ACCOUNT_LEVEL,60000,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_MATH_AUTHENTICATOR,10000,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_LUA_SCRIPT,1000,&QueueTimerCallback);
		}
	}
	else
	{
		LogAdd(LOG_RED,"WSAStartup() failed with error: %d",WSAGetLastError());
	}

	SetTimer(hWnd,WM_TIMER_2000,2000,0);

	HACCEL hAccelTable = LoadAccelerators(hInstance,(LPCTSTR)IDC_GAMESERVER);

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
	wcex.hIcon = LoadIcon(hInstance,(LPCTSTR)(IDI_GAMESERVER));
	wcex.hCursor = LoadCursor(0,IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = (LPCSTR)IDC_GAMESERVER;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance,(LPCTSTR)(IDI_GAMESERVER));

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
	hWndStatusBar = CreateWindowEx(0,STATUSCLASSNAME,NULL,WS_CHILD | WS_VISIBLE,0,0,0,0,hWnd,(HMENU)IDC_STATUSBAR,hInstance,NULL);
	ShowWindow(hWndStatusBar,SW_HIDE);

	int iQueueBarWidths[] = { 140,300,422,-1 };

	SendMessage(hWndStatusBar,SB_SETPARTS,4,(LPARAM)iQueueBarWidths);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	HMENU hMenu = GetMenu(hWnd);

	#if(GAMESERVER_UPDATE<=201)
	DeleteMenu(hMenu,IDM_EVENT_FORCEILLUSIONTEMPLE,MF_BYCOMMAND);
	#endif

	#if(GAMESERVER_UPDATE<=401)
	DeleteMenu(hMenu,IDM_EVENT_FORCEMOSSMERCHANT,MF_BYCOMMAND);
	#endif

	#if(GAMESERVER_UPDATE<=603)
	DeleteMenu(hMenu,IDM_EVENT_FORCEACHERONGUARDIAN,MF_BYCOMMAND);
	DeleteMenu(hMenu,IDM_EVENT_FORCEARKA0,MF_BYCOMMAND);
	DeleteMenu(hMenu,IDM_EVENT_FORCEARKA1,MF_BYCOMMAND);
	DeleteMenu(hMenu,IDM_EVENT_FORCEARKA2,MF_BYCOMMAND);
	DeleteMenu(hMenu,IDM_EVENT_FORCEARKA3,MF_BYCOMMAND);
	DeleteMenu(hMenu,IDM_EVENT_FORCEARKA4,MF_BYCOMMAND);
	DeleteMenu(hMenu,IDM_EVENT_FORCEARKA5,MF_BYCOMMAND);
	DeleteMenu(hMenu,IDM_EVENT_FORCEARKA6,MF_BYCOMMAND);
	DeleteMenu(hMenu,IDM_EVENT_FORCEARKA7,MF_BYCOMMAND);
	DeleteMenu(hMenu,IDM_EVENT_FORCEARKA8,MF_BYCOMMAND);
	DeleteMenu(hMenu,IDM_EVENT_FORCEARKA9,MF_BYCOMMAND);
	#endif

	#if(GAMESERVER_UPDATE>=803)
	DeleteMenu(hMenu,IDM_TOOL_GENERATE_ITEM,MF_BYCOMMAND);
	#endif

	#if(GAMESERVER_SHOP<2)
	DeleteMenu(hMenu,IDM_RELOAD_RELOADCASHSHOP,MF_BYCOMMAND);
	#endif
	
	return 1;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) // OK
{
	switch(message)
	{
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDM_EXIT:
					if(gObjTotalUser != 0)
					{
						MessageBox(hWnd,"Can not close gameserver with connected players,please disconnect them first!","Danger of data loss!",MB_OK | MB_ICONEXCLAMATION);
						break;
					}

					if(MessageBox(hWnd,"Are you sure to terminate GameServer?","Ask terminate server",MB_YESNO | MB_ICONQUESTION) == IDYES)
					{
						DestroyWindow(hWnd);
					}
					break;
				case IDM_FILE_ALLUSERDISCONNECT:
					gObjAllDisconnect();
					break;
				case IDM_FILE_OFFLINEUSERDISCONNECT:
					gObjOfflineDisconnect();
					break;
				case IDM_FILE_1MINUTESERVERCLOSE:
					if(gCloseMsg == 0)
					{
						gCloseMsg = 1;
						gCloseMsgTime = 60;
						gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(286),gCloseMsgTime/60);
					}
					break;
				case IDM_FILE_3MINUTESERVERCLOSE:
					if(gCloseMsg == 0)
					{
						gCloseMsg = 1;
						gCloseMsgTime = 180;
						gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(286),gCloseMsgTime/60);
					}
					break;
				case IDM_FILE_5MINUTESERVERCLOSE:
					if(gCloseMsg == 0)
					{
						gCloseMsg = 1;
						gCloseMsgTime = 300;
						gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(286),gCloseMsgTime/60);
					}
					break;


										//==================================================================================================================================================
					//FakeOnline_EMU	
#if USE_FAKE_ONLINE == TRUE

				case ID_FAKEONLINE_RELOADDATA:
					s_FakeOnline.LoadFakeData(".\\IA.xml");
					break;
				case ID_FAKEONLINE_ADDFAKEONLINE:
				{
					if (gJoinServerConnection.CheckState() != 0 && gDataServerConnection.CheckState() != 0)
					{
						s_FakeOnline.RestoreFakeOnline();
						//s_FakeOnline.AccountsRestored = 2;
					}

					/*
					if (s_FakeOnline.AccountsRestored == 0)
					{
					s_FakeOnline.AccountsRestored = 1;
					}*/
				}
				break;
				case ID_FAKEONLINE_DELFAKEONLINE:
				{
					for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
					{
						if (gObjIsConnectedGP(n) != 0 && gObj[n].IsFakeOnline)
						{
							s_FakeOnline.OnAttackAlreadyConnected(&gObj[n]);

						}
					}
				}
				break;
#endif

				//==================================================================================================================================================




					case IDM_EVENT_FORCEBLOODCASTLE:
					gBloodCastle.ForceStart();
					break;
				case IDM_EVENT_FORCECHAOSCASTLE:
					gChaosCastle.ForceStart();
					break;
				case IDM_EVENT_FORCEDEVILSQUARE:
					gDevilSquare.ForceStart();
					break;
				case IDM_EVENT_FORCEILLUSIONTEMPLE:
					gIllusionTemple.ForceStart();
					break;
				case IDM_EVENT_FORCEMOSSMERCHANT:
					gMossMerchant.ForceStart();
					break;
				case IDM_EVENT_FORCELORENDEEP:
					gCastleDeep.ForceStart();
					break;
				case IDM_EVENT_FORCEACHERONGUARDIAN:
					gAcheronGuardian.ForceStart();
					break;
				case IDM_EVENT_FORCECRYWOLF0:
					gCrywolf.SetState(CRYWOLF_STATE_NONE);
					break;
				case IDM_EVENT_FORCECRYWOLF1:
					gCrywolf.SetState(CRYWOLF_STATE_NOTIFY_1);
					break;
				case IDM_EVENT_FORCECRYWOLF2:
					gCrywolf.SetState(CRYWOLF_STATE_NOTIFY_2);
					break;
				case IDM_EVENT_FORCECRYWOLF3:
					gCrywolf.SetState(CRYWOLF_STATE_READY);
					break;
				case IDM_EVENT_FORCECRYWOLF4:
					gCrywolf.SetState(CRYWOLF_STATE_START);
					break;
				case IDM_EVENT_FORCECRYWOLF5:
					gCrywolf.SetState(CRYWOLF_STATE_END);
					break;
				case IDM_EVENT_FORCECRYWOLF6:
					gCrywolf.SetState(CRYWOLF_STATE_END_CYCLE);
					break;
				case IDM_EVENT_FORCESIEGE0:
					gCastleSiege.SetState(CASTLESIEGE_STATE_NONE,TRUE);
					break;
				case IDM_EVENT_FORCESIEGE1:
					gCastleSiege.SetState(CASTLESIEGE_STATE_REGSIEGE,TRUE);
					break;
				case IDM_EVENT_FORCESIEGE2:
					gCastleSiege.SetState(CASTLESIEGE_STATE_REGMARK,TRUE);
					break;
				case IDM_EVENT_FORCESIEGE3:
					gCastleSiege.SetState(CASTLESIEGE_STATE_NOTIFY,TRUE);
					break;
				case IDM_EVENT_FORCESIEGE4:
					gCastleSiege.SetState(CASTLESIEGE_STATE_READYSIEGE,TRUE);
					break;
				case IDM_EVENT_FORCESIEGE5:
					gCastleSiege.SetState(CASTLESIEGE_STATE_STARTSIEGE,TRUE);
					break;
				case IDM_EVENT_FORCESIEGE6:
					gCastleSiege.SetState(CASTLESIEGE_STATE_ENDSIEGE,TRUE);
					break;
				case IDM_EVENT_FORCESIEGE7:
					gCastleSiege.SetState(CASTLESIEGE_STATE_ENDCYCLE,TRUE);
					break;
				case IDM_EVENT_FORCEARKA0:
					#if(GAMESERVER_UPDATE>=702)
					gArcaBattle.SetState(AB_STATE_BLANK);
					#endif
					break;
				case IDM_EVENT_FORCEARKA1:
					#if(GAMESERVER_UPDATE>=702)
					gArcaBattle.SetState(AB_STATE_EMPTY);
					#endif
					break;
				case IDM_EVENT_FORCEARKA2:
					#if(GAMESERVER_UPDATE>=702)
					gArcaBattle.SetState(AB_STATE_SIGN1);
					#endif
					break;
				case IDM_EVENT_FORCEARKA3:
					#if(GAMESERVER_UPDATE>=702)
					gArcaBattle.SetState(AB_STATE_SIGN2);
					#endif
					break;
				case IDM_EVENT_FORCEARKA4:
					#if(GAMESERVER_UPDATE>=702)
					gArcaBattle.SetState(AB_STATE_READY);
					#endif
					break;
				case IDM_EVENT_FORCEARKA5:
					#if(GAMESERVER_UPDATE>=702)
					gArcaBattle.SetState(AB_STATE_PARTY);
					#endif
					break;
				case IDM_EVENT_FORCEARKA6:
					#if(GAMESERVER_UPDATE>=702)
					gArcaBattle.SetState(AB_STATE_START);
					#endif
					break;
				case IDM_EVENT_FORCEARKA7:
					#if(GAMESERVER_UPDATE>=702)
					gArcaBattle.SetState(AB_STATE_CLOSE);
					#endif
					break;
				case IDM_EVENT_FORCEARKA8:
					#if(GAMESERVER_UPDATE>=702)
					gArcaBattle.SetState(AB_STATE_CLEAN);
					#endif
					break;
				case IDM_EVENT_FORCEARKA9:
					#if(GAMESERVER_UPDATE>=702)
					gArcaBattle.SetState(AB_STATE_CLEAR);
					#endif
					break;
				case IDM_FORCE_INVASION0:
					gInvasionManager.ForceStart(0);
					break;
				case IDM_FORCE_INVASION1:
					gInvasionManager.ForceStart(1);
					break;
				case IDM_FORCE_INVASION2:
					gInvasionManager.ForceStart(2);
					break;
				case IDM_FORCE_INVASION3:
					gInvasionManager.ForceStart(3);
					break;
				case IDM_FORCE_INVASION4:
					gInvasionManager.ForceStart(4);
					break;
				case IDM_FORCE_INVASION5:
					gInvasionManager.ForceStart(5);
					break;
				case IDM_FORCE_INVASION6:
					gInvasionManager.ForceStart(6);
					break;
				case IDM_FORCE_INVASION7:
					gInvasionManager.ForceStart(7);
					break;
				case IDM_FORCE_INVASION8:
					gInvasionManager.ForceStart(8);
					break;
				case IDM_FORCE_INVASION9:
					gInvasionManager.ForceStart(9);
					break;
				case IDM_FORCE_INVASION10:
					gInvasionManager.ForceStart(10);
					break;
				case IDM_FORCE_INVASION11:
					gInvasionManager.ForceStart(11);
					break;
				case IDM_FORCE_INVASION12:
					gInvasionManager.ForceStart(12);
					break;
				case IDM_FORCE_INVASION13:
					gInvasionManager.ForceStart(13);
					break;
				case IDM_FORCE_INVASION14:
					gInvasionManager.ForceStart(14);
					break;
				case IDM_FORCE_INVASION15:
					gInvasionManager.ForceStart(15);
					break;
				case IDM_FORCE_INVASION16:
					gInvasionManager.ForceStart(16);
					break;
				case IDM_FORCE_INVASION17:
					gInvasionManager.ForceStart(17);
					break;
				case IDM_FORCE_INVASION18:
					gInvasionManager.ForceStart(18);
					break;
				case IDM_FORCE_INVASION19:
					gInvasionManager.ForceStart(19);
					break;
				case IDM_FORCE_INVASION20:
					gInvasionManager.ForceStart(20);
					break;
				case IDM_FORCE_INVASION21:
					gInvasionManager.ForceStart(21);
					break;
				case IDM_FORCE_INVASION22:
					gInvasionManager.ForceStart(22);
					break;
				case IDM_FORCE_INVASION23:
					gInvasionManager.ForceStart(23);
					break;
				case IDM_FORCE_INVASION24:
					gInvasionManager.ForceStart(24);
					break;
				case IDM_FORCE_INVASION25:
					gInvasionManager.ForceStart(25);
					break;
				case IDM_FORCE_INVASION26:
					gInvasionManager.ForceStart(26);
					break;
				case IDM_FORCE_INVASION27:
					gInvasionManager.ForceStart(27);
					break;
				case IDM_FORCE_INVASION28:
					gInvasionManager.ForceStart(28);
					break;
				case IDM_FORCE_INVASION29:
					gInvasionManager.ForceStart(29);
					break;
				case IDM_RELOAD_RELOADBLACKLIST:
					gServerInfo.ReadBlackListInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] BlackList reloaded successfully");
					break;
				case IDM_RELOAD_RELOADCASHSHOP:
					gServerInfo.ReadCashShopInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] CashShop reloaded successfully");
					break;
				case IDM_RELOAD_RELOADCHAOSMIX:
					gServerInfo.ReadChaosMixInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] ChaosMix reloaded successfully");
					break;
				case IDM_RELOAD_RELOADCHARACTER:
					gServerInfo.ReadCharacterInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Character reloaded successfully");
					break;
				case IDM_RELOAD_RELOADCOMMAND:
					gServerInfo.ReadCommandInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Command reloaded successfully");
					break;
				case IDM_RELOAD_RELOADCOMMON:
					gServerInfo.ReadCommonInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Common reloaded successfully");
					break;
				case IDM_RELOAD_RELOADCUSTOM:
					gServerInfo.ReadCustomInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Custom reloaded successfully");
					break;
				case IDM_RELOAD_RELOADEVENT:
					gServerInfo.ReadEventInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Event reloaded successfully");
					break;
				case IDM_RELOAD_RELOADEVENTITEMBAG:
					gServerInfo.ReadEventItemBagInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] EventItemBag reloaded successfully");
					break;
				case IDM_RELOAD_RELOADHACK:
					gServerInfo.ReadHackInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Hack reloaded successfully");
					break;
				case IDM_RELOAD_RELOADITEM:
					gServerInfo.ReadItemInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Item reloaded successfully");
					break;
				case IDM_RELOAD_RELOADMONSTER:
					gServerInfo.ReloadMonsterInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Monster reloaded successfully");
					break;
				case IDM_RELOAD_RELOADMOVE:
					gServerInfo.ReadMoveInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Move reloaded successfully");
					break;
				case IDM_RELOAD_RELOADQUEST:
					gServerInfo.ReadQuestInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Quest reloaded successfully");
					break;
				case IDM_RELOAD_RELOADSCRIPT:
					gServerInfo.ReadScriptInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Script reloaded successfully");
					break;
				case IDM_RELOAD_RELOADSHOP:
					gServerInfo.ReadShopInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Shop reloaded successfully");
					break;
				case IDM_RELOAD_RELOADSKILL:
					gServerInfo.ReadSkillInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Skill reloaded successfully");
					break;
				case IDM_RELOAD_RELOADUTIL:
					gServerInfo.ReadUtilInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Util reloaded successfully");
					break;
				case IDM_TOOL_GENERATE_ITEM:
					gServerInfo.WriteItemInfo();
					break;
				case IDM_USERS:
					DialogBox(hInst,(LPCTSTR)IDD_USERS,hWnd,(DLGPROC)Users);
					break;
				case IDM_MAP_VIEWER:
					DialogBox(hInst,(LPCTSTR)IDD_MAP_VIEWER,hWnd,(DLGPROC)MapViewer);
					break;
				case IDM_ABOUT:
					DialogBox(hInst,(LPCTSTR)IDD_ABOUTBOX,hWnd,(DLGPROC)About);
					break;
				case IDM_SSEMU:
					ShellExecute(0,0,"https://www.ssemu.com.ar",0,0,SW_SHOW);
					break;
				default:
					return DefWindowProc(hWnd,message,wParam,lParam);
			}
			break;
		case WM_TIMER:
			switch(wParam)
			{
				case WM_TIMER_1000:
					GJServerUserInfoSend();
					ConnectServerInfoSend();
					break;
				case WM_TIMER_2000:
					gObjCountProc();
					gServerDisplayer.Run();
					break;
				case WM_TIMER_10000:
					JoinServerReconnect(hWnd,WM_JOIN_SERVER_MSG_PROC);
					DataServerReconnect(hWnd,WM_DATA_SERVER_MSG_PROC);
					break;
			}
			break;
		case WM_JOIN_SERVER_MSG_PROC:
			JoinServerMsgProc(wParam,lParam);
			break;
		case WM_DATA_SERVER_MSG_PROC:
			DataServerMsgProc(wParam,lParam);
			break;
		case WM_CLOSE:
			if(gObjTotalUser != 0)
			{
				MessageBox(hWnd,"Can not close gameserver with connected players,please disconnect them first!","Danger of data loss!",MB_OK | MB_ICONEXCLAMATION);
				break;
			}

			if(MessageBox(hWnd,"Are you sure to terminate GameServer?","Ask terminate server",MB_YESNO | MB_ICONQUESTION) == IDYES)
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

int CheckIpAddress(HWND hDlg,char* text)
{
	HWND hWndIP = GetDlgItem(hDlg,IDC_USERIPLIST);

	int count = SendMessage(hWndIP,LB_GETCOUNT,0,0);

	for(int n=0;n < count;n++)
	{
		int aIndex = (int)SendMessage(hWndIP,LB_GETITEMDATA,(LPARAM)n,0);

		if(strcmp(gObj[aIndex].IpAddr,text) == 0)
		{
			return 1;
		}
	}

	return 0;
}

LRESULT CALLBACK Users(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam) // OK
{
	HWND hWndAcc = GetDlgItem(hDlg,IDC_ACCNAME);

	HWND hWndChar = GetDlgItem(hDlg,IDC_CHARNAME);

	HWND hWndIP = GetDlgItem(hDlg,IDC_USERIPLIST);

	HWND hWndHID = GetDlgItem(hDlg,IDC_USERHIDLIST);

	HWND hWndUL = GetDlgItem(hDlg,IDC_ACCOUNTLIST);

	if(message == WM_INITDIALOG)
	{
		SendMessage(hWndAcc,WM_SETTEXT,0,(LPARAM)"Account:");

		SendMessage(hWndChar,WM_SETTEXT,0,(LPARAM)"Character:");

		SendMessage(hWndIP,LB_RESETCONTENT,0,0);

		SendMessage(hWndHID,LB_RESETCONTENT,0,0);

		SendMessage(hWndUL,LB_RESETCONTENT,0,0);

		for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
		{
			if(gObj[n].Connected >= OBJECT_CONNECTED && gObj[n].Type == OBJECT_USER)
			{
				if(CheckIpAddress(hDlg,gObj[n].IpAddr) == 0)
				{
					int aItem = SendMessage(hWndIP,LB_ADDSTRING,0,(LPARAM)gObj[n].IpAddr);

					SendMessage(hWndIP,LB_SETITEMDATA,aItem,n);	
				}

				char buff[256];

				wsprintf(buff,"[%s][%s]",gObj[n].Account,gObj[n].Name);

				int bItem = SendMessage(hWndUL,LB_ADDSTRING,0,(LPARAM)buff);

				SendMessage(hWndUL,LB_SETITEMDATA,bItem,n);
			}
		}

		SetFocus(hWndIP);

		return 1;
	}
	else if(message == WM_COMMAND)
	{
		if(LOWORD(wParam) == IDC_USERIPLIST && HIWORD(wParam) == LBN_SELCHANGE)
		{
			int aItem = SendMessage(hWndIP,LB_GETCURSEL,0,0);

			int aIndex = SendMessage(hWndIP,LB_GETITEMDATA,aItem,0);

			SendMessage(hWndHID,LB_RESETCONTENT,0,0);

			if(gObj[aIndex].Connected < OBJECT_CONNECTED || gObj[aIndex].Type < OBJECT_USER)
			{
				return 0;
			}

			for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
			{
				if(gObj[n].Connected >= OBJECT_CONNECTED && gObj[n].Type == OBJECT_USER && strcmp(gObj[aIndex].IpAddr,gObj[n].IpAddr) == 0)
				{
					int bItem = SendMessage(hWndHID,LB_ADDSTRING,0,(LPARAM)gObj[n].HardwareId);

					SendMessage(hWndHID,LB_SETITEMDATA,bItem,n);
				}
			}

			return 1;
		}
		else if(LOWORD(wParam) == IDC_USERHIDLIST && HIWORD(wParam) == LBN_SELCHANGE)
		{
			int aItem = SendMessage(hWndHID,LB_GETCURSEL,0,0);

			int aIndex = SendMessage(hWndHID,LB_GETITEMDATA,aItem,0);

			char buff[256];

			wsprintf(buff,"Account: %s",gObj[aIndex].Account);

			SendMessage(hWndAcc,WM_SETTEXT,0,(LPARAM)buff);

			wsprintf(buff,"Character: %s",gObj[aIndex].Name);

			SendMessage(hWndChar,WM_SETTEXT,0,(LPARAM)buff);

			return 1;
		}
		else if(LOWORD(wParam) == IDC_REFRESH)
		{
			SendMessage(hWndAcc,WM_SETTEXT,0,(LPARAM)"Account:");

			SendMessage(hWndChar,WM_SETTEXT,0,(LPARAM)"Character:");

			SendMessage(hWndIP,LB_RESETCONTENT,0,0);

			SendMessage(hWndHID,LB_RESETCONTENT,0,0);

			SendMessage(hWndUL,LB_RESETCONTENT,0,0);

			for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
			{
				if(gObj[n].Connected >= OBJECT_CONNECTED && gObj[n].Type == OBJECT_USER)
				{
					if(CheckIpAddress(hDlg,gObj[n].IpAddr) == 0)
					{
						int aItem = SendMessage(hWndIP,LB_ADDSTRING,0,(LPARAM)gObj[n].IpAddr);

						SendMessage(hWndIP,LB_SETITEMDATA,aItem,n);	
					}

					char buff[256];

					wsprintf(buff,"[%s][%s]",gObj[n].Account,gObj[n].Name);

					int bItem = SendMessage(hWndUL,LB_ADDSTRING,0,(LPARAM)buff);

					SendMessage(hWndUL,LB_SETITEMDATA,bItem,n);
				}
			}

			SetFocus(hWndIP);

			return 1;
		}
		else if(LOWORD(wParam) == IDC_DISCONNECT)
		{
			SendMessage(hWndAcc,WM_SETTEXT,0,(LPARAM)"Account:");

			SendMessage(hWndChar,WM_SETTEXT,0,(LPARAM)"Character:");

			int aItem = SendMessage(hWndHID,LB_GETCURSEL,0,0);

			int aIndex = SendMessage(hWndHID,LB_GETITEMDATA,aItem,0);

			gObjUserKill(aIndex);

			SendMessage(hWndHID,LB_DELETESTRING,aItem,0);

			if(SendMessage(hWndHID,LB_GETCOUNT,0,0) == 0)
			{
				int bItem = SendMessage(hWndIP,LB_GETCURSEL,0,0);

				SendMessage(hWndIP,LB_DELETESTRING,bItem,0);
			}

			return 1;
		}
		else if(LOWORD(wParam) == IDC_BANACC)
		{
			int aItem = SendMessage(hWndHID,LB_GETCURSEL,0,0);

			int aIndex = SendMessage(hWndHID,LB_GETITEMDATA,aItem,0);

			SDHP_COMMAND_BAN_ACCOUNT_SEND pMsg;

			pMsg.header.set(0x0F,0x05,sizeof(pMsg));

			memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

			pMsg.BlocCode = 1;

			gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,-1);

			gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(685));

			gObjUserKill(aIndex);

			SendMessage(hWndHID,LB_DELETESTRING,aItem,0);

			if(SendMessage(hWndHID,LB_GETCOUNT,0,0) == 0)
			{
				int bItem = SendMessage(hWndIP,LB_GETCURSEL,0,0);

				SendMessage(hWndIP,LB_DELETESTRING,bItem,0);
			}

			return 1;
		}
		else if(LOWORD(wParam) == IDC_BANCHAR)
		{
			int aItem = SendMessage(hWndHID,LB_GETCURSEL,0,0);

			int aIndex = SendMessage(hWndHID,LB_GETITEMDATA,aItem,0);

			SDHP_COMMAND_BAN_CHARACTER_SEND pMsg;

			pMsg.header.set(0x0F,0x06,sizeof(pMsg));

			memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.name));

			pMsg.CtlCode = 1;

			gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,-1);

			gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(685));

			gObjUserKill(aIndex);

			SendMessage(hWndHID,LB_DELETESTRING,aItem,0);

			if(SendMessage(hWndHID,LB_GETCOUNT,0,0) == 0)
			{
				int bItem = SendMessage(hWndIP,LB_GETCURSEL,0,0);

				SendMessage(hWndIP,LB_DELETESTRING,bItem,0);
			}

			return 1;
		}
		else if(LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg,LOWORD(wParam));
			return 1;
		}
	}

	return 0;
}

LRESULT CALLBACK MapViewer(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam) // OK
{
	HWND m_hwnd = GetDlgItem(hDlg,IDC_MAP_LIST);

	if(message == WM_INITDIALOG)
	{
		SendMessage(GetDlgItem(hDlg,IDC_MONSTER_VIEW),BM_SETCHECK,gMapShowMonster,0);
		
		SendMessage(GetDlgItem(hDlg,IDC_NPC_VIEW),BM_SETCHECK,gMapShowNPC,0);
		
		SendMessage(GetDlgItem(hDlg,IDC_PLAYER_VIEW),BM_SETCHECK,gMapShowPlayer,0);

		SendMessage(m_hwnd,LB_RESETCONTENT,0,0);

		char buff[256];

		for(int i=0;i < MAX_MAP;i++)
		{
			if(gMapManager.IsValidMap(i) != 0 && gMapServerManager.CheckMapServer(i) != 0)
			{
				wsprintf(buff,"[%02d] - %s",i,gMapManager.GetMapName(i));

				int item = SendMessage(m_hwnd,LB_ADDSTRING,0,(LPARAM)buff);

				SendMessage(m_hwnd,LB_SETITEMDATA,item,i);
			}
		}

		SetFocus(m_hwnd);
			
		SetTimer(hDlg,MAP_VIEWER_TIMER,2000,0);

		return 1;
	}
	else if(message == WM_TIMER && wParam == MAP_VIEWER_TIMER)
	{
		HDC hdc = GetDC(hDlg);

		char buff[256];

		int px = 11;
		int py = 11;
		int item = SendMessage(m_hwnd,LB_GETCURSEL,0,0);
		int map = SendMessage(m_hwnd,LB_GETITEMDATA,item,0);

		HBRUSH m_brush[3] = { CreateSolidBrush(RGB(255,255,255)),CreateSolidBrush(RGB(255,0,255)),CreateSolidBrush(RGB(110,240,120)) };

		RECT rect;

		GetClientRect(hDlg,&rect);

		rect.top = 11;
		rect.left = 11;
		rect.bottom = 255*3+11;
		rect.right = 255*3+11;

		FillRect(hdc,&rect,CreateSolidBrush(RGB(40,40,140)));

		if(gMap[map].m_MapAttr != 0)
		{
			for(int x=0;x<255;x++)
			{
				for(int y=0;y<255;y++)
				{
					if(gMap[map].CheckAttr(x,y,4) != 0 || gMap[map].CheckAttr(x,y,8) != 0)
					{
						rect.top = px+x*1*3;
						rect.left = py+y*1*3;
						rect.bottom = rect.top+3;
						rect.right = rect.left+3;

						FillRect(hdc,&rect,(HBRUSH)GetStockObject(BLACK_BRUSH));
					}
				}
			}
		}

		for(int n=0;n < MAX_OBJECT;n++)
		{
			if(gObjIsConnected(n) == 0)
			{
				continue;
			}

			LPOBJ lpTarget = &gObj[n];

			if(lpTarget->Map != map || lpTarget->DieRegen != 0 || lpTarget->Teleport != TELEPORT_NONE || lpTarget->Live == 0)
			{
				continue;
			}

			rect.top = px+lpTarget->X*1*3;
			rect.left = py+lpTarget->Y*1*3;
			rect.bottom = rect.top+4;
			rect.right = rect.left+4;

			if(lpTarget->Type == OBJECT_USER)
			{
				if(gMapShowPlayer != 0)
				{
					SetTextColor(hdc,RGB(255,255,255));
					SetBkMode(hdc,TRANSPARENT);

					wsprintf(buff,"%s [%d-%d]",lpTarget->Name,lpTarget->X,lpTarget->Y);;

					TextOut(hdc,rect.left,rect.bottom,buff,strlen(buff));

					FillRect(hdc,&rect,m_brush[0]);
				}
			}
			else if(lpTarget->Type == OBJECT_NPC)
			{
				if(gMapShowNPC != 0)
				{
					FillRect(hdc,&rect,m_brush[1]);
				}
			}
			else
			{
				if(gMapShowMonster != 0)
				{
					FillRect(hdc,&rect,m_brush[2]);
				}
			}
		}

		DeleteObject((HGDIOBJ)m_brush);

		SetBkMode(hdc,TRANSPARENT);

		ReleaseDC(hDlg,hdc);

		return 1;
	}
	else if(message == WM_COMMAND)
	{
		if(LOWORD(wParam) == IDC_MONSTER_VIEW)
		{
			gMapShowMonster ^= 1;
		}
		else if(LOWORD(wParam) == IDC_NPC_VIEW)
		{
			gMapShowNPC ^= 1;
		}
		else if(LOWORD(wParam) == IDC_PLAYER_VIEW)
		{
			gMapShowPlayer ^= 1;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg,LOWORD(wParam));
			return 1;
		}
	}

	return 0;
}