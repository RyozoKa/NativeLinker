/*	Windows only code

*/


#ifdef WIN32
#include "Core.h"
#include "HypWindowsConsole.h"


HINSTANCE	HypWinConsole::hInstance	= NULL;
HWND		HypWinConsole::messagebox	= NULL;
HWND		HypWinConsole::commandline	= NULL;
HWND		HypWinConsole::window		= NULL;
HBRUSH		HypWinConsole::BRUSH		= NULL;
HFONT		HypWinConsole::ARIAL		= NULL;
COLORREF	HypWinConsole::BLACK		= (((BYTE)(0)|((WORD)((BYTE)(0))<<8))|(((DWORD)(BYTE)(0))<<16));
COLORREF	HypWinConsole::GREEN		= (DWORD)((WORD)(255 << 8));
WNDCLASSEX	HypWinConsole::wcx;
WNDPROC		HypWinConsole::lpfnEditWndProc	= NULL;
HypConsole::callback	HypConsole::SendCommand	= NULL;

DWORD WINAPI _init(LPVOID c);


void HypWinConsole::Initialize()
{
	if(!hThread)
		hThread = CreateThread(NULL, 0, _init, NULL, 0, 0);
	GLog->SetStrmPtr(WriteLine);
}

DWORD WINAPI _init(LPVOID c)
{
	//Initialize variables
	HypWinConsole::ARIAL = CreateFont(-12,
		0,
		0,
		0,
		FW_NORMAL,
		0,
		0,
		0,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		FF_DONTCARE,
		L"Arial");
 
		if(!HypWinConsole::hInstance)
			HypWinConsole::hInstance = GetModuleHandle(NULL);
		HypWinConsole::wcx.cbSize = sizeof(HypWinConsole::wcx);          // size of structure 
		HypWinConsole::wcx.style = CS_HREDRAW | CS_VREDRAW;                    // redraw if size changes 
		HypWinConsole::wcx.lpfnWndProc = HypWinConsole::MainWndProc;     // points to window procedure - this must be declared outside of the class!
		HypWinConsole::wcx.cbClsExtra = 0;                // no extra class memory 
		HypWinConsole::wcx.cbWndExtra = 0;                // no extra window memory 
		HypWinConsole::wcx.hInstance = HypWinConsole::hInstance;         // handle to instance 
		HypWinConsole::wcx.hIcon = LoadIcon(NULL,IDI_APPLICATION);              // predefined app. icon 
		HypWinConsole::wcx.hCursor = LoadCursor(NULL, IDC_ARROW);                    // predefined arrow 
		HypWinConsole::wcx.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
		HypWinConsole::wcx.lpszMenuName =  L"MainMenu";    // name of menu resource 
		HypWinConsole::wcx.lpszClassName = L"MainWClass";  // name of window class 
		HypWinConsole::wcx.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

		RegisterClassEx(&HypWinConsole::wcx);

		HypWinConsole::BRUSH = CreateSolidBrush(HypWinConsole::BLACK);

	HypWinConsole::window = CreateWindow( 
        L"MainWClass",        // name of window class 
        L"Terminal",            // title-bar string 
        WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME, // top-level window 
        CW_USEDEFAULT,       // default horizontal position 
        CW_USEDEFAULT,       // default vertical position 
        640,
        340,       
        (HWND) NULL,         // no owner window 
        (HMENU) NULL,        // use class menu 
        HypWinConsole::hInstance,           // handle to application instance 
        (LPVOID) NULL);      // no window-creation data 
	if(!HypWinConsole::window)
		ErrorExit(L"");
	ShowWindow(HypWinConsole::window, 5);
    UpdateWindow(HypWinConsole::window);
	BOOL fGotMessage;
	 MSG msg;
    while ((fGotMessage = GetMessage(&msg, (HWND) NULL, 0, 0)) != 0 && fGotMessage != -1) 
    { 
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    }
	return 0;
}

