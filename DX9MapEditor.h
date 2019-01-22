#pragma once

#include "resource.h"
#include "Core/DX9Window.h"
#include "Game/DX9Map.h"
#include "DX9TileSelector.h"

namespace DX9ENGINE
{
	const wchar_t MAP_EDITOR_NAME[]{ L"JW EDITOR [DirectX9 2D Game Map Editor] " };

	class DX9MapEditor final
	{
	private:
		static const int WINDOW_X = 50;
		static const int WINDOW_Y = 50;
		static const int WINDOW_SEPERATE_X = 250;
		static const int WINDOW_SEPERATE_INTERVAL = 10;
		static const int WINDOW_VSCROLL_SIZE = 15;
		static const int WINDOW_HSCROLL_SIZE = 15;
		static const int WINDOW_PADDING_X = 10;

		static DX9Window* ms_WindowParent;
		static DX9Window* ms_WindowLeft;
		static DX9Window* ms_WindowRight;
		static RECT ms_TempRect;
		static SMapInfo ms_MapInfo;

		// For left child window
		static UNIQUE_PTR<DX9Image> ms_TileImage;
		static UNIQUE_PTR<DX9Image> ms_MoveImage;
		
		// For right child window
		static UNIQUE_PTR<DX9Image> ms_MapBG;
		static DX9Map* ms_Map;

		// For both child windows
		static UNIQUE_PTR<DX9MapTileSelector> ms_MapTileSelector;

		wchar_t m_BaseDir[MAX_FILE_LEN];
		bool m_Keys[NUM_KEYS];
		HWND m_hWndMain;
		MSG m_MSG;
		HACCEL m_hAccel;

	private:
		void DX9MapEditor::Destroy();

		static void LoadTileWindowImages();
		static void SetMapEditorCaption();

		friend auto GetLeftChildPositionAndSizeFromParent(RECT Rect)->RECT;
		friend auto GetRightChildPositionAndSizeFromParent(RECT Rect)->RECT;
		friend LRESULT CALLBACK ParentWindowProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
		friend LRESULT CALLBACK LeftChildWindowProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
		friend LRESULT CALLBACK RightChildWindowProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
		friend LRESULT CALLBACK DlgProcNewMap(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

	public:
		DX9MapEditor() {};
		~DX9MapEditor() {};

		auto DX9MapEditor::Create(int Width, int Height)->EError;

		void DX9MapEditor::Run();

		void DX9MapEditor::MainLoop();
	};
};