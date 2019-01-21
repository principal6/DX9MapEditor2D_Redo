#include "DX9TileSelector.h"
#include "Core/DX9Window.h"

using namespace DX9ENGINE;

// Static member variable
const wchar_t* DX9TileSelector::SEL_FN = L"tilesel.png";

auto DX9TileSelector::Create(DX9Window* pBase, WSTRING BaseDir)->EError
{
	// Create image object
	if (m_Selector = MAKE_UNIQUE(DX9Image)())
	{
		if (DX_FAILED(m_Selector->Create(pBase, BaseDir)))
			return EError::IMAGE_NOT_CREATED;
	}

	m_Selector->SetTexture(SEL_FN);
	m_Selector->SetAlpha(SEL_ALPHA);

	return EError::OK;
}

void DX9TileSelector::Destroy()
{

}

void DX9TileSelector::Draw()
{
	if (m_Selector)
		m_Selector->Draw();
}

void DX9TileSelector::SetSize(float TileSize)
{
	m_Selector->SetSize(D3DXVECTOR2(TileSize, TileSize));
}