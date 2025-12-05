#pragma once

#include "Engine/Core/World/UIActor.h"
#include <string>

class TextRendererComponent;

class DemoUIActor : public UIActor
{
public:
	DemoUIActor();
	~DemoUIActor() override;

	void OnSpawned() override;

public:
	void SetText(std::wstring& text);

private:
	TextRendererComponent* m_text = nullptr;
};