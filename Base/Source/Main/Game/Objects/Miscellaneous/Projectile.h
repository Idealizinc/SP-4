/*
Copyright (c) Lim Rui An, Ryan 2016

Creator(s)    : Lim Rui An, Ryan
Creation Date : 20 November 2016 13:37 PM
File Name     : Projectile.h
Purpose       : Defines a projectile object
*/

#ifndef _PROJECTILE_H
#define _PROJECTILE_H

#include "GameObject.h"

class Projectile : public GameObject
{
public:
	Projectile(const std::string& Name = "", const int& DamageDealt = 0, const float& LifeTime = 0, const float& ExistenceTime = 0) : DamageDealt(DamageDealt), LifeTime(LifeTime), ExistenceTime(ExistenceTime){ SetEntityID(Name); };
	virtual ~Projectile(){};

	virtual void Update(const float& dt)
	{
		if (Active) // Still can update if invisible
		{
			if (!Static)
			{
				// Update Angles & Position
				if (Velocity.LengthSquared() > 1.f)
				{
					// Handle Movement
					// For average speed
					Vector3 StoredVelocity = Velocity;
					// Extrapolate the position
					Position += (StoredVelocity + Velocity) * 0.5f * (float)dt;

					// Check Collision
					//CheckCollisionWithStaticObjects();

					// Update the angles and look vector
					LookVector = Velocity.Normalized();
					float TargetRotationAngle = Math::RadianToDegree(atan2(LookVector.x, LookVector.z));
					if (TargetRotationAngle < 0.f)
						TargetRotationAngle += 360.f;

					// Interpolate the rotation angle
					if (abs(RotationAngle - TargetRotationAngle) > Math::EPSILON)
					{
						float Interval = (float)((((int)(TargetRotationAngle - RotationAngle) % 360) + 540) % 360) - 180;
						RotationAngle += Interval * 10 * (float)dt;
					}
				}
			}
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
	bool PlayerTeam = false;

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