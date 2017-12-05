// HelloWorld.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include<io.h>
#include<locale>
#include<codecvt>
#include <objidl.h>
#include <gdiplus.h>
#include<vector>
#include<fcntl.h>
#include <string>
#include <atlstr.h>
#include <ddraw.h>
#include <windowsx.h>
#include <Commctrl.h>
#include "resource.h"
using namespace std;
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
//using namespace System.Drawing;
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")

#ifndef UNICODE
typedef std::string String
#else
typedef std::wstring String
#endif;

#define MAX_LOADSTRING 100
#define WinDowsY 525
#define WinDowsX 800

#define CMBNGUON 1000
#define  CMBLOAICHITIEU 1001
#define TIENNOW 1002
#define TIEN 1003
#define GHICHU 1004
#define THEM 1005
#define QUANLY 1006
#define  THONGKE 1007
#define  TONGTHU 1008
#define  TONGCHI 1009
#define XOA 1010
;

void OnPaint(HWND hwnd);
void OnDestroy(HWND hwnd);
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnInSert(HWND);
void HienHienAnh(HDC hdc, WCHAR *duongdan, int x, int y, int width, int height);
void Update_Money(char* Loai, TCHAR* cTien);
bool Update_ThongKe();
bool GhiFileText(char Loai_Nguon[][100], TCHAR* DateTime, TCHAR* cTien, TCHAR* cGhiChu);
float TinhGocQuay(float phanTram);
void VeBieuDo(HDC hdc);
bool Update_Thu_Chi();
bool flag = false;
int tongthu = 0;
int tongchi = 0;
float PhanTram[10];//Lưu phần trăm để vẽ biểu đồ
// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
HWND SoDu;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_HELLOWORLD, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HELLOWORLD));

	// Main message loop:
	HWND hfc;
	WCHAR clas[10];
	while (GetMessage(&msg, NULL, 0, 0))
	{
		hfc = GetFocus();
		GetClassName(hfc, clas, 10);
		if (_wcsicmp(clas, L"Edit") == 0 && hfc == SoDu)
		{
			if (msg.message == WM_KEYDOWN)
				continue;
		}
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDI_ICON1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, L"Quan Ly Chi Tieu 1.0", WS_OVERLAPPEDWINDOW,
		500, 50, WinDowsX, WinDowsY, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//

GdiplusStartupInput gdiPlusStartupInput;
ULONG_PTR gdiPlusToken;
HWND  cmb_Nguon;
HWND cmb_LoaiChiTieu;
HWND Tien;
HWND GhiChu;
HWND DatePicker;
HWND Them;
HWND ThongKe;
HWND TongThu;
HWND TongChi;
HWND Xoa;
HWND Listview;
HFONT hFont;


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = NULL;

	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
	case WM_CTLCOLORSTATIC: // dùng để thay đổi màu lớp static
		SetTextColor((HDC)wParam, RGB(0, 0, 160));
		SetBkMode((HDC)wParam, TRANSPARENT);
		return (LRESULT)GetStockObject(NULL_BRUSH);

		
	case WM_COMMAND:
		if (LOWORD(wParam) == TIENNOW)
		{
			if (HIWORD(wParam) == EN_CHANGE)
			{
				wParam = 0;
				return 0;
			}
		}
		if (LOWORD(wParam) == CMBNGUON)
		{
			switch (HIWORD(wParam))
			{
			case CBN_DROPDOWN:
				break;
			case BN_CLICKED:
				break;
			case CBN_SELCHANGE:
				int idx_row;
				char strText[255] = { 0 };
				idx_row = SendMessage(cmb_Nguon, CB_GETCURSEL, 0, 0);
				SendMessage(cmb_Nguon, CB_GETLBTEXT, idx_row, (LPARAM)strText);
				if (idx_row == 0)
				{
					SendDlgItemMessage(hWnd, CMBLOAICHITIEU, CB_RESETCONTENT, 0, 0);
				}

				if (idx_row == 1)//Thu
				{
					SendDlgItemMessage(hWnd, CMBLOAICHITIEU, CB_RESETCONTENT, 0, 0);
					SendMessage(cmb_LoaiChiTieu, CB_DELETESTRING, 0, 0);
					SendMessage(cmb_LoaiChiTieu, CB_ADDSTRING, 0, (LPARAM)_T("Lương"));
					SendMessage(cmb_LoaiChiTieu, CB_ADDSTRING, 0, (LPARAM)_T("Vay Nợ"));
					SendMessage(cmb_LoaiChiTieu, CB_ADDSTRING, 0, (LPARAM)_T("Thu Nhập Thêm"));
					SendMessage(cmb_LoaiChiTieu, CB_ADDSTRING, 0, (LPARAM)_T("Kinh Doanh"));

					SendMessage(cmb_LoaiChiTieu, CB_SETCURSEL, 0, 0); //highlight/select the first option
				}

				if (idx_row == 2)//Chi
				{
					SendDlgItemMessage(hWnd, CMBLOAICHITIEU, CB_RESETCONTENT, 0, 0);
					SendMessage(cmb_LoaiChiTieu, CB_ADDSTRING, 0, (LPARAM)_T("Di Chuyển"));
					SendMessage(cmb_LoaiChiTieu, CB_ADDSTRING, 0, (LPARAM)_T("Đồ Ăn/ Thức Uống"));
					SendMessage(cmb_LoaiChiTieu, CB_ADDSTRING, 0, (LPARAM)_T("Gia Đình/ Cá Nhân"));
					SendMessage(cmb_LoaiChiTieu, CB_ADDSTRING, 0, (LPARAM)_T("Hóa Đơn"));
					SendMessage(cmb_LoaiChiTieu, CB_ADDSTRING, 0, (LPARAM)_T("Giải Trí"));
					SendMessage(cmb_LoaiChiTieu, CB_ADDSTRING, 0, (LPARAM)_T("Mua Sắm"));

					SendMessage(cmb_LoaiChiTieu, CB_SETCURSEL, 0, 0); //highlight/select the first option
				}

				break;
			}
		}
		if (LOWORD(wParam) == XOA)
		{
			SendMessage(cmb_Nguon, CB_SETCURSEL, 0, 0);
			SendDlgItemMessage(hWnd, CMBLOAICHITIEU, CB_RESETCONTENT, 0, 0);
			SetWindowText(Tien, L"");
			SetWindowText(GhiChu, L"");
		}

		if (LOWORD(wParam) == THEM)
		{
			bool flag = true;
			char strText1[3][100];
			int idx_row[4];
			idx_row[0] = SendMessage(cmb_Nguon, CB_GETCURSEL, 0, 0);
			idx_row[1] = SendMessage(cmb_LoaiChiTieu, CB_GETCURSEL, 0, 0);

			TCHAR* strTien = new TCHAR[100];
			TCHAR* strGhiChu = new TCHAR[100];
			TCHAR* DateTime = new TCHAR[100];

			SendMessage(cmb_Nguon, CB_GETLBTEXT, idx_row[0], (LPARAM)strText1[0]);
			SendMessage(cmb_LoaiChiTieu, CB_GETLBTEXT, idx_row[1], (LPARAM)strText1[1]);

			GetWindowText(DatePicker, DateTime, 100);
			GetWindowText(Tien, strTien, 100);
			GetWindowText(GhiChu, strGhiChu, 100);

			//Ghi file
			bool write = GhiFileText(strText1, DateTime, strTien, strGhiChu);
			if (!write){
				MessageBox(hWnd, L"Kiểm tra lại file1!", L"Trạng Thái", 0);
				break;
			}

			//Cập nhập lại tiền trong file money.
			Update_Money(strText1[0], strTien);
			MessageBox(hWnd, L"Thêm Thành Công", L"Trạng Thái", 0);

			//Cập nhật lại THONGKE
			write = Update_ThongKe();
			if (!write){
				MessageBox(hWnd, L"Kiểm tra lại file2!", L"Trạng Thái", 0);
				break;
			}
			//Cập nhật lại thuchi
			write = Update_Thu_Chi();
			if (!write){
				MessageBox(hWnd, L"Kiểm tra lại file3!", L"Trạng Thái", 0);
				break;
			}

			//Cập nhật lại trạng thái trống
			SendMessage(cmb_Nguon, CB_SETCURSEL, 0, 0);
			SendDlgItemMessage(hWnd, CMBLOAICHITIEU, CB_RESETCONTENT, 0, 0);
			SetWindowText(Tien, L"");
			SetWindowText(GhiChu, L"");

			//Cập nhật biểu đồ
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void OnPaint(HWND hWnd)
{
	if (flag = true){
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hWnd, &ps);
		HienHienAnh(hdc, L"4.bmp", 0, 0, 1024, 699);
		HienHienAnh(hdc, L"8.bmp", 12, 12, 100, 99);
		//
		VeBieuDo(hdc);
		EndPaint(hWnd, &ps);
	}
}

// Message handler for about box.
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{

	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	InitCommonControls();
	GdiplusStartup(&gdiPlusToken, &gdiPlusStartupInput, NULL);

	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	hFont = CreateFont(lf.lfHeight - 1, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	HWND hwnd = CreateWindowEx(0, L"STATIC", L"Quản lý chi tiêu chưa bao giờ dễ dàng đến thế!", WS_CHILD | WS_VISIBLE | SS_CENTER, 146, 98, 230, 13, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd = CreateWindowEx(0, L"STATIC", L"Số dư", WS_CHILD | WS_VISIBLE, 178, 33, 47, 18, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd = CreateWindowEx(0, L"STATIC", L"Hôm nay", WS_CHILD | WS_VISIBLE, 12, 144, 49, 13, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd = CreateWindowEx(0, L"STATIC", L"Chi:", WS_CHILD | WS_VISIBLE, 613, 164, 41, 13, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd = CreateWindowEx(0, L"STATIC", L"Thu:", WS_CHILD | WS_VISIBLE, 400, 164, 100, 20, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd = CreateWindowEx(0, L"STATIC", L"Ngày cụ thể:", WS_CHILD | WS_VISIBLE, 100, 144, 65, 13, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd = CreateWindowEx(0, L"STATIC", L"-------------------------------------------------------------------------------------------------------------------",
		WS_CHILD | WS_VISIBLE | SS_CENTER, 12, 157, 352, 13, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	hFont = CreateFont(lf.lfHeight - 2, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	hwnd = CreateWindowEx(0, L"STATIC", L"Nguồn:", WS_CHILD | WS_VISIBLE, 35, 198, 52, 18, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
	hwnd = CreateWindowEx(0, L"STATIC", L"Loại chi tiêu:", WS_CHILD | WS_VISIBLE, 35, 246, 94, 18, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd = CreateWindowEx(0, L"STATIC", L"Tiền:", WS_CHILD | WS_VISIBLE, 35, 297, 36, 18, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	GhiChu = CreateWindowEx(0, L"STATIC", L"Ghi chú:", WS_CHILD | WS_VISIBLE, 35, 361, 59, 18, hWnd, NULL, hInst, NULL);
	SendMessage(GhiChu, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd = CreateWindowEx(0, L"STATIC", L"-------------------------------------------------------------------------------------------------------------------",
		WS_CHILD | WS_VISIBLE | SS_CENTER, 12, 398, 352, 13, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	cmb_Nguon = CreateWindowEx(0, L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_HASSTRINGS, 240, 195, 132, 21, hWnd, (HMENU)CMBNGUON, hInst, NULL);
	SendMessage(cmb_Nguon, WM_SETFONT, WPARAM(hFont), TRUE);

	cmb_LoaiChiTieu = CreateWindowEx(0, L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWN, 240, 243, 132, 21, hWnd, (HMENU)CMBLOAICHITIEU, hInst, NULL);
	SendMessage(cmb_LoaiChiTieu, WM_SETFONT, WPARAM(hFont), TRUE);

	TongChi = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT, 672, 161, 100, 20, hWnd, (HMENU)TONGCHI, hInst, NULL);
	SendMessage(TongChi, WM_SETFONT, WPARAM(hFont), TRUE);

	TongThu = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT, 470, 161, 100, 20, hWnd, (HMENU)TONGTHU, hInst, NULL);
	SendMessage(TongThu, WM_SETFONT, WPARAM(hFont), TRUE);

	Tien = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER | ES_NUMBER, 240, 287, 132, 28, hWnd, (HMENU)TIEN, hInst, NULL);
	SendMessage(Tien, WM_SETFONT, WPARAM(hFont), TRUE);

	GhiChu = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER | ES_MULTILINE, 149, 340, 223, 39, hWnd, (HMENU)GHICHU, hInst, NULL);
	SendMessage(GhiChu, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd = CreateWindowEx(0, L"BUTTON", L"Thêm", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 240, 425, 132, 39, hWnd, (HMENU)THEM, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd = CreateWindowEx(0, L"BUTTON", L"Xóa", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 425, 132, 39, hWnd, (HMENU)XOA, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	DatePicker = CreateWindowEx(0, DATETIMEPICK_CLASS, TEXT("DateTime"), WS_CHILD | WS_VISIBLE |DTS_LONGDATEFORMAT, 172, 138, 200, 20, hWnd, NULL, hInst, NULL);

	Listview = CreateWindowEx(NULL, WC_LISTVIEWW, L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | LVS_REPORT | WS_BORDER | LVS_EX_BORDERSELECT,
		379, 23, 393, 135, hWnd, (HMENU)IDC_LISTVIEW, hInst, NULL);

	hwnd = CreateWindowEx(0, L"STATIC", L"Biểu đồ tỷ lệ các loại chi tiêu", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 500, 225, 750, 220 + 75, hWnd, (HMENU)THEM, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd = CreateWindowEx(0, L"STATIC", L"Di Chuyển", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 695, 285, 750, 220 + 75, hWnd, (HMENU)THEM, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd = CreateWindowEx(0, L"STATIC", L"Đồ ăn", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 695, 315, 750, 220 + 75, hWnd, (HMENU)THEM, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd = CreateWindowEx(0, L"STATIC", L"Gia Đình", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 695, 345, 750, 220 + 75, hWnd, (HMENU)THEM, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd = CreateWindowEx(0, L"STATIC", L"Hóa Đơn", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 695, 375, 750, 220 + 75, hWnd, (HMENU)THEM, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd = CreateWindowEx(0, L"STATIC", L"Giải Trí", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 695, 405, 750, 220 + 75, hWnd, (HMENU)THEM, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd = CreateWindowEx(0, L"STATIC", L"Mua Sắm", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 695, 435, 750, 220 + 75, hWnd, (HMENU)THEM, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
	
	hFont = CreateFont(lf.lfHeight - 15, lf.lfWidth -5,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);
	SoDu = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER | ES_RIGHT, 240, 23, 132, 37, hWnd, (HMENU)TIENNOW, hInst, NULL);
	SendMessage(SoDu, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(SoDu, EM_SETREADONLY, 0, 0);

	//--------------------------------------

	//Send combobox items individually

	SendMessage(cmb_Nguon, CB_ADDSTRING, 0, (LPARAM)_T("..."));
	SendMessage(cmb_Nguon, CB_ADDSTRING, 0, (LPARAM)_T("Thu"));
	SendMessage(cmb_Nguon, CB_ADDSTRING, 0, (LPARAM)_T("Chi"));
	SendMessage(cmb_Nguon, CB_SETCURSEL, 0, 0); //highlight/select the first option

	//Init monney in bucket
	fstream file;
	file.open("money.txt", ios::in);

	if (!file.fail())
	{

		string s;
		file >> s;

		//Thêm chấm vào đơn vị
		int dem = 0;
		for (int i = s.length() - 1; i >= 0; i--){
			if (dem % 3 == 0 && i != s.length() - 1){
				s.insert(i + 1, ".");
			}
			dem++;
		}

		s += " VND";
		
		wstring stemp = wstring(s.begin(), s.end());
		
		LPCWSTR result = stemp.c_str();
		SetWindowText(SoDu, (LPWSTR)result);
	}
	file.close();

	_setmode(_fileno(stdout), _O_WTEXT); //needed for output
	_setmode(_fileno(stdin), _O_WTEXT); //needed for input

	wfstream filelog;
	filelog.open("file.txt", ios::in);


	Update_ThongKe();

	Update_Thu_Chi();


	flag = true;
	OnPaint(hWnd);



	return true;
}

void LoadCol(WCHAR* s, int pos, int size)
{
	LVCOLUMN lvCol;
	lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvCol.fmt = LVCFMT_LEFT;
	lvCol.pszText = s;
	lvCol.cx = size;
	ListView_InsertColumn(Listview, pos, &lvCol);
}

void OnDestroy(HWND hwnd)
{
	PostQuitMessage(0);
}

bool Update_Thu_Chi()
{
	wfstream filelog;
	int iDiChuyen = 0, iDoanThucuong = 0, iGiadinhCanhan = 0, iHoaDon = 0, iGiaitri = 0, iMuasam = 0;
	int thu = 0, chi = 0, sum;
	filelog.open("file.txt", ios::in);

	if (filelog.fail())
		return false;

	const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
	filelog.imbue(utf8_locale);

	wstring s, ss, loai;
	while (getline(filelog, s)){
		if (s.length() < 2)
			return false;
		int start;
		int end = s.find_last_of(L",");

		int pos_tc = s.find_first_of(L",");
		ss = s.substr(0, end);
		start = ss.find_last_of(L",");

		ss = ss.substr(start + 2, end);
		end = pos_tc;

		loai = s.substr(pos_tc + 2, 3);

		//Loc loại chi tiêu
		int loaichitieu;
		int i = 0;
		while (i != 2){
			s = s.substr(s.find_first_of(L",") + 2);
			i++;
		}
		s = s.erase(s.find_first_of(L","));

		if (loai == L"Thu")
			thu = thu + _wtoi(ss.c_str());
		else if (loai == L"Chi"){
			chi = chi + _wtoi(ss.c_str());
			if (s == L"Di Chuyển")
				iDiChuyen += _wtoi(ss.c_str());
			else if (s == L"Đồ Ăn/ Thức Uống")
				iDoanThucuong += _wtoi(ss.c_str());
			else if (s == L"Gia Đình/ Cá Nhân")
				iGiadinhCanhan += _wtoi(ss.c_str());
			else if (s == L"Hóa Đơn")
				iHoaDon += _wtoi(ss.c_str());
			else if (s == L"Giải Trí")
				iGiaitri += _wtoi(ss.c_str());
			else if (s == L"Mua Sắm")
				iMuasam += _wtoi(ss.c_str());
		}

	}

	sum = iDiChuyen + iDoanThucuong + iGiadinhCanhan + iHoaDon + iGiaitri + iMuasam;
	if (sum == 0)
		sum = 1;

	PhanTram[0] = ((float)iDiChuyen / sum) * 100;
	PhanTram[1] = ((float)iDoanThucuong / sum) * 100;
	PhanTram[2] = ((float)iGiadinhCanhan / sum) * 100;
	PhanTram[3] = ((float)iHoaDon / sum) * 100;
	PhanTram[4] = ((float)iGiaitri / sum) * 100;
	PhanTram[5] = ((float)iMuasam / sum) * 100;

	tongthu = thu;
	tongchi = chi;
	s = to_wstring(thu);
	ss = to_wstring(chi);

	//Thêm chấm vào đơn vị
	int dem = 0;
	for (int i = s.length() - 1; i >= 0; i--){
		if (dem % 3 == 0 && i != s.length() - 1){
			s.insert(i + 1, L".");
		}
		dem++;
	}
	s += L" VND";
	//Thêm chấm vào đơn vị
	dem = 0;
	for (int i = ss.length() - 1; i >= 0; i--){
		if (dem % 3 == 0 && i != ss.length() - 1){
			ss.insert(i + 1, L".");
		}
		dem++;
	}
	ss += L" VND";

	LPCWSTR result_thu = s.c_str();
	LPCWSTR result_chi = ss.c_str();

	SetWindowText(TongChi, (LPWSTR)result_chi);
	SetWindowText(TongThu, (LPWSTR)result_thu);
	filelog.close();

	return true;
}

void HienHienAnh(HDC hdc, WCHAR *duongdan, int x, int y, int width, int height)
{
	HDC MemDC1, MemDC2;
	MemDC1 = CreateCompatibleDC(hdc);
	MemDC2 = CreateCompatibleDC(hdc);

	HBITMAP hbmp1, hbmp2;
	hbmp1 = (HBITMAP)LoadImage(hInst, duongdan, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbmp2 = CreateCompatibleBitmap(hdc, width, height);

	SelectObject(MemDC1, hbmp1);
	SelectObject(MemDC2, hbmp2);
	BITMAP bm;	//Dung de lay kich thuoc anh bitmap
	GetObject(hbmp1, sizeof(BITMAP), &bm);
	StretchBlt(MemDC2, 0, 0, width, height, MemDC1, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	BitBlt(hdc, x, y, width, height, MemDC2, 0, 0, SRCCOPY);// tao quan truyen x, y vao
}

void Update_Money(char* Loai, TCHAR* cTien)
{
	fstream filemoney;
	filemoney.open("money.txt", ios::in | ios::out);

	if (!filemoney.fail())
	{

		LPCWSTR  hLoai = (LPWSTR)Loai;
		string bucket;
		string strLoai = CW2A(hLoai);
		filemoney >> bucket;

		int iButket = atoi(bucket.c_str());
		int iMoney = _wtoi(cTien);

		if (strLoai == "Thu")
			iButket += iMoney;
		else iButket -= iMoney;

		filemoney.close();

		filemoney.open("money.txt");
		filemoney << iButket;
		filemoney.close();

		bucket = to_string(iButket);

		int dem = 0;
		for (int i = bucket.length() - 1; i >= 0; i--){
			if (dem % 3 == 0 && i != bucket.length() - 1){
				bucket.insert(i + 1, ".");
			}
			dem++;
		}

		bucket += " VND";
		wstring stemp = wstring(bucket.begin(), bucket.end());
		LPCWSTR result = stemp.c_str();


		SetWindowText(SoDu, (LPWSTR)result);
	}
}

bool Update_ThongKe()
{
	LoadCol(L"Thời gian", 0, 125);
	LoadCol(L"Loại chi tiêu", 1, 100);
	LoadCol(L"Tiền", 2, 80);
	LoadCol(L"Ghi chú", 3, 100);

	wfstream filelog;
	filelog.open("file.txt", ios::in);

	if (filelog.fail())
		return false;

	const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
	filelog.imbue(utf8_locale);

	int posstar = 0, posend = 0;
	wstring s, ss, item[5];
	while (getline(filelog, s)){
		int i = 0;
		while (s.length() > 0){
			posend = s.find_first_of(L",");
			ss = s.substr(posstar + 1, posend - posstar - 1);
			s = s.substr(posend - posstar + 1);
			item[i] = ss;
			i++;
			if (i == 5)
				break;
		}
			LVITEM lvi;
			lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
			lvi.iItem = 0;
			lvi.iSubItem = 0;
			lvi.pszText = (LPWSTR)item[0].c_str();
			lvi.iImage = 0;
			lvi.cchTextMax = 50;

			ListView_InsertItem(Listview, &lvi);

			ListView_SetItemText(Listview, 0, 1, (LPWSTR) item[2].c_str());
			ListView_SetItemText(Listview, 0, 2, (LPWSTR)item[3].c_str());
			ListView_SetItemText(Listview, 0, 3, (LPWSTR)item[4].c_str());
		}
	
	filelog.close();

	return true;
}

bool GhiFileText(char Loai_Nguon[][100], TCHAR* DateTime, TCHAR* cTien, TCHAR* cGhiChu)
{
	wfstream file;
	fstream filerecord;
	string snum_record;
	int inum_record;

	file.open("file.txt", ios::out | ios::app);

	if (file.fail())
		return false;

	filerecord >> snum_record;
	inum_record = atoi(snum_record.c_str());

	//Ghi file
	wstring ws2(DateTime), result;
	result += ws2 + L", ";
	for (int i = 0; i < 2; i++){
		LPCWSTR  sw = (LPWSTR)Loai_Nguon[i];
		wstring str2(sw);
		result += str2 + L", ";
	}

	////Let's convert to string...
	wstring ws(cTien);
	string s(ws.begin(), ws.end());
	result += ws + L", ";

	wstring  ws1(cGhiChu);
	result += ws1;
	result += L"\r\n";

	const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
	file.imbue(utf8_locale);
	file << result;
	inum_record++;

	file.close();
	filerecord.close();

	return true;
}

float TinhGocQuay(float phanTram) {
	return (float)((phanTram * 360) / 100);
}

vector<SolidBrush*> CreatBrush() {
	SolidBrush* sDoan = new SolidBrush(Color(255, 255, 255, 255));
	SolidBrush* sDiChuyen = new SolidBrush(Color(255, 224, 224, 254));
	SolidBrush* sGiaDinh = new SolidBrush(Color(255, 177, 177, 252));
	SolidBrush* sHoaDon = new SolidBrush(Color(255, 135, 135, 251));
	SolidBrush* sGiaiTri = new SolidBrush(Color(255, 100, 100, 250));
	SolidBrush* sMuaSam = new SolidBrush(Color(255, 50, 50, 249));
	vector<SolidBrush*> dsBrush;
	dsBrush.push_back(sDoan);
	dsBrush.push_back(sDiChuyen);
	dsBrush.push_back(sGiaDinh);
	dsBrush.push_back(sHoaDon);
	dsBrush.push_back(sGiaiTri);
	dsBrush.push_back(sMuaSam);
	return dsBrush;
}

void VeBieuDo(HDC hdc)
{
	Graphics* graphics = new Graphics(hdc);
	graphics->SetSmoothingMode(SmoothingModeAntiAlias);
	GraphicsPath Path[10];
	Pen* pen = new Pen(Color(255, 0, 0, 0), 1.5);
	vector<SolidBrush*> BrushPath = CreatBrush();
	float startAngle = 270;
	float endAngle = 0;
	bool isEmpty = true;

	for (int i = 0; i < 6; i++)
	if (PhanTram[i] != 0)
		isEmpty = false;


	if (isEmpty){
		Path[7].AddPie(415, 265, 250, 250, 270, 360);
		graphics->FillPath(new SolidBrush(Color(255, 64, 108, 191)), &Path[7]);

		for (int i = 0; i < 6; i++){
			Path[i].AddPie(625, 230 + i * 35, 75, 75, 330, 30);
			graphics->DrawPath(pen, &Path[i]);
			graphics->FillPath(BrushPath[i], &Path[i]);
		}
	}
	else{
		for (int i = 0; i < 6; i++) {
			Path[i].AddPie(595, 265 + i * 30, 75, 75, 330, 30);
			endAngle = TinhGocQuay(PhanTram[i]);
			Path[i].AddPie(390, 265, 200, 200, startAngle, endAngle);
			graphics->FillPath(BrushPath[i], &Path[i]);
			graphics->DrawPath(pen, &Path[i]);
			Path[i].~GraphicsPath();
			startAngle = startAngle + endAngle;
		}
	}
	delete pen;
	delete graphics;
}

