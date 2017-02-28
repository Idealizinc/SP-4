#include "GameObject.h"
#include "../../Engine/System/SceneSystem.h"
#include "../../Engine/System/RenderSystem.h"

GameObject::GameObject()
{
	Bounds = nullptr;
	SetMesh(nullptr);
}

GameObject::GameObject(const GameObject& rhs)
{
	Bounds = nullptr;
	*this = rhs;
}

GameObject::~GameObject() 
{
	Exit();
}

void GameObject::Exit()
{
	if (Bounds)
	{
		delete Bounds;
		Bounds = nullptr;
	}
}

void GameObject::SetBounds()
{
	if (Bounds == nullptr)
		Bounds = new Boundary();
	Bounds->SetPosition(this->GetPosition());
	Bounds->SetDimensions(this->GetDimensions());
	Bounds->SetRotationAngle(this->GetRotationAngle());
	Bounds->ResetValues();
}

Boundary* GameObject::GetBoundary()const
{
	return Bounds;
}

GameObject& GameObject::operator= (const GameObject &object)
{
	std::string S = object.GetEntityID();
	SetEntityID(S);
    SetMesh(object.GetMesh());
	SetPosition(object.GetPosition());
	SetDimensions(object.GetDimensions());
	SetVelocity(object.GetVelocity());
	SetRotationAngle(object.GetRotationAngle());
	SetRotationAxis(object.GetRotationAxis());
	SetMass(object.GetMass());
	SetBounds();
	return *this;
}

void GameObject::settingNewBounds(Boundary &theNewBounds)
{
	if (Bounds)
	{
		delete Bounds;
        Bounds = nullptr;
	}
	Bounds = &theNewBounds;
    Bounds->SetPosition(this->GetPosition());
    Bounds->SetDimensions(this->GetDimensions());
    Bounds->SetRotationAngle(this->GetRotationAngle());
    Bounds->ResetValues();
}