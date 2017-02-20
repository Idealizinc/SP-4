#include "PerspectiveRaycaster.h"

void PerspectiveRaycaster::CalculateCursorPosition()
{
	// Convert the screen coordinates of the mouse cursor to a -1 to 1 scale
	CursorOriginCoordinates = SceneSystem::Instance().cSS_InputManager->GetMousePosition();
	CursorOriginCoordinates -= Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth*0.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight*0.5f);
	CursorOriginCoordinates.x /= SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth*0.5f;
	CursorOriginCoordinates.y /= SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight*0.5f;
}

Ray PerspectiveRaycaster::CalculateCursorPositionInWorldSpace(Camera* camera, const float& CameraViewAngle)
{
	CalculateCursorPosition();

	// Get Direction Vectors
	Vector3 CameraView = (camera->target - camera->position).Normalized();
	Vector3 ScreenXAxis = CameraView.Cross(camera->up).Normalized();
	Vector3 ScreenYAxis = CameraView.Cross(ScreenXAxis).Normalized();

	// Calculate Screen Size in World Space
	float ViewAngleRadians = (float)(CameraViewAngle * Math::PI / 180.f);
	float HalfHeight = (float)(tan(ViewAngleRadians*0.5f));
	float HalfAspectRatio = HalfHeight * (SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth/ SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight);
	
	ScreenXAxis *= HalfAspectRatio;
	ScreenYAxis *= HalfHeight;

	Ray RC;
	// Find the Ray Position in World Space
	RC.End = camera->position + CameraView;
	RC.End.x += ScreenXAxis.x * CursorOriginCoordinates.x + ScreenYAxis.x * CursorOriginCoordinates.y;
	RC.End.y += ScreenXAxis.y * CursorOriginCoordinates.x + ScreenYAxis.y * CursorOriginCoordinates.y;
	RC.End.z -= ScreenXAxis.z * CursorOriginCoordinates.x + ScreenYAxis.z * CursorOriginCoordinates.y;
	// Set the ray's origin
	RC.Origin = camera->position;
	// Calculate the direction vector of the ray
	RC.Direction = RC.End - RC.Origin;
	
	return RC;
}

Vector3 PerspectiveRaycaster::CalculateIntersectionPointInPlane(const Vector3& PointOnPlane, const Vector3& PlaneNormal, const Ray& Raycast)
{
	float TimeScalar = PlaneNormal.Dot(PointOnPlane - Raycast.Origin) / PlaneNormal.Dot(Raycast.Direction);
	Vector3 IntersectionPoint = Raycast.Origin + Raycast.Direction * TimeScalar;
	return IntersectionPoint;
}
