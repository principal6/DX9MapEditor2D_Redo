#include "DX9MapEditor.h"
#include "resource.h"

using namespace DX9ENGINE;

DX9Base* DX9MapEditor::ms_BaseParent;
DX9Base* DX9MapEditor::ms_BaseLeft;
DX9Base* DX9MapEditor::ms_BaseRight;
RECT DX9MapEditor::ms_TempRect;

LRESULT CALLBACK DlgProcNewMap(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

RECT DX9ENGINE::GetLeftChildWindowPositionAndSize(RECT Rect)
{
	RECT Result = {0, 0,
		DX9MapEditor::WINDOW_SEPERATE_X - DX9MapEditor::WINDOW_VSCROLL_SIZE,
		Rect.bottom - DX9MapEditor::WINDOW_HSCROLL_SIZE };
	return Result;
}

RECT DX9ENGINE::GetRightChildWindowPositionAndSize(RECT Rect)
{
	RECT Result = { DX9MapEditor::WINDOW_SEPERATE_X + DX9MapEditor::WINDOW_SEPERATE_INTERVAL, 0,
		Rect.right - DX9MapEditor::WINDOW_SEPERATE_X - DX9MapEditor::WINDOW_VSCROLL_SIZE - DX9MapEditor::WINDOW_PADDING_X,
		Rect.bottom - DX9MapEditor::WINDOW_HSCROLL_SIZE };
	return Result;
}

LRESULT CALLBACK DX9ENGINE::ParentWindowProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_COMMAND:
		// Menu & accelerator
		switch (LOWORD(wParam)) // in LOWORD of wParam is the resource id
		{
		case ID_MAP_NEWMAP:
			// Show new map dialog
			DialogBox(nullptr, MAKEINTRESOURCE(IDD_DIALOG1), DX9MapEditor::ms_BaseParent->GethWnd(), DlgProcNewMap);
			break;
		}

		break;
	case WM_SIZE:
		// Resize ms_BaseLeft and ms_BaseRight and their correspondent window in accordance with the parent window size
		if (DX9MapEditor::ms_BaseLeft && DX9MapEditor::ms_BaseRight)
		{
			// Resize ms_BaseLeft and its correspondent window
			GetClientRect(hWnd, &DX9MapEditor::ms_TempRect);
			DX9MapEditor::ms_TempRect = GetLeftChildWindowPositionAndSize(DX9MapEditor::ms_TempRect);
			MoveWindow(DX9MapEditor::ms_BaseLeft->GethWnd(), DX9MapEditor::ms_TempRect.left, DX9MapEditor::ms_TempRect.top,
				DX9MapEditor::ms_TempRect.right, DX9MapEditor::ms_TempRect.bottom, TRUE);

			// Resize ms_BaseRight and its correspondent window
			GetClientRect(hWnd, &DX9MapEditor::ms_TempRect);
			DX9MapEditor::ms_TempRect = GetRightChildWindowPositionAndSize(DX9MapEditor::ms_TempRect);
			MoveWindow(DX9MapEditor::ms_BaseRight->GethWnd(), DX9MapEditor::ms_TempRect.left, DX9MapEditor::ms_TempRect.top,
				DX9MapEditor::ms_TempRect.right, DX9MapEditor::ms_TempRect.bottom, TRUE);

			if (DX9MapEditor::ms_BaseLeft->GetDevice() && DX9MapEditor::ms_BaseRight->GetDevice())
			{
				DX9MapEditor::ms_BaseLeft->Resize();
				DX9MapEditor::ms_BaseRight->Resize();
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, Message, wParam, lParam));
}

LRESULT CALLBACK LeftChildWindowProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_SIZE:
		break;
	}
	return(DefWindowProc(hWnd, Message, wParam, lParam));
}

LRESULT CALLBACK RightChildWindowProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_SIZE:
		break;
	}
	return(DefWindowProc(hWnd, Message, wParam, lParam));
}

