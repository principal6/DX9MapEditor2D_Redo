#pragma once

#include "Core/DX9Image.h"

namespace DX9ENGINE
{
	// ***
	// *** Forward declaration ***
	class DX9Base;
	// ***

	class DX9TileSelector final
	{
	private:
		static const int SEL_ALPHA = 100;
		static const wchar_t* SEL_FN;

		UNIQUE_PTR<DX9Image> m_Selector;

	public:
		DX9TileSelector() {};
		~DX9TileSelector() {};

		auto Create(DX9Base* pBase, WSTRING BaseDir)->Error;
		void Destroy();

		void Draw();

		void SetSize(int TileSize);
	};
};