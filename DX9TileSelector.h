#pragma once

#include "Core/DX9Image.h"

namespace DX9ENGINE
{
	// ***
	// *** Forward declaration ***
	class DX9Window;
	struct SMouseData;
	struct SMapInfo;
	// ***

	class DX9MapTileSelector final
	{
	public:
		DX9MapTileSelector();
		~DX9MapTileSelector() {};

		auto Create(DX9Window* pTileSelectorWindow, DX9Window* pMapWindow, WSTRING BaseDir)->EError;
		void Destroy();

		void UpdateTileSelector(SMouseData* MouseData);
		void UpdateMapSelector(SMouseData* MouseData);
		void Draw();

		auto SetMapInfo(SMapInfo* pInfo)->EError;

	private:
		auto DX9MapTileSelector::ConvertPositionToCellXY(POINT Position)->POINT;
		void DX9MapTileSelector::InitializeSelectorPositionAndSize();

	private:
		static const int SEL_ALPHA = 160;
		static const wchar_t* SEL_FN;

		SMapInfo* m_pMapInfo;
		POINT m_SelectionStart;
		POINT m_SelectionSize;
		UNIQUE_PTR<DX9Image> m_TileSelector;
		UNIQUE_PTR<DX9Image> m_MapSelector;
	};
};