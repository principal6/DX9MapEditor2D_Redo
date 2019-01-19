#include "DX9MapEditor.h"
#include "resource.h"

using namespace DX9ENGINE;

DX9Base* DX9MapEditor::ms_BaseParent;
DX9Base* DX9MapEditor::ms_BaseLeft;
DX9Base* DX9MapEditor::ms_BaseRight;
RECT DX9MapEditor::ms_TempRect;
MapInfo DX9MapEditor::ms_MapInfo;

DX9Image* DX9MapEditor::ms_TileImage;
DX9Image* DX9MapEditor::ms_MoveImage;
DX9Map* DX9MapEditor::ms_Map;

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
		case ID_ACCELERATOR40013: // ACCEL Ctrl + N
		case ID_MAP_NEWMAP:
			// Show new map dialog
			DialogBox(nullptr, MAKEINTRESOURCE(IDD_DIALOG1), DX9MapEditor::ms_BaseParent->GethWnd(), DlgProcNewMap);
			break;
		case ID_ACCELERATOR40014: // ACCEL Ctrl + O
		case ID_MAP_OPENMAP:
			// Show load map dialog
			if (DX9MapEditor::ms_BaseParent->OpenFileDlg(L"JWM File\0*.jwm\0"))
			{
				if (DX9MapEditor::ms_BaseParent->GetDlgFileTitle().size())
				{
					// Load map
					DX9MapEditor::ms_Map->LoadMap(DX9MapEditor::ms_BaseParent->GetDlgFileTitle());

					// Load tile image
					DX9MapEditor::ms_Map->GetMapInfo(&DX9MapEditor::ms_MapInfo);
					DX9MapEditor::ms_TileImage->SetTexture(DX9MapEditor::ms_MapInfo.TileName);
					DX9MapEditor::ms_MoveImage->SetTexture(DX9MapEditor::ms_MapInfo.MoveName);
				}
			}
			break;
		case ID_ACCELERATOR40015: // ACCEL Ctrl + S
		case ID_MAP_SAVEMAP:
			break;
		case ID_ACCELERATOR40008: // ACCEL F2
		case ID_MODE_TILEMODE:
			DX9MapEditor::ms_Map->SetMode(DX9Common::MapMode::TileMode);
			break;
		case ID_ACCELERATOR40010: // ACCEL F3
		case ID_MODE_MOVEMODE:
			DX9MapEditor::ms_Map->SetMode(DX9Common::MapMode::MoveMode);
			break;
		case ID_ACCELERATOR40017: // ACCEL F1
		case ID_HELP_INFO:
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

LRESULT CALLBACK DX9ENGINE::DlgProcNewMap(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	wchar_t tWC[255] {};
	WSTRING tStr;

	WSTRING Combobox_Ratio[] = { L"x32", L"x64" };
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
			if (DX9MapEditor::ms_BaseParent->OpenFileDlg(L"All files\0*.*\0"))
			{
				// Get the name of the tile
				DX9MapEditor::ms_MapInfo.TileName = DX9MapEditor::ms_BaseParent->GetDlgFileTitle();
				SetWindowText(GetDlgItem(hDlg, IDC_EDIT4), DX9MapEditor::ms_MapInfo.TileName.c_str());
			}
			break;
		case IDOK:
			GetDlgItemText(hDlg, IDC_EDIT4, tWC, 255);

			if (wcslen(tWC))
			{
				GetDlgItemText(hDlg, IDC_EDIT1, tWC, 255);
				DX9MapEditor::ms_MapInfo.MapName = tWC;

				DX9MapEditor::ms_MapInfo.MapCols = GetDlgItemInt(hDlg, IDC_EDIT2, FALSE, FALSE);
				DX9MapEditor::ms_MapInfo.MapRows = GetDlgItemInt(hDlg, IDC_EDIT3, FALSE, FALSE);

				GetDlgItemText(hDlg, IDC_COMBO1, tWC, 255);
				tStr = tWC;
				tStr = tStr.substr(1);
				DX9MapEditor::ms_MapInfo.TileSize = _wtoi(tStr.c_str());

				if (DX9MapEditor::ms_MapInfo.MapCols && DX9MapEditor::ms_MapInfo.MapRows)
				{
					// Create a new map
					DX9MapEditor::ms_Map->CreateNewMap(&DX9MapEditor::ms_MapInfo);

					// Load tile image
					DX9MapEditor::ms_Map->GetMapInfo(&DX9MapEditor::ms_MapInfo);
					DX9MapEditor::ms_TileImage->SetTexture(DX9MapEditor::ms_MapInfo.TileName);
					DX9MapEditor::ms_MoveImage->SetTexture(DX9MapEditor::ms_MapInfo.MoveName);
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

	m_hAccel = LoadAccelerators(nullptr, MAKEINTRESOURCE(IDR_ACCELERATOR1)); // accelator load

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

	// Create image objects
	if (ms_TileImage = new DX9Image)
	{
		if (DX_FAILED(ms_TileImage->Create(ms_BaseLeft->GetDevice(), ms_MainWindowData)))
			return Error::IMAGE_NOT_CREATED;
		ms_TileImage->SetSize(0, 0);
	}
	if (ms_MoveImage = new DX9Image)
	{
		if (DX_FAILED(ms_MoveImage->Create(ms_BaseLeft->GetDevice(), ms_MainWindowData)))
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
	if (ms_Map = new DX9Map)
	{
		if (DX_FAILED(ms_Map->Create(ms_BaseRight->GetDevice(), ms_MainWindowData)))
			return Error::MAP_NOT_CREATED;
	}

	return Error::OK;
}

void DX9MapEditor::Run()
{
	/*
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
	*/

	while (GetMessage(&m_MSG, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(m_hWnd, m_hAccel, &m_MSG))
		{
			TranslateMessage(&m_MSG);
			DispatchMessage(&m_MSG);

			MainLoop();
		}
	}

	//return static_cast<int>(m_MSG.wParam);

	Destroy();
}

void DX9MapEditor::MainLoop()
{
	// Draw left child
	ms_BaseLeft->BeginRender();

	if (ms_Map)
	{
		if (ms_Map->GetMode() == MapMode::TileMode)
		{
			if (ms_TileImage)
			{
				ms_TileImage->Draw();
			}
		}
		else
		{
			if (ms_MoveImage)
			{
				ms_MoveImage->Draw();
			}
		}
	}

	ms_BaseLeft->EndRender();

	// Draw right child
	ms_BaseRight->BeginRender();
	
	if (ms_Map)
	{
		D3DXVECTOR2 mapOffset = D3DXVECTOR2(0, 0);
		ms_Map->SetGlobalPosition(-mapOffset);
		ms_Map->Draw();
	}

	ms_BaseRight->EndRender();
}

void DX9MapEditor::Destroy()
{
	DX_DESTROY(ms_Map);
	DX_DESTROY(ms_TileImage);
	DX_DESTROY(ms_MoveImage);
	DX_DESTROY(ms_BaseLeft);
	DX_DESTROY(ms_BaseRight);
	DX_DESTROY(ms_BaseParent);
}