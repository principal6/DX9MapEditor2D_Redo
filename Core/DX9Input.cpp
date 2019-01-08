#include "DX9Input.h"

DX9Input::DX9Input()
{
	m_DI8 = nullptr;
	m_DIDevKeyboard = nullptr;
	m_DIDevMouse = nullptr;

	memset(&m_BufferKeyState, 0, sizeof(m_BufferKeyState));
	memset(&m_KeyDown, false, sizeof(m_KeyDown));
	memset(&m_KeyUp, false, sizeof(m_KeyUp));

	m_MouseX = 0;
	m_MouseY = 0;
}

DX9Common::ReturnValue DX9Input::Create(HWND hWnd)
{
	m_hWnd = hWnd;

	memset(m_MouseBtnDown, false, sizeof(m_MouseBtnDown));
	memset(m_MouseBtnUp, false, sizeof(m_MouseBtnUp));
	memset(m_MouseBtnIdle, true, sizeof(m_MouseBtnIdle));

	if(FAILED(DirectInput8Create(DX9Common::ms_hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void **) &m_DI8, nullptr)))
		return ReturnValue::OBJECT_NOT_CREATED;

	if (FAILED(CreateMouseDevice(DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return ReturnValue::OBJECT_NOT_CREATED;

	if (FAILED(CreateKeyboardDevice(DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return ReturnValue::OBJECT_NOT_CREATED;

	return ReturnValue::OK;
}

bool DX9Input::CreateMouseDevice(DWORD dwFlags)
{
	if(FAILED(m_DI8->CreateDevice(GUID_SysMouse, &m_DIDevMouse, nullptr)))
		return false;

	if(FAILED(m_DIDevMouse->SetDataFormat(&c_dfDIMouse2)))
		return false;

	if(FAILED(m_DIDevMouse->SetCooperativeLevel(m_hWnd, dwFlags)))
		return false;

	if(FAILED(m_DIDevMouse->Acquire()))
		return false;

	return true;
}

bool DX9Input::CreateKeyboardDevice(DWORD dwFlags)
{
	if(FAILED(m_DI8->CreateDevice(GUID_SysKeyboard, &m_DIDevKeyboard, nullptr)))
		return false;

	if(FAILED(m_DIDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return false;

	if(FAILED(m_DIDevKeyboard->SetCooperativeLevel(m_hWnd, dwFlags)))
		return false;

	if(FAILED(m_DIDevKeyboard->Acquire()))
		return false;
	
	return true;
}

void DX9Input::Destroy()
{
	if (m_DIDevMouse)
	{
		m_DIDevMouse->Unacquire();
		m_DIDevMouse->Release();
		m_DIDevMouse = nullptr;
	}
	if (m_DIDevKeyboard)
	{
		m_DIDevKeyboard->Unacquire();
		m_DIDevKeyboard->Release();
		m_DIDevKeyboard = nullptr;
	}
	if (m_DI8)
	{
		m_DI8->Release();
		m_DI8 = nullptr;
	}
}

bool DX9Input::OnKeyDown(DWORD DIK_KeyCode)
{
	HRESULT hr;
	if(FAILED(hr = m_DIDevKeyboard->GetDeviceState(sizeof(m_BufferKeyState),
		(LPVOID)&m_BufferKeyState)))
	{
		if(hr == DIERR_INPUTLOST)
			m_DIDevKeyboard->Acquire();
	}

	if (m_BufferKeyState[DIK_KeyCode] & 0x80)	// Ű�� ������ true�� ��ȯ
	{
		m_KeyUp[DIK_KeyCode] = false;
		m_KeyDown[DIK_KeyCode] = true;
		return true;
	}
	else
	{
		if (m_KeyDown[DIK_KeyCode] == true) // ������ Ű�� �����־��ٸ�?
		{
			m_KeyUp[DIK_KeyCode] = true;
		}
		else
		{
			m_KeyUp[DIK_KeyCode] = false;
		}
		
		m_KeyDown[DIK_KeyCode] = false;
		return false;
	}
}

bool DX9Input::OnKeyUp(DWORD DIK_KeyCode)
{
	return m_KeyUp[DIK_KeyCode];
}

DIMOUSESTATE2 DX9Input::OnMouseMove()
{
	DIMOUSESTATE2 Result;
	HRESULT hr;
	
	memset(&m_MouseState, 0, sizeof(m_MouseState));
	if(FAILED(hr = m_DIDevMouse->GetDeviceState(sizeof(m_MouseState),(LPVOID) &m_MouseState)))
	{
		if(hr == DIERR_INPUTLOST)
			m_DIDevMouse->Acquire();
	}

	Result.lX = m_MouseState.lX;
	Result.lY = m_MouseState.lY;
	Result.lZ = m_MouseState.lZ;

	return Result;
}

bool DX9Input::CheckMouseButton(int button)
{
	if(m_MouseState.rgbButtons[button] & 0x80) // ��ư�� ����
	{
		if ( (m_MouseBtnDown[button] == false) && (m_MouseBtnIdle[button] == true) )
		{
			m_MouseBtnUp[button] = false;
			m_MouseBtnDown[button] = true;
		}
	}
	else if(!m_MouseState.rgbButtons[button]) // ��ư�� ������ ����
	{
		if ( (m_MouseBtnDown[button] == false) && (m_MouseBtnIdle[button] == false) )
		{
			m_MouseBtnUp[button] = true;
			m_MouseBtnIdle[button] = true;
			m_MouseBtnDown[button] = false;
		}
		else if ( (m_MouseBtnDown[button] == true) && (m_MouseBtnIdle[button] == true) )
		{
			m_MouseBtnUp[button] = true;
			m_MouseBtnDown[button] = false;
		}
	}

	return true;
}

bool DX9Input::OnMouseButtonDown(int button)
{
	CheckMouseButton(button);

	if (m_MouseBtnDown[button] == true)
	{
		m_MouseBtnDown[button] = false;
		m_MouseBtnIdle[button] = false;
		return true;
	}

	return false;
}

bool DX9Input::OnMouseButtonUp(int button)
{
	CheckMouseButton(button);

	if (m_MouseBtnUp[button] == true)
	{
		m_MouseBtnUp[button] = false;
		m_MouseBtnIdle[button] = true;
		return true;
	}

	return false;
}

bool DX9Input::GetKeyState(DWORD DIK_KeyCode) const
{
	if (m_BufferKeyState[DIK_KeyCode] & 0x80)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void DX9Input::GetAllKeyState(bool* Keys)
{
	// Check all DIK states
	HRESULT hr;
	if (FAILED(hr = m_DIDevKeyboard->GetDeviceState(sizeof(m_BufferKeyState), (LPVOID)&m_BufferKeyState)))
	{
		if (hr == DIERR_INPUTLOST)
		{
			m_DIDevKeyboard->Acquire();
		}
	}

	for (int i = 1; i <= NUM_KEYS; i++)
	{
		Keys[i] = false;

		if (m_BufferKeyState[i] & 0x80)
			Keys[i] = true;
	}
}

bool DX9Input::GetMouseButtonDown(int button)
{
	CheckMouseButton(button);
	return m_MouseBtnDown[button];
}