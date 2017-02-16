#include "PerspectiveRaycaster.h"

void PerspectiveRaycaster::CalculateCursorPosition()
{
	CursorOriginCoordinates = SceneSystem::Instance().cSS_InputManager->GetMousePosition();
	CursorOriginCoordinates -= Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth*0.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight*0.5f);
	CursorOriginCoordinates.x /= SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth*0.5f;
	CursorOriginCoordinates.y /= SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight*0.5f;
}

Vector3 PerspectiveRaycaster::CalculateCursorPositionInWorldSpace(Camera* camera, float CameraViewAngle, float NearPlane = 1.f)
{
	CalculateCursorPosition();

	Vector3 CameraView = (camera->target - camera->position).Normalized();
	Vector3 ScreenXAxis = CameraView.Cross(camera->up).Normalized();
	Vector3 ScreenYAxis = CameraView.Cross(ScreenXAxis).Normalized();

	float ViewAngleRadians = (float)(CameraViewAngle * Math::PI / 180.f);
	float HalfHeight = (float)(tan(ViewAngleRadians*0.5f));
	float HalfAspectRatio = HalfHeight * (SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth/ SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight);
	
	ScreenXAxis *= HalfAspectRatio;
	ScreenYAxis *= HalfHeight;

	Vector3 WorldPosition = camera->position + CameraView;
	WorldPosition.x += ScreenXAxis.x * CursorOriginCoordinates.x + ScreenYAxis.x * CursorOriginCoordinates.y;
	WorldPosition.y -= ScreenXAxis.y * CursorOriginCoordinates.x + ScreenYAxis.y * CursorOriginCoordinates.y;
	WorldPosition.z += ScreenXAxis.z * CursorOriginCoordinates.x + ScreenYAxis.z * CursorOriginCoordinates.y;

	Vector3 Direction = WorldPosition - camera->position;

	return Direction;
}