#include <iostream>
//WINAPI message handling
LRESULT CALLBACK HypWinConsole::MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) 
{
	switch(Msg)
	{
		case WM_CREATE:
	    {
			HypWinConsole::messagebox = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), NULL,ES_READONLY | WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL, 0, 0, 635, 280, hWnd, (HMENU)101,
			GetModuleHandle(NULL), NULL);
			SendMessage(HypWinConsole::messagebox, WM_SETFONT, (WPARAM)HypWinConsole::ARIAL, MAKELPARAM(true,0));
			HypWinConsole::commandline = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), NULL, WS_CHILD | WS_VISIBLE | ES_LEFT, 0,287, 635, 20, hWnd, (HMENU)103,GetModuleHandle(NULL), NULL);
			SendMessage(HypWinConsole::commandline, WM_SETFONT, (WPARAM)HypWinConsole::ARIAL, MAKELPARAM(true,0));
			HypWinConsole::lpfnEditWndProc = (WNDPROC) SetWindowLongPtr(HypWinConsole::commandline, GWLP_WNDPROC, (LONG_PTR)HypWinConsole::SubClassProc); 
		}
		break;
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORSTATIC:
		{
			if(GetDlgItem(hWnd,101) == (HWND)lParam)//Message box
			{
				HDC hEdit = (HDC)wParam;
				SetTextColor( hEdit, HypWinConsole::GREEN );
				SetBkColor  ( hEdit, HypWinConsole::BLACK );
				return (LONG)HypWinConsole::BRUSH;//PUT IN GLOBAL VARIABLE
			}
			else if(GetDlgItem(hWnd,103) == (HWND)lParam)//Commandline
			{
				HDC hEdit = (HDC)wParam;
				SetTextColor( hEdit, HypWinConsole::GREEN );
				SetBkColor  ( hEdit, HypWinConsole::BLACK );
				return (LONG)HypWinConsole::BRUSH;//PUT IN GLOBAL VARIABLE
			}
			break;
		}
		case WM_DESTROY:
		{
			//PostQuitMessage(0);
			return 0;
		}
	}
		return DefWindowProc(hWnd, Msg, wParam, lParam);
}

void HypWinConsole::Show()
{
	ShowWindow( window, SW_SHOW);
	bIsVisible = true;
}

void HypWinConsole::Hide()
{
	ShowWindow( window, SW_HIDE);
	bIsVisible = false;
}

///////////////////////////////////////////////////////////
//	Constructors - Destructors

HypWinConsole::HypWinConsole() : hThread(NULL)
{}

HypWinConsole::~HypWinConsole()
{
	DestroyWindow(window);
	DestroyWindow(messagebox);
	DestroyWindow(commandline);
	DeleteObject(BRUSH);
	DeleteObject(ARIAL);
}

///////////////////////////////////////////////////////////
//	Write a line to the console
//	only used by the output handler.

void HypWinConsole::WriteLine(const wchar_t* input)
{
	int idx = GetWindowTextLength(HypWinConsole::messagebox);
    SendMessage(HypWinConsole::messagebox, EM_SETSEL, (WPARAM)idx, (LPARAM)idx);
	SendMessage(HypWinConsole::messagebox, EM_REPLACESEL, 0, (LPARAM)input);
}

//In Subclass Proc
LRESULT CALLBACK HypWinConsole::SubClassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{ 
	switch (msg) 
	{ 
		case WM_KEYDOWN: 
			switch (wParam) 
			{  
				case VK_RETURN: 
					wchar_t buffer[255];
					GetWindowText(hwnd, buffer, 255);
					if(SendCommand)
					{
						(SendCommand)(buffer);//Dispatch a command
						GLog->Log(buffer);
					}
				break;
			} 
		break; 
	} 
	return CallWindowProc(lpfnEditWndProc, hwnd, msg, wParam, lParam); 
} 

#endif