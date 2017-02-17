#ifndef ROBOT_CHARACTER_H
#define ROBOT_CHARACTER_H

//#include "../../ObjectStates/RobotStateManager.h"
#include "../../../Engine/Objects/CharacterEntity.h"
#include "../../SceneManagement/ScenePartitionGraph.h"

class RobotCharacter : public CharacterEntity
{
public:
	RobotCharacter();
	~RobotCharacter();

	virtual void Init();
	virtual void Update(const float& dt);
	//virtual void Render();
	void GenerateRobot(const Vector3& bodyPosition);
	bool CheckCollision(const string& type = "Ray");
	void Exit();

	float InternalTimer2;
	float WaitTime;

	void AssignToSPG();
	Node* Self;

private:
	Node* LHand;
	Node* RHand;
	Node* LLeg;
	Node* RLeg;
	float HandAnimationTimer;
	float HandAnimation;
	bool HandPlus;
	float DeathOffset;
};


#endif