LRESULT CALLBACK DlgProcNewMap(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	wchar_t tWC[255] = { 0 };
	WSTRING tStr;
	size_t tFind;
	int tMapCols = 0;
	int tMapRows = 0;
	int tTileSize = 0;

	WSTRING Combobox_Ratio[] = { L"x32", L"x64" };
	size_t aa = Combobox_Ratio->size();
	HWND tempHWND;

	switch (iMessage)
	{
	case WM_INITDIALOG:
		tempHWND = (HWND)GetDlgItem(hDlg, IDC_COMBO1);

		for (size_t i = 0; i < Combobox_Ratio->size() - 1; i++)
		{
			// Send a CB_ADDSTRING message to load each item (Add string to combobox.)
			SendMessage(tempHWND, CB_ADDSTRING, (WPARAM)0, (LPARAM)Combobox_Ratio[i].c_str());
		}

		// Send the CB_SETCURSEL message to display an initial item in the selection field
		SendMessage(tempHWND, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

		PostMessage(hDlg, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(hDlg, IDC_EDIT1), TRUE);
		return TRUE;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDC_BUTTON1:
			//if (g_myWND->OpenFileDlg(L"모든 파일\0*.*\0") == TRUE)
			{
				// 타일 이름 얻기
				//g_strTileName = g_myWND->GetDlgFileName().c_str();
				//tFind = g_strTileName.find_last_of('\\');
				//if (tFind)
				//	g_strTileName = g_strTileName.substr(tFind + 1);
				//SetWindowText(GetDlgItem(hDlg, IDC_EDIT4), g_strTileName.c_str());
			}
			break;
		case IDC_COMBO1:

			break;
		case IDOK:
			GetDlgItemText(hDlg, IDC_EDIT4, tWC, 255);
			tFind = wcslen(tWC);

			if (tFind)
			{
				GetDlgItemText(hDlg, IDC_EDIT1, tWC, 255);
				//g_strMapName = tWC;

				tMapCols = GetDlgItemInt(hDlg, IDC_EDIT2, FALSE, FALSE);
				tMapRows = GetDlgItemInt(hDlg, IDC_EDIT3, FALSE, FALSE);

				GetDlgItemText(hDlg, IDC_COMBO1, tWC, 255);
				tStr = tWC;
				tStr = tStr.substr(1);
				tTileSize = _wtoi(tStr.c_str());

				if (tMapCols && tMapRows)
				{
					// 타일 불러오기
					

					// 맵 만들기
					

				}
			}
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		return TRUE;
	}
	return FALSE;
}

auto DX9MapEditor::Create(int Width, int Height)->Error
{
	// Create parent window
	if (ms_BaseParent = new DX9Base)
	{
		if (DX_FAILED(ms_BaseParent->CreateParentWindow(WINDOW_X, WINDOW_Y, Width, Height,
			RGBInt(50, 50, 80), ParentWindowProc, MAKEINTRESOURCE(IDR_MENU1))))
			return Error::BASE_NOT_CREATED;

		// Set main window handle
		m_hWnd = ms_BaseParent->GethWnd();
	}

	// Set data that will be shared in many sub-classes
	// For left child window
	GetCurrentDirectoryW(255, ms_MainWindowData.AppDir);
	ms_MainWindowData.WindowWidth = WINDOW_SEPERATE_X;
	ms_MainWindowData.WindowHeight = Height;
	ms_MainWindowData.WindowHalfWidth = static_cast<float>(ms_MainWindowData.WindowWidth / 2.0f);
	ms_MainWindowData.WindowHalfHeight = static_cast<float>(ms_MainWindowData.WindowHeight / 2.0f);

	// Create left child base and initialize Direct3D9
	if (ms_BaseLeft = new DX9Base)
	{
		// Get main window RECT
		GetClientRect(m_hWnd, &ms_TempRect);
		ms_TempRect = GetLeftChildWindowPositionAndSize(ms_TempRect);
		if (DX_FAILED(ms_BaseLeft->CreateChildWindow(m_hWnd, ms_TempRect.left, ms_TempRect.top, ms_TempRect.right, ms_TempRect.bottom,
			RGBInt(160, 160, 160), LeftChildWindowProc)))
			return Error::BASE_NOT_CREATED;
	}

	// Create image object
	if (m_Image = new DX9Image)
	{
		if (DX_FAILED(m_Image->Create(ms_BaseLeft->GetDevice(), ms_MainWindowData)))
			return Error::IMAGE_NOT_CREATED;
	}

	// Create right child base and initialize Direct3D9
	if (ms_BaseRight = new DX9Base)
	{
		// Get main window RECT
		GetClientRect(m_hWnd, &ms_TempRect);
		ms_TempRect = GetRightChildWindowPositionAndSize(ms_TempRect);
		if (DX_FAILED(ms_BaseRight->CreateChildWindow(m_hWnd, ms_TempRect.left, ms_TempRect.top, ms_TempRect.right, ms_TempRect.bottom,
			RGBInt(200, 200, 200), RightChildWindowProc)))
			return Error::BASE_NOT_CREATED;
	}
	
	// Create map object
	if (m_Map = new DX9Map)
	{
		if (DX_FAILED(m_Map->Create(ms_BaseRight->GetDevice(), ms_MainWindowData)))
			return Error::MAP_NOT_CREATED;
	}

	return Error::OK;
}

auto DX9MapEditor::LoadMap(WSTRING FileName)->Error
{
	if (m_Map)
	{
		m_Map->LoadMap(FileName);
		return Error::OK;
	}
	return Error::OBJECT_NOT_CREATED;
}

void DX9MapEditor::Run()
{
	while (m_MSG.message != WM_QUIT)
	{
		if (PeekMessage(&m_MSG, nullptr, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&m_MSG);
			DispatchMessage(&m_MSG);
		}
		else
		{
			MainLoop();
		}
	}

	/*
		while (GetMessage(&m_MSG, 0, 0, 0))
	{
		if (!TranslateAccelerator(m_hWnd, hAccel, &m_MSG)) {
			TranslateMessage(&m_MSG);
			DispatchMessage(&m_MSG);

			MainLoop();
		}
	}

	return static_cast<int>(m_MSG.wParam);
	*/

	Destroy();
}

void DX9MapEditor::MainLoop()
{
	// Draw left child
	ms_BaseLeft->BeginRender();

	if (m_Image)
	{
		m_Image->Draw();
	}

	ms_BaseLeft->EndRender();

	// Draw right child
	ms_BaseRight->BeginRender();
	
	if (m_Map)
	{
		D3DXVECTOR2 mapOffset = D3DXVECTOR2(0, 0);
		m_Map->SetGlobalPosition(-mapOffset);
		m_Map->Draw();
	}

	ms_BaseRight->EndRender();
}

void DX9MapEditor::Destroy()
{
	m_Map->Destroy();
	m_Image->Destroy();
	ms_BaseLeft->Destroy();
	ms_BaseRight->Destroy();
	ms_BaseParent->Destroy();

	delete m_Map;
	delete m_Image;
	delete ms_BaseLeft;
	delete ms_BaseRight;
	delete ms_BaseParent;

	m_Map = nullptr;
	m_Image = nullptr;
	ms_BaseLeft = nullptr;
	ms_BaseRight = nullptr;
	ms_BaseParent = nullptr;
}