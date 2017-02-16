/*
Copyright (c) Lim Rui An, Ryan 2016

Creator(s)    : Lim Rui An, Ryan
Creation Date : 5 February 2016 15:13 PM
File Name     : CameraAerial.h
Purpose       : Defines a Gimbal Based Camera that is fixed in a specific direction
*/

#ifndef _CAMERA_AERIAL_H
#define _CAMERA_AERIAL_H

#include "../../Engine/Objects/Camera.h"

class CameraAerial : public Camera
{
public:
	// Base
	CameraAerial();
	virtual ~CameraAerial();

	// Main
	virtual void Init(const Vector3& Position = 0, const Vector3& TargetPosition = Vector3(1, 0, 0), const Vector3& CameraUp = Vector3(0, 1, 0));
	void AltInit(const Vector3& TargetPosition = 0, const Vector3& CameraAxialOffset = 0, const Vector3& CameraUp = Vector3(0, 1, 0));
	virtual void Update(const float&);
	virtual void Exit();

	// Quick Use Functions
	void Reset();

	// Public Variables
	Vector3 CameraAxisMovementValues;
	Vector3 CurrentCameraRotation;
	Vector3 LookVector;
	Vector3 CameraMoveTargetPosition;

private:
	// Internal Functions
	void CalculateCameraVectors();

	// Internal Variables
	Vector3 DefaultPosition, DefaultTarget, DefaultUp;
	Vector3  RightVector;
	Vector3 MinimumCameraRotation, MaximumCameraRotation;
	float CameraRotationSpeed;
	Vector3 CameraAxialOffset;

protected:

};

#endif //_CAMERA_PLAYER_H
