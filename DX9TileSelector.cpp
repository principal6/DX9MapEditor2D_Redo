#include "DX9TileSelector.h"
#include "Core/DX9Base.h"

using namespace DX9ENGINE;

// Static member variable
const wchar_t* DX9TileSelector::SEL_FN = L"tilesel.png";

auto DX9TileSelector::Create(DX9Base* pBase, WSTRING BaseDir)->Error
{
	// Create image object
	if (m_Selector = MAKE_UNIQUE(DX9Image)())
	{
		if (DX_FAILED(m_Selector->Create(pBase, BaseDir)))
			return Error::IMAGE_NOT_CREATED;
	}

	m_Selector->SetTexture(SEL_FN);
	m_Selector->SetAlpha(SEL_ALPHA);

	return Error::OK;
}

void DX9TileSelector::Destroy()
{

}

void DX9TileSelector::Draw()
{
	if (m_Selector)
		m_Selector->Draw();
}

void DX9TileSelector::SetSize(int TileSize)
{
	m_Selector->SetSize(TileSize, TileSize);
}