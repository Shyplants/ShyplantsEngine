#include "ClientPCH/ClientPCH.h"
#include "Actors/DemoUIActor.h"

#include "Engine/Core/Component/TextRendererComponent.h"
#include "Engine/Core/Component/SceneComponent.h"

DemoUIActor::DemoUIActor()
{

}

DemoUIActor::~DemoUIActor()
{
}

void DemoUIActor::OnSpawned()
{
	m_text = AddComponent<TextRendererComponent>(this);
	m_text->SetText(L"Shyplants Engine!");
	m_text->SetScale(1.0f);

	GetRootComponent()->SetLocalPosition({ 20.0f, 20.0f, 0.0f });
}

void DemoUIActor::SetText(std::wstring& text)
{
	if (m_text)
		m_text->SetText(text);
}