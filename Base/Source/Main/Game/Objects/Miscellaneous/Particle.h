/*
Copyright (c) Lim Rui An, Ryan 2016

Creator(s)    : Lim Rui An, Ryan
Creation Date : 26 November 2016 12:15 PM
File Name     : Particle.h
Purpose       : Defines a Particle
*/

#ifndef _PARTICLE_H
#define _PARTICLE_H

#include "Vector3.h"
#include "Mesh.h"

#include "../../../Engine/Objects/BaseObject.h"

class Particle : public BaseObject{
public:
	Particle(){ Active = false; };
	Particle(const std::string& MeshName, const float& Mass, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const Vector3& PlayerPosition, const float& LifeTime);

	Vector3 PlayerPosition;

	virtual void Update(double dt);

	void Reset();
	bool CheckLife();
	
	void SetMeshBasedOnID();

private:
	Vector3 DefaultDimensions;
	float LifeTime = 0;
	float CurrentTime = 0;
	Mesh* StoredMesh;

	bool operator<(Particle& rhs);

};

#endif // _PARTICLE_H