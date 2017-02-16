#include "Bullet.h"

Bullet::Bullet()
{
	Active = false;
	Damage = 10;
	DirVector.SetZero();
	PrevPos.SetZero();
}
Bullet::~Bullet()
{
	Exit();
}

void Bullet::Update(const float& dt)
{
	if (Active) // Still can update if invisible
	{
		Vector3 StoredVelocity = GetVelocity();
		//SetVelocity(GetVelocity() + Vector3(0, -9.8f) * 0.5f * dt); // For Gravity
		PrevPos = GetPosition();
		if (!Static)
			SetPosition(GetPosition() + (StoredVelocity + GetVelocity()) * 0.5f * (float)dt);
		SetRotationAngle(Math::RadianToDegree(atan2(-GetVelocity().x, GetVelocity().y)));
	}
}