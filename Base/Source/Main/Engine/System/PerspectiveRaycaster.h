#ifndef _PERSPECTIVE_RAYCASTER_H
#define _PERSPECTIVE_RAYCASTER_H

/*
Copyright (c) Lim Rui An, Ryan 2017

Creator(s)    : Lim Rui An, Ryan
Creation Date : 15 February 2017 11:42 AM
File Name     : PerspectiveRaycaster.h
Purpose       : Defines a PerspectiveRaycaster that converts mouse clicks in camera space to direction vectors in world space
*/

#include "../Internal/Entity.h"
#include "../Internal/SingletonTemplate.h"
#include "../Objects/Camera.h"
#include "SceneSystem.h"
#include "Vector3.h"
#include "Mtx44.h"


struct Ray
{
	Vector3 Origin;
	Vector3 End;
	Vector3 Direction;
};

class PerspectiveRaycaster : public SingletonTemplate<PerspectiveRaycaster>
{
public:
	Ray CalculateCursorPositionInWorldSpace(Camera* camera, const float& CameraViewAngle);
	Vector3 CalculateIntersectionPointInPlane(const Vector3& PointOnPlane, const Vector3& PlaneNormal, const Ray& Raycast);

private:
	// Functions
	void CalculateCursorPosition();

	// Variables
	Vector3 CursorOriginCoordinates;

protected:

};

#endif //_PERSPECTIVE_RAYCASTER_H