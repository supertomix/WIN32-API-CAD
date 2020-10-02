
#include <windows.h>
#include <iostream>
#include <fstream>
#include <windowsx.h>
#include <stdio.h>
#include <string>

#define _CRT_SECURE_NO_DEPRECATE

using namespace std;


#define  EXIT 5413;

const wchar_t windowClass[] = L"win32app";
const wchar_t windowTitle[] = L"Win32API";

wstring xres;
wstring yres;
wchar_t fres;
wstring md;

static int px, py;
int x, y;
wchar_t f;


BOOL Line(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL); //сделать текущими координаты x1, y1
	return LineTo(hdc, x2, y2);
}

//void display_file(char* path)
//{
//	FILE* file;
//	file = fopen(path, "r");
//	fseek(file, 0, SEEK_END);
//	int _size = ftell(file);
//	rewind(file);
//	char* data = new char(_size + 1);
//	fread(data, _size, 1, file);
//	data[_size] = '\0';
//}

int load_file(HWND hWnd, int *elem, int *px, int *py)
{
	const int len = 5, strings = 3;
	const char ch = '\n';
	char mass[len][strings];
	char* a[1];
	char* b[3];
	ifstream inf("save.txt", ios::in);
	if (!inf)
	{
		exit(1);
	}
	for (int i = 0; i < strings; i++)
	{
		inf.getline(mass[i], len-1, ch);
	}
	a[0] = mass[0];
	b[1] = mass[1];
	



	//cout << a << endl;
	/*char b = mass[1][1];
	char c = mass[1][2];
	*elem  = mass[0][0]-48;*/
	//*px	  = (int)strcat(a, b) ;
	//*py   = mass[2][1];

	inf.close();
	return 0;
}
void open_file(HWND hWnd)
{
	DWORD dwTemp1;
	OPENFILENAME ofn;
	wchar_t szFileName[MAX_PATH] = L"";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = L"txt";
	if (GetOpenFileName(&ofn))
	{
		if (ofn.lpstrFile != L"")
		{
			InvalidateRect(hWnd, NULL, 0);
			HANDLE hFile = CreateFile(
				ofn.lpstrFile,
				GENERIC_READ | GENERIC_WRITE,
				NULL,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL
			);
			SendMessage(hWnd, WM_CLEAR, 0, 0);
			wchar_t buf[20];
			bool b = ReadFile(hFile, buf, sizeof(wchar_t) * 20, &dwTemp1, NULL);
			CloseHandle(hFile); //?

			wchar_t* buf2 = new wchar_t[dwTemp1 / 2 + 1];
			f = buf[0];
			int flag = 0; int i = 3;
			while (buf[i] != L'\r')
				x += buf[i++];
			i += 2;
			while (buf[i] != L'\r')
				y += buf[i++];
			buf2[dwTemp1 / 2] = L'\0';
			MessageBox(hWnd, buf2, L"File name", MB_OK);
		/*	restruct = false;
			paintopen = true;
			paint = false;
			res = false;
			npn = false;
			source = false;
			counter++;*/
			InvalidateRect(hWnd, NULL, 0);
			//onlyopen = true;
		}
		else {
			InvalidateRect(hWnd, NULL, 0);
		}
		
	}	
}

void save_file(HWND hWnd, int flag, int elem, int px, int py)
{
	if (flag == 1) 
	{
		const int len = 10, strings = 10;
		const char ch = '\n';
		char mass[len][strings];
		ofstream onf("save.txt");
		if (!onf)
		{
			MessageBox(hWnd, L"BAD", L"File name", MB_OK);
			exit(1);
		}
		onf << elem << endl;
		onf << px << endl;
		onf << py << endl;
		onf.close();
	}
	if (flag == 2)
	{
		DWORD dwTemp;
		string s;
		// DWORD dwCounter, dwTemp;
		OPENFILENAME ofn1;
		wchar_t szFileName[MAX_PATH] = L"";
		ZeroMemory(&ofn1, sizeof(ofn1));
		ofn1.lStructSize = sizeof(ofn1);
		ofn1.hwndOwner = hWnd;
		ofn1.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
		ofn1.lpstrFile = szFileName;
		ofn1.nMaxFile = MAX_PATH;
		ofn1.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		ofn1.lpstrDefExt = L"txt";
		if (GetSaveFileName(&ofn1))
			MessageBox(hWnd, ofn1.lpstrFile, L"File name", MB_OK);
		HANDLE hFile = CreateFile(
			ofn1.lpstrFile,
			GENERIC_WRITE,
			NULL,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL

		);
		md += to_wstring(elem) + L"\r\n";
		md += to_wstring(px) + L"\r\n";
		md += to_wstring(py) + L"\r\n";
		int l = md.length();
		auto mmd = md.c_str();
		WriteFile(hFile, mmd, sizeof(wchar_t) * l, &dwTemp, NULL);
		CloseHandle(hFile);
		md.clear();
	}
}

