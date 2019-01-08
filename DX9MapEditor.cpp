#include "DX9MapEditor.h"
#include "resource.h"

DX9Base* DX9MapEditor::ms_BaseParent;
DX9Base* DX9MapEditor::ms_BaseLeft;
DX9Base* DX9MapEditor::ms_BaseRight;
RECT DX9MapEditor::ms_TempRect;

RECT GetLeftChildWindowPositionAndSize(RECT Rect)
{
	RECT Result = {0, 0,
		DX9MapEditor::WINDOW_SEPERATE_X - DX9MapEditor::WINDOW_VSCROLL_SIZE,
		Rect.bottom - DX9MapEditor::WINDOW_HSCROLL_SIZE };
	return Result;
}

RECT GetRightChildWindowPositionAndSize(RECT Rect)
{
	RECT Result = { DX9MapEditor::WINDOW_SEPERATE_X + DX9MapEditor::WINDOW_SEPERATE_INTERVAL, 0,
		Rect.right - DX9MapEditor::WINDOW_SEPERATE_X - DX9MapEditor::WINDOW_VSCROLL_SIZE - DX9MapEditor::WINDOW_PADDING_X,
		Rect.bottom - DX9MapEditor::WINDOW_HSCROLL_SIZE };
	return Result;
}

LRESULT CALLBACK ParentWindowProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_SIZE:
		if (DX9MapEditor::ms_BaseLeft && DX9MapEditor::ms_BaseRight)
		{
			GetClientRect(hWnd, &DX9MapEditor::ms_TempRect);
			DX9MapEditor::ms_TempRect = GetLeftChildWindowPositionAndSize(DX9MapEditor::ms_TempRect);
			MoveWindow(DX9MapEditor::ms_BaseLeft->GethWnd(), DX9MapEditor::ms_TempRect.left, DX9MapEditor::ms_TempRect.top,
				DX9MapEditor::ms_TempRect.right, DX9MapEditor::ms_TempRect.bottom, TRUE);

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

DX9Common::ReturnValue DX9MapEditor::Create(int Width, int Height)
{
	// Create parent window
	if (ms_BaseParent = new DX9Base)
	{
		if (DX_FAILED(ms_BaseParent->CreateParentWindow(WINDOW_X, WINDOW_Y, Width, Height,
			RGBInt(50, 50, 80), ParentWindowProc, MAKEINTRESOURCE(IDR_MENU1))))
			return ReturnValue::BASE_NOT_CREATED;

		// Set main window handle
		m_hWnd = ms_BaseParent->GethWnd();
	}

	// Set data that will be shared in many sub-classes
	// For left child window
	GetCurrentDirectoryW(255, m_WindowData.AppDir);
	m_WindowData.WindowWidth = WINDOW_SEPERATE_X;
	m_WindowData.WindowHeight = Height;
	m_WindowData.WindowHalfWidth = static_cast<float>(Width / 2.0f);
	m_WindowData.WindowHalfHeight = static_cast<float>(Height / 2.0f);

	// Create left child base and initialize Direct3D9
	if (ms_BaseLeft = new DX9Base)
	{
		// Get main window RECT
		GetClientRect(m_hWnd, &ms_TempRect);
		ms_TempRect = GetLeftChildWindowPositionAndSize(ms_TempRect);
		if (DX_FAILED(ms_BaseLeft->CreateChildWindow(m_hWnd, ms_TempRect.left, ms_TempRect.top, ms_TempRect.right, ms_TempRect.bottom,
			RGBInt(160, 160, 160), LeftChildWindowProc)))
			return ReturnValue::BASE_NOT_CREATED;
	}

	// Create image object
	if (m_Image = new DX9Image)
	{
		if (DX_FAILED(m_Image->Create(ms_BaseLeft->GetDevice(), m_WindowData)))
			return ReturnValue::IMAGE_NOT_CREATED;
	}

	m_WindowData.WindowWidth = Width - WINDOW_SEPERATE_X - WINDOW_SEPERATE_INTERVAL;
	m_WindowData.WindowHalfWidth = static_cast<float>(Width / 2.0f);

	// Create right child base and initialize Direct3D9
	if (ms_BaseRight = new DX9Base)
	{
		// Get main window RECT
		GetClientRect(m_hWnd, &ms_TempRect);
		ms_TempRect = GetRightChildWindowPositionAndSize(ms_TempRect);
		if (DX_FAILED(ms_BaseRight->CreateChildWindow(m_hWnd, ms_TempRect.left, ms_TempRect.top, ms_TempRect.right, ms_TempRect.bottom,
			RGBInt(200, 200, 200), RightChildWindowProc)))
			return ReturnValue::BASE_NOT_CREATED;
	}
	
	// Create map object
	if (m_Map = new DX9Map)
	{
		if (DX_FAILED(m_Map->Create(ms_BaseRight->GetDevice(), m_WindowData)))
			return ReturnValue::MAP_NOT_CREATED;
	}

	return ReturnValue::OK;
}

DX9Common::ReturnValue DX9MapEditor::LoadMap(WSTRING FileName)
{
	if (m_Map)
	{
		m_Map->LoadMap(FileName);
		return ReturnValue::OK;
	}
	return ReturnValue::OBJECT_NOT_CREATED;
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