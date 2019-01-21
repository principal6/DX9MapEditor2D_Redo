#pragma once

#include "Core/DX9Image.h"

namespace DX9ENGINE
{
	// ***
	// *** Forward declaration ***
	class DX9Window;
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

		auto Create(DX9Window* pBase, WSTRING BaseDir)->EError;
		void Destroy();

		void Draw();

		void SetSize(float TileSize);
	};
};