long __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	PAINTSTRUCT ps;
	HDC hdc;
	RECT r;
	HWND hwndDlg;
	wchar_t per[32];
	
	static int elem;
	GetClientRect(hWnd, &r);
	int val;
	int flag_for_draw = 1;
	switch (message) 
	{
		case WM_CREATE: 
		{
			
			HMENU hMenubar = nullptr;
			HMENU hFileM = nullptr;
			HMENU hElement = nullptr;

			hMenubar = CreateMenu();
			hFileM = CreateMenu();
			hElement = CreateMenu();

			AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hFileM, L"&File");

			AppendMenu(hFileM, MF_STRING, 5400, L"Open...");
			AppendMenu(hFileM, MF_STRING, 5401, L"Save as...");
			AppendMenu(hFileM, MF_SEPARATOR, NULL, NULL);
			AppendMenu(hFileM, MF_STRING, 5413, L"Exit");


			AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hElement, L"&Element");

			AppendMenu(hElement, MF_STRING, 5402, L"Resistor");
			AppendMenu(hElement, MF_STRING, 5403, L"Npn BJT");
			AppendMenu(hElement, MF_STRING, 5404, L"Current source");
			SetMenu(hWnd, hMenubar);

			load_file(hWnd, &elem, &px, &py);
			SendMessage(hWnd, WM_LBUTTONDOWN, NULL, NULL);
			flag_for_draw = 2;
			DWORD dwTemp3;
			HANDLE hFile = CreateFile(
				L"D:\\save.txt",
				GENERIC_READ,
				NULL,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL
				);
			wchar_t buf1[20];

			bool z = ReadFile(hFile, buf1, sizeof(wchar_t) * 20, &dwTemp3, NULL);
			CloseHandle(hFile);
	
			if (dwTemp3 != 0)
			{
				fres = buf1[0];
				int flag = 0; int i = 3;
				while (buf1[i] != L'\n')
					xres += buf1[i++];
				i += 2;
				while (buf1[i] != L'\n')
					yres += buf1[i++];
				InvalidateRect(hWnd, NULL, 0);
			}
			

			break;
		}
		case WM_COMMAND: 
		{
			switch(LOWORD(wParam)) 
			{
			case 5413: //exit
				{
					val = MessageBox(hWnd,
						L"Вы уверены, что хотите выйти?",
						L"Завершение работы приложения",
						MB_OKCANCEL | MB_ICONEXCLAMATION | MB_DEFBUTTON2);
					if (val == IDOK)
					{
						save_file(hWnd, 1, elem, px, py);
						DestroyWindow(hWnd);
					}
					else
					{
						break;
					}
					break;
				}
			case 5400:
			{
				open_file(hWnd);
				break;
			}
			case 5401:// сохранение как...
			{
				//save_file(hWnd, 2, elem, px, py);
				break;
			}
			case 5402://resistor
			{
				elem = 1;
				SendMessage(hWnd, WM_LBUTTONDOWN, NULL, 0x00800130);
				break;
			}
			case 5403://trans
			{
				elem = 2;
				SendMessage(hWnd, WM_LBUTTONDOWN, NULL, 0x00800130);
				break;
			}
			case 5404://current
			{
				elem = 3;
				SendMessage(hWnd, WM_LBUTTONDOWN, NULL, 0x00800130);
				break;
			} 
			}
			break;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_CLOSE:
		{
			SendMessage(hWnd, WM_COMMAND, 5413, NULL);
			break;
		}
		
		case WM_LBUTTONDOWN:			
			{

				hdc = GetDC(hWnd);

				GetClientRect(hWnd, &r);						//Очистка окна
				FillRect(hdc, &r, (HBRUSH)(COLOR_WINDOW + 1));	//

				
				px = HIWORD(lParam);
				py = LOWORD(lParam);
				
				if (elem == 1)
				{
					TextOut(hdc, 3, 310, L"Resistor", 8);
					Line(hdc, py - 90, px, py - 50, px);
					Rectangle(hdc, py - 50, px - 25, py + 50, px + 25);
					Line(hdc, py + 50, px, py + 90, px);
				}
				else if (elem == 2)
				{
					TextOut(hdc, 3, 310, L"NpN BJT", 7);
					Ellipse(hdc, py - 50, px - 50, py + 50, px + 50);
					Line(hdc, py - 33, px + 17, py + 33, px + 17);//Горизонтальная линия
					Line(hdc, py, px + 17, py, px + 66); // Вертикальная
					Line(hdc, py - 20, px + 17, py - 43, px - 25);
					Line(hdc, py + 20, px + 17, py + 43, px - 25);

					Line(hdc, py - 70, px - 25, py - 43, px - 25); //диагональные линии
					Line(hdc, py + 43, px - 25, py + 70, px - 25);

					Line(hdc, py + 43, px - 25, py + 41, px - 9);
					Line(hdc, py + 43, px - 25, py + 30, px - 12);


				
				}
				else if (elem == 3)
				{
					
					TextOut(hdc, 3, 310, L"Current source", 14);
					Ellipse(hdc, py - 50, px - 50, py + 50, px + 50);

					Line(hdc, py, px - 15, py, px - 70);
					Line(hdc, py, px + 10, py, px + 70); // Вертикальные линии
				

					Line(hdc, py, px - 15, py + 15, px - 5);
					Line(hdc, py, px - 15, py - 15, px - 5); //диагональные линии

					Line(hdc, py, px + 10, py + 15, px + 20);
					Line(hdc, py, px + 10, py - 15, px + 20);
					
				}
				MessageBox(hWnd, md.c_str(), L"File name", MB_OK);
				SendMessage(hWnd, WM_PAINT,NULL,NULL);
				InvalidateRect(hWnd, NULL, 0);
				EndPaint(hWnd, &ps);
			
			}
			
			break;
		case WM_MOUSEMOVE: {

			x = LOWORD(lParam);
			y = HIWORD(lParam);
			swprintf_s(per, L"(%d,%d)", x,y);
			
			SendMessage(hWnd, WM_SETTEXT, NULL, (LPARAM)per);
			break;
		}
		case WM_PAINT: {
			hdc = BeginPaint(hWnd, &ps);
			//резик	

			

			EndPaint(hWnd, &ps);

			break; }


	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = windowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex)) 
	{
		MessageBox(NULL, L"Can’t register window class!", L"Win32 API Test", NULL);
		return 1;
	}

	HWND hWnd = CreateWindow(windowClass, windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, hInstance, NULL);

	if (!hWnd) 
	{
		MessageBox(NULL, L"Can’t create window!", L"Win32 API Test", NULL);
		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

