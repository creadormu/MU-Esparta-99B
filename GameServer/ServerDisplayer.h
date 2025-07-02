// ServerDisplayer.h: interface for the CServerDisplayer class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_LOG_TEXT_LINE 29
#define MAX_LOG_TEXT_SIZE 150

enum eLogColor
{
	LOG_BLACK = 0,
	LOG_RED = 1,
	LOG_GREEN = 2,
	LOG_BLUE = 3,
};

struct LOG_DISPLAY_INFO
{
	char text[MAX_LOG_TEXT_SIZE];
	eLogColor color;
};

class CServerDisplayer
{
public:
	CServerDisplayer();
	virtual ~CServerDisplayer();
	void Init(HWND hWnd);
	void Run();
	void SetWindowName();
	void LogTextPaint();
	void PaintBarInfo();
	void LogAddText(eLogColor color,char* text,int size);
private:
	HWND m_hwnd;
	LOG_DISPLAY_INFO m_log[MAX_LOG_TEXT_LINE];
	int m_count;
};

extern CServerDisplayer gServerDisplayer;