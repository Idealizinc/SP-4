#include "Particle.h"
#include "../../Engine/System/RenderSystem.h"
#include "../../Engine/System/SceneSystem.h"

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

void Particle::Update(const float& dt)
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
			Velocity += Vector3(0,-9.8f) * dt; // For Gravity
			if (!Static)
				SetPosition(GetPosition() + (StoredVelocity + GetVelocity()) * 0.5f * (float)dt);
			SetRotationAngle(Math::RadianToDegree(atan2(PlayerPosition.x - GetPosition().x, PlayerPosition.z - GetPosition().z)));
		}
	}
}

void Particle::Render()
{
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());

	if (Active && Visible && StoredMesh)
	{
		RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
		float TimeRatio = 1;
		if (LifeTime != -1)
		{
			TimeRatio = 1.1f - CurrentTime / LifeTime;
		}
		if (InWorldSpace)
		{
			SceneSystem::Instance().GetCurrentScene().modelStack->PushMatrix();
			SceneSystem::Instance().GetCurrentScene().modelStack->Translate(Position.x, Position.y, Position.z);
			if (LifeTime != -1)
			{
				SceneSystem::Instance().GetCurrentScene().modelStack->Rotate(90, -1, 0, 0);
				SceneSystem::Instance().GetCurrentScene().modelStack->Rotate(TimeRatio * 360, 0, 0, 1);
			}
			else SceneSystem::Instance().GetCurrentScene().modelStack->Rotate(Math::RadianToDegree(atan2(PlayerPosition.x - GetPosition().x, PlayerPosition.z - GetPosition().z)), 0, 1, 0);
			SceneSystem::Instance().GetCurrentScene().modelStack->Scale(TimeRatio * GetDimensions().x, TimeRatio * GetDimensions().y, TimeRatio * GetDimensions().z);
			if ((TimeRatio * GetDimensions()).LengthSquared() > 0.1f)
				Renderer->RenderMesh(*StoredMesh, false);
			SceneSystem::Instance().GetCurrentScene().modelStack->PopMatrix();
		}
		else
		{
			Renderer->SetHUD(true);
			glDisable(GL_CULL_FACE);
			SceneSystem::Instance().GetCurrentScene().modelStack->PushMatrix();
			float TimeRatio = 1.1f - CurrentTime / LifeTime;
			SceneSystem::Instance().GetCurrentScene().modelStack->Rotate(TimeRatio * 360, 0, 1, 0);
			Renderer->RenderMeshIn2D(*StoredMesh, false, Dimensions.x, Dimensions.y, Position.x, Position.y); 
			SceneSystem::Instance().GetCurrentScene().modelStack->PopMatrix();
			glEnable(GL_CULL_FACE); 
			Renderer->SetHUD(false);
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
	SetMass(1);
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
	this->InWorldSpace = true;
}

void Particle::SetLifeTime(const float& LifeTime)
{
	this->LifeTime = LifeTime;
	this->CurrentTime = 0.f;
}