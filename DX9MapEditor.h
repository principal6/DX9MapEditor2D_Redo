#pragma once

#include "resource.h"
#include "Core/DX9Base.h"
#include "DX9Map.h"

namespace DX9ENGINE
{
	class DX9MapEditor final : public DX9Common
	{
	private:
		static const int WINDOW_X = 50;
		static const int WINDOW_Y = 50;
		static const int WINDOW_SEPERATE_X = 250;
		static const int WINDOW_SEPERATE_INTERVAL = 10;
		static const int WINDOW_VSCROLL_SIZE = 15;
		static const int WINDOW_HSCROLL_SIZE = 15;
		static const int WINDOW_PADDING_X = 10;

		static DX9Base* ms_BaseParent;
		static DX9Base* ms_BaseLeft;
		static DX9Base* ms_BaseRight;
		static RECT ms_TempRect;
		static MapInfo ms_MapInfo;
		static DX9Image* ms_TileImage;
		static DX9Image* ms_MoveImage;
		static DX9Map* ms_Map;

		bool m_Keys[NUM_KEYS];
		MSG m_MSG;

	private:
		void DX9MapEditor::Destroy() override;

		friend RECT GetLeftChildWindowPositionAndSize(RECT Rect);
		friend RECT GetRightChildWindowPositionAndSize(RECT Rect);
		friend LRESULT CALLBACK ParentWindowProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
		friend LRESULT CALLBACK DlgProcNewMap(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

	public:
		DX9MapEditor() {};
		~DX9MapEditor() {};

		auto DX9MapEditor::Create(int Width, int Height)->Error;

		void DX9MapEditor::Run();

		void DX9MapEditor::MainLoop();
	};
};