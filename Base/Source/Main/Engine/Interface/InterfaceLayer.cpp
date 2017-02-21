#include "InterfaceLayer.h"
#include "../../Engine/System/RenderSystem.h"
#include "../../Engine/System/SceneSystem.h"

void InterfaceLayer::Init()
{
	Position = OriginalPosition = TargetPosition;
}

void InterfaceLayer::Update(const float& dt)
{
	if (Active)
	{
		Vector3 DirVec = TargetPosition - Position;
		if (DirVec.LengthSquared() > 1.f)
		{
			Position += DirVec * dt;
		}

		for (auto it : InternalElementContainer)
		{
			if (it->Active)
				it->Update(dt);
		}
	}
}

void InterfaceLayer::Render()
{
	if (Active && Visible)
	{
		for (auto it : InternalElementContainer)
		{
			if (it->Active && it->Visible)
				it->Render(Position);
		}
	}
}

void InterfaceLayer::Exit()
{
	for (auto it : InternalElementContainer)
	{
		it->Exit();
		delete it;
	}
	InternalElementContainer.clear();
}

void InterfaceLayer::SetTargetPosition(const Vector3& Position)
{
	TargetPosition = Position;
}

void InterfaceLayer::SetOriginalPosition(const Vector3& Position)
{
	OriginalPosition = Position;
	SetPosition(Position);
}

void InterfaceLayer::ResetToOriginal()
{
	Position = OriginalPosition;
}

void InterfaceLayer::SwapOriginalWithTarget()
{
	std::swap(TargetPosition, OriginalPosition);
}

InterfaceElement* InterfaceLayer::CreateNewInterfaceElement(const std::string& ElementID, const std::string& MeshID, const Vector3& OriginalPosition, const Vector3& Dimensions)
{
	InterfaceElement* NewE = new InterfaceElement();
	NewE->SetEntityID(ElementID);
	NewE->SetOriginalPosition(OriginalPosition);
	NewE->SetMesh(MeshID);
	NewE->SetDimensions(Dimensions);
	InternalElementContainer.push_back(NewE);
	return NewE;
}

std::vector<InterfaceElement*>& InterfaceLayer::GetContainer()
{
	return InternalElementContainer;
}