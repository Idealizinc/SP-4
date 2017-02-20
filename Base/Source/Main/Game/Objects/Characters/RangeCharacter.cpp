#include "RangeCharacter.h"

RangeCharacter::RangeCharacter()
{
	Init();
}
RangeCharacter::~RangeCharacter()
{
	Exit();
}

void RangeCharacter::SetCharacter(UnitType* Type, UnitRace* Race)
{
	HealthPoints = Type->GetHealth();
	MaxHealthPoints = Type->GetMaxHealth();
	WalkSpeed = Type->GetWalkspeed();
	DetectionRadius = Type->GetRange();
	RaceType = Race->GetRace();
	SetMesh(Type->GetMeshName());

	int RandomChoice = Math::RandIntMinMax(0, Type->PossibleWeapon.size() - 1);
	WT = Type->PossibleWeapon[RandomChoice];

	RangeStateManager* RS = dynamic_cast<RangeStateManager*>(InternalStateManager);
	RS->RWeapon = new RangeWeapon(WT->GetDamage(), WT->GetRate());
}

void RangeCharacter::Init()
{
	InternalTimer = 0;
	WaitTime = 0;
	FieldOfView = 150;
	TargetFriend = nullptr;

	if (InternalStateManager != nullptr)
		delete InternalStateManager;
	InternalStateManager = new RangeStateManager();
	InternalStateManager->Init();
	SetEntityID("Mercedes");

	InternalStateManager->SetInternalCharacter(this);
}

void RangeCharacter::Update(double dt)
{
	if (InternalStateManager != nullptr)
		InternalStateManager->Update((float)dt);
	if (Active) // Still can update if invisible
	{
		//UpdateAlertTimer(dt);
		Vector3 StoredVelocity = GetVelocity();
		if (!Static)
			SetPosition(GetPosition() + (StoredVelocity + GetVelocity()) * 0.5f * (float)dt);
		if (!GetVelocity().IsZero())
			LookVector = GetVelocity().Normalized();
		if (GetVelocity().LengthSquared() > 0)
			SetRotationAngle(Math::RadianToDegree(atan2(-GetVelocity().x, GetVelocity().z)));
	}
}

void RangeCharacter::Exit()
{
	if (InternalStateManager)
	{
		RangeStateManager* RM = dynamic_cast<RangeStateManager*>(InternalStateManager);
		RM->Exit();
		delete RM;
		InternalStateManager = nullptr;
	}

	for (set<BaseObject*>::iterator Iter = CharacterObjectList.begin(); Iter != CharacterObjectList.end(); /*++Iter*/)
	{
		BaseObject* temp = *Iter;
		CharacterObjectList.erase(Iter++);
		delete temp;
	}
}