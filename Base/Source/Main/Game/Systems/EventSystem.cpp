#include "EventSystem.h"
#include "../../Engine/System/SceneSystem.h"
std::vector<Vector3> SpawnPoints;

void EventSystem::Init()
{
	GameOver = false;
	float Interval = SceneSystem::Instance().GetCurrentScene().TerrainScale.x * 0.15f;
	SpawnPoints.push_back(Vector3(Interval, Interval));
	SpawnPoints.push_back(Vector3(-Interval, Interval));
	SpawnPoints.push_back(Vector3(Interval, -Interval));
	SpawnPoints.push_back(Vector3(-Interval, -Interval));

	CurrentCharacterCount.insert(std::pair<std::string, int>(Robot, 0));

	CharacterCountLimit.insert(std::pair<std::string, int>(Robot, 15));

	SpawnTime.insert(std::pair<std::string, float>(Robot, 4.f));

	CollectiveTimers.insert(std::pair<std::string, float>(Robot, 0.f));

}

void EventSystem::Update(const float& dt)
{
	for (std::map<std::string, float>::iterator it = CollectiveTimers.begin(); it != CollectiveTimers.end(); ++it)
	{
		// Updating my timer.
		it->second += dt;
		float STime = SpawnTime.find((*it).first)->second;
		// Is spawn timer done?
		if (it->second > STime)
		{
			int CCount = CurrentCharacterCount.find(it->first)->second;
			int CLimit = CharacterCountLimit.find(it->first)->second;
			// Is there too many of this character?
			if (CCount < CLimit)
			{
				// Let's Spawn The Character!
				SpawnCharacter((*it).first);
				// Add To The Count.
				CurrentCharacterCount.find((*it).first)->second += 1;
				// Reset The Timer.
				it->second = 0;
			}
		}
	}
}

void EventSystem::SpawnCharacter(std::string Name)
{
	if (Name == Robot)
	{
		Math::InitRNG();
		int Choice = Math::RandIntMinMax(0, SpawnPoints.size() - 1);
		RobotCharacter* NewChar = new RobotCharacter();
		NewChar->Init();
		NewChar->SetPosition(SpawnPoints[Choice]);
		//NewChar->SetPosition(Vector3(0,0,10));
		NewChar->SetDimensions(Vector3(RobotSize, RobotSize, RobotSize));
		NewChar->GenerateRobot(NewChar->GetPosition());
	}
}

void EventSystem::ClearCharacterCount()
{
	for (std::map<std::string, int>::iterator it = CurrentCharacterCount.begin(); it != CurrentCharacterCount.end(); ++it)
	{
		it->second = 0;
	}
}