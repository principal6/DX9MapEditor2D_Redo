#include "DX9TileSelector.h"
#include "Core/DX9Window.h"
#include "Game/DX9Map.h"

using namespace DX9ENGINE;

// Static member variable
const wchar_t* DX9MapTileSelector::SEL_FN = L"tilesel.png";

auto DX9MapTileSelector::ConvertPositionToCellXY(POINT Position)->POINT
{
	POINT Result{ 0, 0 };
	
	if (m_pMapInfo && m_pMapInfo->TileSize)
	{
		Result.x = (static_cast<int>(Position.x) / m_pMapInfo->TileSize);
		Result.y = (static_cast<int>(Position.y) / m_pMapInfo->TileSize);
	}

	return Result;
}

DX9MapTileSelector::DX9MapTileSelector()
{
	m_pMapInfo = nullptr;

	m_SelectionStart = { 0, 0 };
	m_SelectionSize = { 0, 0 };
}

auto DX9MapTileSelector::Create(DX9Window* pTileSelectorWindow, DX9Window* pMapWindow, WSTRING BaseDir)->EError
{
	// Create tile selector image
	if (m_TileSelector = MAKE_UNIQUE(DX9Image)())
	{
		if (DX_FAILED(m_TileSelector->Create(pTileSelectorWindow, BaseDir)))
			return EError::IMAGE_NOT_CREATED;
	}
	m_TileSelector->SetTexture(SEL_FN);
	m_TileSelector->SetAlpha(SEL_ALPHA);

	// Create map selector image
	// Map selector's texture will be set when SetMapInfo() is called
	if (m_MapSelector = MAKE_UNIQUE(DX9Image)())
	{
		if (DX_FAILED(m_MapSelector->Create(pMapWindow, BaseDir)))
			return EError::IMAGE_NOT_CREATED;
	}
	m_MapSelector->SetAlpha(SEL_ALPHA);

	return EError::OK;
}

void DX9MapTileSelector::Destroy()
{

}

void DX9MapTileSelector::UpdateTileSelector(SMouseData* MouseData)
{
	if (m_pMapInfo)
	{
		if (m_pMapInfo->TileSize)
		{
			POINT PositionInCells = ConvertPositionToCellXY(MouseData->MousePosition);
			POINT DownPositionInCells = ConvertPositionToCellXY(MouseData->MouseDownPosition);

			m_SelectionSize.x = abs(DownPositionInCells.x - PositionInCells.x);
			m_SelectionSize.y = abs(DownPositionInCells.y - PositionInCells.y);

			m_SelectionStart = DownPositionInCells;
			if ((DownPositionInCells.x - PositionInCells.x) > 0)
			{
				// X position flip
				m_SelectionStart.x = DownPositionInCells.x - m_SelectionSize.x;
			}
			if ((DownPositionInCells.y - PositionInCells.y) > 0)
			{
				// Y position flip
				m_SelectionStart.y = DownPositionInCells.y - m_SelectionSize.y;
			}

			D3DXVECTOR2 NewPosition;
			NewPosition.x = static_cast<float>(m_SelectionStart.x * m_pMapInfo->TileSize);
			NewPosition.y = static_cast<float>(m_SelectionStart.y * m_pMapInfo->TileSize);

			D3DXVECTOR2 NewSize{ 0, 0 };
			NewSize.x = m_SelectionSize.x + 1.0f;
			NewSize.y = m_SelectionSize.y + 1.0f;
			NewSize *= static_cast<float>(m_pMapInfo->TileSize);

			m_TileSelector->SetPosition(NewPosition);
			m_TileSelector->SetSize(NewSize);

			m_MapSelector->SetSize(NewSize);
			m_MapSelector->SetAtlasUV(NewPosition, NewSize);
		}
	}
}

void DX9MapTileSelector::UpdateMapSelector(SMouseData* MouseData)
{
	if (m_pMapInfo)
	{
		POINT PositionInCells = ConvertPositionToCellXY(MouseData->MousePosition);

		D3DXVECTOR2 NewPosition;
		NewPosition.x = static_cast<float>(PositionInCells.x * m_pMapInfo->TileSize);
		NewPosition.y = static_cast<float>(PositionInCells.y * m_pMapInfo->TileSize);

		m_MapSelector->SetPosition(NewPosition);
	}
}

void DX9MapTileSelector::Draw()
{
	if (m_TileSelector)
		m_TileSelector->Draw();

	if (m_MapSelector)
		m_MapSelector->Draw();
}

auto DX9MapTileSelector::SetMapInfo(SMapInfo* pInfo)->EError
{
	if (nullptr == (m_pMapInfo = pInfo))
		return EError::NULLPTR_MAP_INFO;

	m_TileSelector->SetSize(D3DXVECTOR2(static_cast<float>(m_pMapInfo->TileSize), static_cast<float>(m_pMapInfo->TileSize)));
	m_MapSelector->SetTexture(m_pMapInfo->TileName);
	
	InitializeSelectorPositionAndSize();

	return EError::OK;
}

void DX9MapTileSelector::InitializeSelectorPositionAndSize()
{
	if (m_pMapInfo)
	{
		m_TileSelector->SetPosition(D3DXVECTOR2(0, 0));

		m_MapSelector->SetSize(m_TileSelector->GetSize());
		m_MapSelector->SetAtlasUV(D3DXVECTOR2(0, 0), m_TileSelector->GetSize());
	}
}