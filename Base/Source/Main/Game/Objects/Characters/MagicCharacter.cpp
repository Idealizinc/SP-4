#include "MagicCharacter.h"

MagicCharacter::MagicCharacter()
{
	Init();
}
MagicCharacter::~MagicCharacter()
{
	Exit();
}

void MagicCharacter::SetCharacter(UnitType* Type, UnitRace* Race)
{
	HealthPoints = Type->GetHealth();
	MaxHealthPoints = Type->GetMaxHealth();
	WalkSpeed = Type->GetWalkspeed();
	DetectionRadius = Type->GetRange();
	RaceType = Race->GetRace();

	int RandomChoice = Math::RandIntMinMax(0, Type->PossibleWeapon.size() - 1);
	WT = Type->PossibleWeapon[RandomChoice];
	CastTime = MaxCastTime = WT->GetRate();

	MagicStateManager* MS = dynamic_cast<MagicStateManager*>(InternalStateManager);
	MS->MWeapon = new MagicWeapon(WT->GetDamage(), WT->GetRate(), WT->GetType());
}

void MagicCharacter::Init()
{
	HealthPoints = MaxHealthPoints = 200;
	WalkSpeed = 8;
	DetectionRadius = 32.f;
	InternalTimer = 0;
	WaitTime = 0;
	FieldOfView = 150;
	TargetFriend = nullptr;

	if (InternalStateManager != nullptr)
		delete InternalStateManager;
	InternalStateManager = new MagicStateManager();
	InternalStateManager->Init();
	SetEntityID("Magic");

	InternalStateManager->SetInternalCharacter(this);
}

void MagicCharacter::Update(double dt)
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
			SetRotationAngle(Math::RadianToDegree(atan2(-GetVelocity().x, GetVelocity().y)));
	}
}

void MagicCharacter::Exit()
{
	if (InternalStateManager)
	{
		MagicStateManager* RM = dynamic_cast<MagicStateManager*>(InternalStateManager);
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