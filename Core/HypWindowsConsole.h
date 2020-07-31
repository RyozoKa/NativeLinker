/*	A windows only terminal
	Revision:
		Christopher Békési - Initial build
*/

#pragma once


class HypWinConsole : public HypConsole
{
public:
	static HWND messagebox;
	static HWND commandline;
	static HWND window;
	//Colors
	static COLORREF BLACK;
	static COLORREF GREEN;
	static HBRUSH BRUSH;
	static HFONT ARIAL;
	//etc
	static WNDCLASSEX wcx;
	static HINSTANCE hInstance;
	HANDLE hThread;
	static WNDPROC lpfnEditWndProc;

	DLLEXPORT HypWinConsole();
	DLLEXPORT ~HypWinConsole();//Take care of any brushes etc!

	//Console interface
	DLLEXPORT virtual void Initialize();
	static void WriteLine(const wchar_t*);
	DLLEXPORT virtual void Hide();
	DLLEXPORT virtual void Show();

	static	LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
	static	LRESULT CALLBACK SubClassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};