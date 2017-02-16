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

#ifndef _PERSPECTIVE_RAYCASTER_H
#define _PERSPECTIVE_RAYCASTER_H

class PerspectiveRaycaster : public SingletonTemplate<PerspectiveRaycaster>
{
public:
	Vector3 CalculateCursorPositionInWorldSpace(Camera* camera, float CameraViewAngle, float NearPlane);

private:
	Vector3 CursorOriginCoordinates;

	void CalculateCursorPosition();

protected:

};

#endif //_PERSPECTIVE_RAYCASTER_H