#include "Engine/PCH/EnginePCH.h"

#include "Engine/System/InputManager.h"

InputManager* InputManager::Instance = nullptr;

void InputManager::Create()
{
	if (!Instance)
	{
		Instance = new InputManager();
	}
}

void InputManager::Destroy()
{
	if (Instance)
	{
		delete Instance;
		Instance = nullptr;
	}
}

bool InputManager::IsValid()
{
	return Instance != nullptr;
}

InputManager& InputManager::Get()
{
	// assert(Instance && "InputManager::Create() was not called");
	return *Instance;
}

void InputManager::ProcessMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	uint32 vk = static_cast<uint32>(wParam);

	if (vk >= 256)
		return;

	switch (msg)
	{
		

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		bool wasDown = (lParam & 0x40000000) != 0;

		// auto-repeat ¹æÁö
		if (!wasDown)
			m_curKeys[vk] = true;
		
		break;
	}

	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		m_curKeys[wParam] = false;
		break;
	}

	case WM_KILLFOCUS:
	{
		m_curKeys.fill(false);
		break;
	}

	default:
		break;
	}
}

void InputManager::Update()
{
	m_prevKeys = m_curKeys;
}

bool InputManager::IsKeyDown(KeyCode key) const
{
	return m_curKeys[static_cast<uint8>(key)];
}

bool InputManager::IsKeyPressed(KeyCode key) const
{
	uint8 k = static_cast<uint8>(key);

	return (m_curKeys[k] && !m_prevKeys[k]);
}

bool InputManager::IsKeyReleased(KeyCode key) const
{
	uint8 k = static_cast<uint8>(key);

	return (!m_curKeys[k] && m_prevKeys[k]);
}
