#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <stdlib.h>
#include <string>
#include <string.h>
#include <tchar.h>
#include <windows.h>

#include "global.h"
#include "randomize.h"
#include "randomize_game.h"
#include "utils.h"

#include "resource.h"

#define ID_INPUTBOX 0
#define ID_CHECKBOX_RANDSEED 1
#define ID_BUTTON_RANDOMIZE 2
#define ID_BUTTON_SETTINGS 3
#define ID_CHECKBOX_SETTINGS_LEVELS 4
#define ID_CHECKBOX_SETTINGS_CHARS 5

#define DEFAULT_INPUTBOX ""
#define DEFAULT_CHECKBOX_RANDSEED false
#define DEFAULT_BUTTON_SETTINGS false
#define DEFAULT_CHECKBOX_SETTINGS_LEVELS true
#define DEFAULT_CHECKBOX_SETTINGS_CHARS false

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintstruct;
	HINSTANCE hInstance;
	HWND inputbox, checkbox_randseed, button_randomize, button_settings, checkbox_settings_levels, checkbox_settings_chars;
	HFONT font;
	std::string temp_string;
	int temp_int;

	switch (message)
	{
	case WM_CREATE:
		hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
		font = CreateFont(14, 5, 0, 0, 500, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, __T("Microsoft Sans Serif"));
		
		inputbox = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 10, 84, 19, hWnd, (HMENU)ID_INPUTBOX, hInstance, NULL);
		SendMessage(inputbox, WM_SETFONT, WPARAM(font), TRUE);
		SendMessage(inputbox, EM_SETLIMITTEXT, WPARAM(10), NULL);
		SendMessage(inputbox, EM_SETREADONLY, WPARAM(DEFAULT_CHECKBOX_RANDSEED), NULL);

		checkbox_randseed = CreateWindow("button", "Use random seed", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 100, 10, 150, 20, hWnd, (HMENU)ID_CHECKBOX_RANDSEED, hInstance, NULL);
		SendMessage(checkbox_randseed, WM_SETFONT, WPARAM(font), TRUE);
		SendMessage(checkbox_randseed, BM_SETCHECK, WPARAM(DEFAULT_CHECKBOX_RANDSEED), NULL);

		button_randomize = CreateWindow("button", "Randomize", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 9, 35, 192, 23, hWnd, (HMENU)ID_BUTTON_RANDOMIZE, hInstance, NULL);
		SendMessage(button_randomize, WM_SETFONT, WPARAM(font), TRUE);

		#if DEFAULT_BUTTON_SETTINGS == true
		temp_string = "Settings [-]";
		temp_int = SW_SHOW;
		#else
		temp_string = "Settings [+]";
		temp_int = SW_HIDE;
		#endif
		button_settings = CreateWindow("button", temp_string.c_str(), WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 9, 62, 192, 23, hWnd, (HMENU)ID_BUTTON_SETTINGS, hInstance, NULL);
		SendMessage(button_settings, WM_SETFONT, WPARAM(font), TRUE);
		settings_shown = DEFAULT_BUTTON_SETTINGS;

		checkbox_settings_levels = CreateWindow("button", "Randomize levels", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 11, 88, 150, 20, hWnd, (HMENU)ID_CHECKBOX_SETTINGS_LEVELS, hInstance, NULL);
		SendMessage(checkbox_settings_levels, WM_SETFONT, WPARAM(font), TRUE);
		SendMessage(checkbox_settings_levels, BM_SETCHECK, WPARAM(DEFAULT_CHECKBOX_SETTINGS_LEVELS), NULL);
		ShowWindow(checkbox_settings_levels, temp_int);
		settings_levels = DEFAULT_CHECKBOX_SETTINGS_LEVELS;

		checkbox_settings_chars = CreateWindow("button", "Randomize characters (unseeded)", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 11, 110, 180, 20, hWnd, (HMENU)ID_CHECKBOX_SETTINGS_CHARS, hInstance, NULL);
		SendMessage(checkbox_settings_chars, WM_SETFONT, WPARAM(font), TRUE);
		SendMessage(checkbox_settings_chars, BM_SETCHECK, WPARAM(DEFAULT_CHECKBOX_SETTINGS_CHARS), NULL);
		ShowWindow(checkbox_settings_chars, temp_int);
		settings_chars = DEFAULT_CHECKBOX_SETTINGS_CHARS;

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
			BOOL checkbox_state;

			switch (LOWORD(wParam))
			{
			case ID_CHECKBOX_RANDSEED:
				checkbox_randseed = GetDlgItem(hWnd, ID_CHECKBOX_RANDSEED);
				inputbox = GetDlgItem(hWnd, ID_INPUTBOX);
				checkbox_state = !SendMessage(checkbox_randseed, BM_GETCHECK, NULL, NULL);
				SendMessage(checkbox_randseed, BM_SETCHECK, checkbox_state, NULL);
				SendMessage(inputbox, EM_SETREADONLY, checkbox_state, NULL);
				break;
			case ID_BUTTON_SETTINGS:
				button_settings = GetDlgItem(hWnd, ID_BUTTON_SETTINGS);
				checkbox_settings_levels = GetDlgItem(hWnd, ID_CHECKBOX_SETTINGS_LEVELS);
				checkbox_settings_chars = GetDlgItem(hWnd, ID_CHECKBOX_SETTINGS_CHARS);
				settings_shown = !settings_shown;
				if (settings_shown)
				{
					SetWindowPos(hWnd, NULL, NULL, NULL, 226, 173, SWP_NOMOVE | SWP_NOREPOSITION);
					SetWindowText(button_settings, "Settings [-]");
					ShowWindow(checkbox_settings_levels, SW_SHOW);
					ShowWindow(checkbox_settings_chars, SW_SHOW);
				}
				else
				{
					SetWindowPos(hWnd, NULL, NULL, NULL, 226, 133, SWP_NOMOVE | SWP_NOREPOSITION);
					SetWindowText(button_settings, "Settings [+]");
					ShowWindow(checkbox_settings_levels, SW_HIDE);
					ShowWindow(checkbox_settings_chars, SW_HIDE);
				}
				break;
			case ID_BUTTON_RANDOMIZE:
				unsigned int seed;
				checkbox_randseed = GetDlgItem(hWnd, ID_CHECKBOX_RANDSEED);
				if (SendMessage(checkbox_randseed, BM_GETCHECK, NULL, NULL))
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
				randomize_levels(seed);
				break;
			case ID_CHECKBOX_SETTINGS_LEVELS:
				settings_levels = !settings_levels;
				checkbox_settings_levels = GetDlgItem(hWnd, ID_CHECKBOX_SETTINGS_LEVELS);
				SendMessage(checkbox_settings_levels, BM_SETCHECK, settings_levels, NULL);
				break;
			case ID_CHECKBOX_SETTINGS_CHARS:
				settings_chars = !settings_chars;
				checkbox_settings_chars = GetDlgItem(hWnd, ID_CHECKBOX_SETTINGS_CHARS);
				SendMessage(checkbox_settings_chars, BM_SETCHECK, settings_chars, NULL);
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

	#if DEFAULT_BUTTON_SETTINGS == true
	int width = 226;
	int height = 173;
	#else
	int width = 226;
	int height = 133;
	#endif

	hWnd = CreateWindow(window, window_title, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, NULL);

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