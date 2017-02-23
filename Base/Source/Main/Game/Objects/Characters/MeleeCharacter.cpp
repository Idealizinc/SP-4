#include "MeleeCharacter.h"
#include "../../Miscellaneous/MeshBuilder.h"

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
	
	HealthPoints = (int)(Type->GetHealth() * Race->GetHealthModifier());
	MaxHealthPoints = (int)(Type->GetMaxHealth() * Race->GetHealthModifier());
	WalkSpeed = Type->GetWalkspeed();
	DetectionRadius = Type->GetRange() * 2;
	RaceType = Race->GetRace();
	SetSprite(Type->GetMeshName());
	//SetMesh(Type->GetMeshName());
	SetMesh(MeshBuilder::GenerateQuad("cosyoloidontevenknow", 0, 1));
	//anim->SetSprite(Type->GetMeshName());
	SetRotationAxis(Vector3(0, 1, 0));

	int RandomChoice = Math::RandIntMinMax(0, Type->PossibleWeapon.size() - 1);
	WT = Type->PossibleWeapon[RandomChoice];

	MeleeStateManager* MS = dynamic_cast<MeleeStateManager*>(InternalStateManager);
	MS->MWeapon = new MeleeWeapon((int)(WT->GetDamage() * Race->GetMeleeDamageModifier()), WT->GetRate(),0, 2);

}

void MeleeCharacter::Init()
{
	isPlayer = false;
	FieldOfView = 200;
	InternalTimer = 0;
	WaitTime = 0;
	FieldOfView = 150;
	TargetFriend = nullptr;
	anim_Time = Math::RandFloatMinMax(1, 5);

	if (InternalStateManager != nullptr)
		delete InternalStateManager;
	InternalStateManager = new MeleeStateManager();
	InternalStateManager->Init();
	SetEntityID("Crusader");

	InternalStateManager->SetInternalCharacter(this);
}

void MeleeCharacter::Update(const float& dt)
{
	if (InternalStateManager != nullptr)
		InternalStateManager->Update((float)dt);
	if (Active) // Still can update if invisible
	{
		Timer += (float)dt;

		CurrentAnimationName = GetCurrentState()->GetStateName();

		if (CurrentAnimationName == "Idle")
			CurrentAnimation = AnimMap.find("Idle")->second;
		else if (CurrentAnimationName == "Scout")
			CurrentAnimation = AnimMap.find("Scout")->second;

		//UpdateAlertTimer(dt);
		Vector3 StoredVelocity = GetVelocity();
		if (!Static)
			SetPosition(GetPosition() + (StoredVelocity + GetVelocity()) * 0.5f * (float)dt);
		if (!GetVelocity().IsZero())
			LookVector = GetVelocity().Normalized();
		if (GetVelocity().LengthSquared() > 0)
			SetRotationAngle(Math::RadianToDegree(atan2(-GetVelocity().x, GetVelocity().z)));


		if (Timer > anim_Time)
			Timer = 0;

		CurrentFrameMultiplier = Timer / anim_Time;
		CurrentFrame = (int)(CurrentAnimation.size() * CurrentFrameMultiplier);

		// This will set the texture to be rendered with the mesh to the one of the correct animation frame
		StoredMesh->textureArray[0] = CurrentAnimation[CurrentFrame];

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