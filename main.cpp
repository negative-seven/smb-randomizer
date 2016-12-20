#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <stdlib.h>
#include <string>
#include <string.h>
#include <tchar.h>
#include <windows.h>

#include "randomize.h"
#include "randomize_game.h"
#include "utils.h"

#include "resource.h"

#define ID_INPUTBOX 0
#define ID_CHECKBOX 1
#define ID_BUTTON 2

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintstruct;
	HINSTANCE hInstance;
	HWND inputbox, checkbox, button;
	HFONT font;

	switch (message)
	{
	case WM_CREATE:
		hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
		font = CreateFont(14, 5, 0, 0, 500, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, __T("Microsoft Sans Serif"));
		
		inputbox = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 10, 84, 19, hWnd, (HMENU)ID_INPUTBOX, hInstance, NULL);
		SendMessage(inputbox, WM_SETFONT, WPARAM(font), TRUE);
		SendMessage(inputbox, EM_SETLIMITTEXT, WPARAM(10), NULL);
		checkbox = CreateWindow("button", "Use random seed", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 100, 10, 150, 20, hWnd, (HMENU)ID_CHECKBOX, hInstance, NULL);
		SendMessage(checkbox, WM_SETFONT, WPARAM(font), TRUE);
		button = CreateWindow("button", "Randomize", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 9, 35, 192, 23, hWnd, (HMENU)ID_BUTTON, hInstance, NULL);
		SendMessage(button, WM_SETFONT, WPARAM(font), TRUE);
		
	case WM_PAINT:
		BeginPaint(hWnd, &paintstruct);
		EndPaint(hWnd, &paintstruct);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case EN_CHANGE:
			switch (LOWORD(wParam))
			{
			case ID_INPUTBOX:
				char text[16] = "";
				DWORD selection_start = 0, selection_end = 0;
				inputbox = GetDlgItem(hWnd, ID_INPUTBOX);
				SendMessage(inputbox, EM_GETSEL, (WPARAM)&selection_start, (LPARAM)&selection_end);
				SendMessage(inputbox, WM_GETTEXT, 16, (LPARAM)text);
				SendMessage(inputbox, WM_SETTEXT, NULL, (LPARAM)limitInputbox(text).c_str());
				SendMessage(inputbox, EM_SETSEL, (WPARAM)&selection_start, (LPARAM)&selection_end);
			}
		}
		case BN_CLICKED:
			switch (LOWORD(wParam))
			{
			case ID_BUTTON:
				unsigned int seed;
				
				checkbox = GetDlgItem(hWnd, ID_CHECKBOX);
				if (SendMessage(checkbox, BM_GETCHECK, NULL, NULL))
				{
					seed = randomInt();
				}
				else
				{
					char seed_temp[16] = "";
					inputbox = GetDlgItem(hWnd, ID_INPUTBOX);
					SendMessage(inputbox, WM_GETTEXT, 16, (LPARAM)seed_temp);
					seed = std::stoi(seed_temp);
				}
				randomize_game(seed);
				break;

			case ID_CHECKBOX:
				checkbox = GetDlgItem(hWnd, ID_CHECKBOX);
				inputbox = GetDlgItem(hWnd, ID_INPUTBOX);
				BOOL checkbox_state = !SendMessage(checkbox, BM_GETCHECK, NULL, NULL);
				SendMessage(checkbox, BM_SETCHECK, checkbox_state, NULL);
				SendMessage(inputbox, EM_SETREADONLY, checkbox_state, NULL);
				break;
			}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wndClassEx;
	static TCHAR window[] = "smb-randomizer";
	static TCHAR window_title[] = "SMB randomizer";
	HWND hWnd;
	MSG msg;
	
	wndClassEx.cbSize = sizeof(WNDCLASSEX);
	wndClassEx.style = CS_HREDRAW | CS_VREDRAW;
	wndClassEx.lpfnWndProc = WndProc;
	wndClassEx.cbClsExtra = 0;
	wndClassEx.cbWndExtra = 0;
	wndClassEx.hInstance = hInstance;
	wndClassEx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClassEx.hbrBackground = CreateSolidBrush(RGB(0xF0, 0xF0, 0xF0));
	wndClassEx.lpszMenuName = NULL;
	wndClassEx.lpszClassName = window;
	wndClassEx.hIconSm = LoadIcon(wndClassEx.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wndClassEx))
	{
		return 1;
	}

	hWnd = CreateWindow(window, window_title, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 226, 106, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return 2;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}