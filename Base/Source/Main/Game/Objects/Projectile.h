/*
Copyright (c) Lim Rui An, Ryan 2016

Creator(s)    : Lim Rui An, Ryan
Creation Date : 20 November 2016 13:37 PM
File Name     : Projectile.h
Purpose       : Defines a projectile object
*/

#ifndef _PROJECTILE_H
#define _PROJECTILE_H

#include "../../Engine/Objects/GameObject.h"
#include "../../Engine/System/SceneSystem.h"
#include "../../Engine/System/RenderSystem.h"

class Projectile : public GameObject
{
public:
	Projectile(const std::string& Name = "", const int& DamageDealt = 0, const float& LifeTime = 0, const float& ExistenceTime = 0) : DamageDealt(DamageDealt), LifeTime(LifeTime), ExistenceTime(ExistenceTime){ 
		SetEntityID(Name); 
		SetRotationAxis(Vector3(0,1));
	};
	virtual ~Projectile(){};

	virtual void Update(const float& dt)
	{
		if (Active) // Still can update if invisible
		{
			ExistenceTime += dt;
			if (ExistenceTime > LifeTime)
				Active = false;
			if (!Static)
			{
				// Update Angles & Position
				if (Velocity.LengthSquared() > 1.f)
				{
					// Handle Movement
					// For average speed
					Vector3 StoredVelocity = Velocity;
					// Extrapolate the position
					if (GravityAffected)
						Velocity += Vector3(0,-5.f) * dt; // For Gravity
					Position += (StoredVelocity + Velocity) * 0.5f * (float)dt;

					// Check Collision
					//CheckCollisionWithStaticObjects();

					// Update the angles and look vector
					LookVector = Velocity.Normalized();
					RotationAngle = Math::RadianToDegree(atan2(LookVector.x, LookVector.z));
				}
			}
		}
	}

	virtual void Render()
	{
		if (Active && Visible && StoredMesh)
		{
			RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
			SceneSystem::Instance().GetCurrentScene().modelStack->PushMatrix();
			SceneSystem::Instance().GetCurrentScene().modelStack->Translate(GetPosition().x, GetPosition().y + GetDimensions().y * 0.25f, GetPosition().z);
			SceneSystem::Instance().GetCurrentScene().modelStack->Rotate(GetRotationAngle(), GetRotationAxis().x, GetRotationAxis().y, GetRotationAxis().z);
			SceneSystem::Instance().GetCurrentScene().modelStack->Rotate(90, -1, 0, 0);
			SceneSystem::Instance().GetCurrentScene().modelStack->Scale(GetDimensions().x, GetDimensions().y, GetDimensions().z);
			if (Dimensions.LengthSquared() > 0.05f)
				Renderer->RenderMesh(*GetMesh(), true);
			SceneSystem::Instance().GetRenderSystem().modelStack->PopMatrix();
		}
	}

	void SetName(const std::string& Name = ""){	SetEntityID(Name); }

	void SetDamageDealt(const int& DamageDealt = 0){ this->DamageDealt = DamageDealt; }

	void SetLifeTime(const float& LifeTime = 0) { this->LifeTime = LifeTime; }

	int GetDamageDealt(){ return DamageDealt; }

	float GetLifeTime(){ return LifeTime; }

	float GetExistenceTime(){ return ExistenceTime; }

	void ResetParameters()
	{
		Active = true;
		Static = false;
		Visible = true;
		DamageDealt = 0;
		LifeTime = 0;
		ExistenceTime = 0;
	}

	std::string OwnerID = "";
	int OwnerFaction = -1;
	bool GravityAffected = true;
	bool DespawnOnHit = true;

protected:
	// Damage Dealt by this bullet
	int DamageDealt;
	// Life Time of bullet
	float LifeTime;
	// Current time of bullet
	float ExistenceTime;
	Vector3 LookVector;

};

#endif //_PROJECTILE_H