#include "Particle.h"
#include "../../../Engine/System/RenderSystem.h"
#include "../../../Engine/System/SceneSystem.h"

Particle::Particle(const std::string& MeshName, const float& Mass, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const Vector3& PlayerPosition, const float& LifeTime)
{
	StoredMesh = nullptr;
	SetEntityID(MeshName);
	SetMesh(MeshName);
	SetMass(Mass);
	SetPosition(Position);
	DefaultDimensions = Dimensions;
	SetDimensions(DefaultDimensions);
	SetVelocity(Velocity);
	this->PlayerPosition = PlayerPosition;
	this->LifeTime = LifeTime;
	this->CurrentTime = 0;
	this->Active = true;
	this->Static = false;
	this->Visible = true;
}

void Particle::SetMeshBasedOnID()
{
	RenderSystem* Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
	std::map<std::string, Mesh*>::iterator it = Renderer->MeshList.find(GetEntityID());
	if (it != Renderer->MeshList.end())
	{
		StoredMesh = it->second;
	}
}

void Particle::Update(double dt)
{
	if (Active) // Still can update if invisible
	{
		CurrentTime += (float)dt;
		if (!CheckLife())
		{
			Active = false;
		}
		else
		{
			if (LifeTime != -1)
				SetDimensions(DefaultDimensions * ((LifeTime - CurrentTime + 0.01f) / LifeTime));
			Vector3 StoredVelocity = GetVelocity();
			//Velocity += m_gravity * dt; // For Gravity
			if (!Static)
				SetPosition(GetPosition() + (StoredVelocity + GetVelocity()) * 0.5f * (float)dt);
			SetRotationAngle(Math::RadianToDegree(atan2(-GetVelocity().x, GetVelocity().y)));
		}
	}
}

bool Particle::CheckLife()
{
	// Still active if Liftime exceeds.
	if (LifeTime == -1) return true;
	else return CurrentTime < LifeTime;
}

bool Particle::operator<(Particle& rhs){
	float V1 = (this->GetPosition() - this->PlayerPosition).LengthSquared();
	float V2 = (rhs.GetPosition() - this->PlayerPosition).LengthSquared();
	return (V1 > V2);
}

void Particle::Reset()
{
	SetMesh("");
	SetMass(0);
	SetPosition(Vector3());
	DefaultDimensions = Vector3(1, 1, 1);
	SetDimensions(DefaultDimensions);
	SetVelocity(Vector3());
	this->PlayerPosition = Vector3();
	this->LifeTime = -1;
	this->CurrentTime = 0;
	this->Active = true;
	this->Static = false;
	this->Visible = true;
}