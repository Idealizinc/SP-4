#include "MeleeCharacter.h"

MeleeCharacter::MeleeCharacter()
{
	Init();
}

MeleeCharacter::~MeleeCharacter()
{
	Exit();
}

void MeleeCharacter::SetCharacter(UnitType* Type, UnitRace* Race)
{
	HealthPoints = Type->GetHealth() * Race->GetHealthModifier();
	MaxHealthPoints = Type->GetMaxHealth() * Race->GetHealthModifier();
	WalkSpeed = Type->GetWalkspeed();
	DetectionRadius = Type->GetRange();
	RaceType = Race->GetRace();
	SetMesh(Type->GetMeshName());
	//SetRotationAxis(Vector3(0, 1, 0));

	int RandomChoice = Math::RandIntMinMax(0, Type->PossibleWeapon.size() - 1);
	WT = Type->PossibleWeapon[RandomChoice];

	MeleeStateManager* MS = dynamic_cast<MeleeStateManager*>(InternalStateManager);
	MS->MWeapon = new MeleeWeapon(WT->GetDamage() * Race->GetMeleeDamageModifier(), WT->GetRate(), 2);

}

void MeleeCharacter::Init()
{
	FieldOfView = 200;
	InternalTimer = 0;
	WaitTime = 0;
	FieldOfView = 150;
	TargetFriend = nullptr;

	if (InternalStateManager != nullptr)
		delete InternalStateManager;
	InternalStateManager = new MeleeStateManager();
	InternalStateManager->Init();
	SetEntityID("Crusader");

	InternalStateManager->SetInternalCharacter(this);
}

void MeleeCharacter::Update(double dt)
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

void MeleeCharacter::Exit()
{
	if (InternalStateManager)
	{
		MeleeStateManager* SM = dynamic_cast<MeleeStateManager*>(InternalStateManager);
		SM->Exit();
		delete SM;
		InternalStateManager = nullptr;
	}

	for (set<BaseObject*>::iterator Iter = CharacterObjectList.begin(); Iter != CharacterObjectList.end(); /*++Iter*/)
	{
		BaseObject* temp = *Iter;
		CharacterObjectList.erase(Iter++);
		delete temp;
	}
}