#include "CameraAerial.h"
#include "../../Engine/System/SceneSystem.h"
#include "SimpleCommand.h"
#include "../Mains/Application.h"

// Base
CameraAerial::CameraAerial()
{
	Init(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1));
}

CameraAerial::~CameraAerial()
{

}

// Main
void CameraAerial::Init(const Vector3& Position, const Vector3& TargetPosition, const Vector3& CameraUp)
{
	this->position = DefaultPosition = Position;
	this->target = DefaultTarget = TargetPosition;
	this->up = DefaultUp = CameraUp;
	this->LookVector = (target - position).Normalized();
	this->RightVector = LookVector.Cross(up);
	this->RightVector.y = 0;
	RightVector.Normalize();
	this->up = DefaultUp = RightVector.Cross(LookVector).Normalized();

	MinimumCameraRotation.Set(-89, -360, -35);
	MaximumCameraRotation.Set(89, 360, 35);
	CurrentCameraRotation.SetZero();
	CameraRotationSpeed = SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth / SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight;// *RelativeMouseSensitivity;
}

void CameraAerial::AltInit(const Vector3& TargetPosition, const Vector3& CameraAxialOffset, const Vector3& CameraUp)
{
	this->position = DefaultPosition = TargetPosition + CameraAxialOffset;
	this->target = DefaultTarget = TargetPosition;
	this->up = DefaultUp = CameraUp;
	this->LookVector = (TargetPosition - DefaultPosition).Normalized();
	this->RightVector = LookVector.Cross(up);
	this->RightVector.y = 0;
	RightVector.Normalize();
	this->up = DefaultUp = RightVector.Cross(LookVector).Normalized();
	this->CameraAxialOffset = CameraAxialOffset;

	MinimumCameraRotation.Set(-89, -360, -35);
	MaximumCameraRotation.Set(89, 360, 35);
	CurrentCameraRotation.SetZero();
	CameraRotationSpeed = SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth / SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight;// *RelativeMouseSensitivity;
}

void CameraAerial::Update(const float& dt)
{
	// Camera Logic Here
	Vector3 Direction;
	if ((Direction = (CameraMoveTargetPosition - (position - CameraAxialOffset))).LengthSquared() > 1.f)
	{
		position += Direction * 2 * dt;
	}
	// Calculating the resultant vectors must and can only be run after all positional/rotational updates
	CalculateCameraVectors();
}

void CameraAerial::Exit()
{

}


// Quick Use Functions
void CameraAerial::Reset()
{

}

// Internal Functions
void CameraAerial::CalculateCameraVectors()
{
	this->target = position - CameraAxialOffset;
}