#include "Boundary.h"

Boundary::Boundary() : BaseObject()
{
	SetVerticeNo(4);
}

Boundary::Boundary(const Vector3 &pos, const Vector3 &dimension, const float &rotation)
{
	SetPosition(pos);
	SetDimensions(dimension);
	SetRotationAngle(rotation);
	SetVerticeNo(4);
	ResetValues();
}

Boundary::Boundary(const Boundary &bounds)
{
	this->Axis = bounds.Axis;
	this->Vertices = bounds.Vertices;
	this->VerticeNo = bounds.VerticeNo;
}

Boundary::~Boundary()
{
	Axis.clear();
	Vertices.clear();
}

void Boundary::SetVerticeNo(const int &verticeNo)
{
	this->VerticeNo = verticeNo;
}

void Boundary::SetAxis()
{
	Axis.clear();
	for (int i = 0; i < VerticeNo; ++i)
	{
		Vector3 point1 = Vertices[i];
		Vector3 point2 = Vertices[i + 1 == VerticeNo ? 0 : i + 1];
		Vector3 edge = point1 - point2;
		Vector3 N = Vector3(edge.z, 0, -edge.x);
		N.Normalize();
		Axis.push_back(N);
	}
}

void Boundary::SetVertices()
{
	Vertices.clear();
	Mtx44 Rotation;
	Rotation.SetToRotation(this->GetRotationAngle(), 0, 1, 0);
	Vertices.push_back(this->GetPosition() + (Rotation * Vector3(this->GetDimensions().x + 2, 0, this->GetDimensions().z + 2) * 0.5f));
	Vertices.push_back(this->GetPosition() + (Rotation * Vector3(-this->GetDimensions().x - 2, 0, this->GetDimensions().z + 2) * 0.5f));
	Vertices.push_back(this->GetPosition() + (Rotation * Vector3(-this->GetDimensions().x - 2, 0, -this->GetDimensions().z - 2) * 0.5f));
	Vertices.push_back(this->GetPosition() + (Rotation * Vector3(this->GetDimensions().x + 2, 0, -this->GetDimensions().z - 2) * 0.5f));
	Rotation.SetToZero();
}

int Boundary::GetVerticeNo()const
{
	return this->VerticeNo;
}

std::vector<Vector3> Boundary::GetAxis()const
{
	return this->Axis;
}

std::vector<Vector3> Boundary::GetVertices()const
{
	return this->Vertices;
}

bool Boundary::CheckCollision(const Boundary &object)
{
	for (int i = 0; i < this->VerticeNo; i++)
	{
		Projection point1 = SetProjection(this->Axis[i], this->Vertices);
		Projection point2 = SetProjection(this->Axis[i], object.Vertices);
		if (!point1.DetermineCollision(point2))
			return false;
	}
	for (int i = 0; i < object.VerticeNo; i++)
	{
		Projection point1 = SetProjection(object.Axis[i], this->Vertices);
		Projection point2 = SetProjection(object.Axis[i], object.Vertices);
		if (!point1.DetermineCollision(point2))
			return false;
	}
	return true;
}

bool Boundary::CheckCollision(const Vector3 &point)
{
	SetOverlappingDistance(0.f);
	SetOverlappingAxis(Vector3(0, 0, 0));
	float overlap = 9999.f;
	Vector3 overlappedAxis;
	for (int i = 0; i < VerticeNo; i++)
	{
		Projection point1 = SetProjection(this->Axis[i], this->Vertices);
		Projection point2 = SetProjectionPoint(this->Axis[i], point);
		if (!point1.DetermineCollision(point2))
			return false;
		else
		{
			float value = point1.GetOverlappingDistance(point2);
			if (overlap > value)
			{
				overlap = value;
				overlappedAxis = this->Axis[i];
			}
		}
	}
	SetOverlappingDistance(overlap);
	SetOverlappingAxis(overlappedAxis);
	return true;
}

bool Boundary::CheckCollisionLaser(const Vector3 &Pos, const Bullet& bullet, const float& Radius) {

	Vector3 P1, P2, N, Line, DirV, P3, P4, Line2, N2;
	P1 = Pos + Vector3(Radius * 0.5f, 0, 0);
	P2 = Pos - Vector3(Radius * 0.5f, 0, 0);
	P3 = Pos + Vector3(0, 0, Radius * 0.5f);
	P4 = Pos - Vector3(0, 0, Radius * 0.5f);
	P1.y = 0;
	P2.y = 0;
	P3.y = 0;
	P4.y = 0;
	Line = P2 - P1;
	Line2 = P4 - P3;
	N = Vector3(-Line.z, 0, Line.x);
	N2 = Vector3(-Line2.z, 0, Line2.x);

	Vector3 PrevPos, BulletCurrPos;
	BulletCurrPos = Vector3(bullet.GetPosition().x, 0, bullet.GetPosition().z);
	PrevPos = Vector3(bullet.PrevPos.x, 0, bullet.PrevPos.z);
	float D1, D2, D3, D4;
	D1 = ((BulletCurrPos - P1).Dot(N));
	D2 = ((PrevPos - P1).Dot(N));

	D3 = ((BulletCurrPos - P3).Dot(N2));
	D4 = ((PrevPos - P3).Dot(N2));

	float t = (1 - D2) / (D1 - D2);
	float t2 = (1 - D4) / (D3 - D4);

	if ((t > 0 && t < 1) || (t2 > 0 && t2 < 1))
	{
		return true;
	}
	return false;
}

bool Boundary::BoundaryCheckBullet(const Vector3& Bullet, const Vector3& Position, const float& Radius) {
	if (Radius >(Vector3(Position.x, 0, Position.z) - Vector3(Bullet.x, 0, Bullet.z)).LengthSquared())
	{
		return true;
	}
	return false;
}

void Boundary::ResetValues()
{
	SetVertices();
	SetAxis();
}

Projection Boundary::SetProjection(const Vector3 &axis, std::vector<Vector3> vertice)
{
	float min, max;
	min = max = axis.Dot(vertice[0]);
	for (int i = 1; i < this->VerticeNo; i++)
	{
		float value = (axis.x * vertice[i].x + axis.z * vertice[i].z);
		if (value < min)
			min = value;
		else if (value > max)
			max = value;
	}
	Projection proj(min, max);
	return proj;
}

Projection Boundary::SetProjectionPoint(const Vector3 &axis, const Vector3 &point)
{
	float min, max;
	min = max = (axis.x * point.x + axis.z * point.z);
	Projection proj(min, max);
	return proj;
}

void Boundary::SetOverlappingAxis(const Vector3 &axis)
{
	this->OverlappingAxis = axis;
}

void Boundary::SetOverlappingDistance(const float &distance)
{
	this->OverlappingDistance = distance;
}

Vector3 Boundary::GetOverlappingAxis()const
{
	return this->OverlappingAxis;
}

float Boundary::GetOverlappingDistance()const
{
	return this->OverlappingDistance;
}