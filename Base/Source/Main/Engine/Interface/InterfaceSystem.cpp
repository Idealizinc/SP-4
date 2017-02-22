#include "InterfaceSystem.h"
#include "../../Engine/System/RenderSystem.h"
#include "../../Engine/System/SceneSystem.h"

void InterfaceSystem::Init()
{
	
}

void InterfaceSystem::Update(const float& dt)
{
	for (auto it : InternalLayerContainer)
	{
		if (it->Active)
			it->Update(dt);
	}
}

void InterfaceSystem::Render()
{
	for (auto it : InternalLayerContainer)
	{
		if (it->Active && it->Visible)
			it->Render();
	}
}

void InterfaceSystem::Exit()
{
	for (auto it : InternalLayerContainer)
	{
		it->Exit();
		delete it;
	}
	InternalLayerContainer.clear();
}

void InterfaceSystem::ResetAllToOriginal()
{
	for (auto it : InternalLayerContainer)
	{
		it->ResetToOriginal();
		if (it->GetContainer().size())
		{
			for (auto it2 : it->GetContainer())
			{
				it2->ResetToOriginal();
			}
		}
	}
}

InterfaceLayer* InterfaceSystem::CreateNewInterfaceLayer(const std::string& LayerID, const Vector3& OriginalPosition, const Vector3& TargetPosition)
{
	InterfaceLayer* NewL = new InterfaceLayer();
	NewL->SetEntityID(LayerID);
	NewL->SetOriginalPosition(OriginalPosition);
	NewL->SetTargetPosition(TargetPosition);
	InternalLayerContainer.push_back(NewL);
	return NewL;
}

