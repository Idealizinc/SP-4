#include "MusicEntity2D.h"
#include "MyMath.h"
#include "../System/MusicSystem.h"

MusicEntity2D::MusicEntity2D() : Entity()
{
	SoundSource = nullptr;
	SoundVolume = 1.f;
	MaxPlayCount = 1;
	IsLooping = false;
	ElapsedTime = 0;
	StoppedPlaying = true;
}

MusicEntity2D::~MusicEntity2D()
{
	while (HistoryOfPlayTimes.size() > 0)
	{
		ISound *theFront = HistoryOfPlayTimes.front();
		if (theFront)
		{
			theFront->drop();
			theFront = 0;
		}
		HistoryOfPlayTimes.erase(HistoryOfPlayTimes.begin());
	}
}

void MusicEntity2D::SetVolume(const float &Volume)
{
	SoundVolume = Volume;
	SoundVolume = Math::Clamp(SoundVolume, 0.01f, 1.f);
	SoundSource->setDefaultVolume(SoundVolume);
}

void MusicEntity2D::Init(const std::string &Name, const std::string &FileName, const float &Volume, const int &MaxPlayCount, const bool &Looping)
{
	SetEntityID(Name);
	SetISoundSource(FileName);
	SetVolume(Volume);
	SetMaxPlayCount(MaxPlayCount);
	SetConstantLooping(Looping);
}

void MusicEntity2D::Play()
{
	if (HistoryOfPlayTimes.size() < MaxPlayCount || IsLooping)
	{
		ISound *thEffect = MusicSystem::Instance().SoundEngine->play2D(SoundSource, IsLooping, false, true);
		HistoryOfPlayTimes.push_back(thEffect);
		StoppedPlaying = false;
	}
}

void MusicEntity2D::Update(const float& dt)
{
	ElapsedTime = dt;
	if (HistoryOfPlayTimes.size() > 0 && HistoryOfPlayTimes.front()->isFinished())
	{
		ISound *theFront = HistoryOfPlayTimes.front();
		HistoryOfPlayTimes.erase(HistoryOfPlayTimes.begin());
		if (theFront)
		{
			theFront->drop();
			theFront = 0;
		}
	}
	else if (HistoryOfPlayTimes.size() > 0 && StoppedPlaying)
	{
		Stop(dt);
	}
}

void MusicEntity2D::SetConstantLooping(const bool &Looping)
{
	IsLooping = Looping;
}

void MusicEntity2D::SetMaxPlayCount(const int &PlayCount)
{
	if (PlayCount > 0)
		MaxPlayCount = PlayCount;
}

void MusicEntity2D::Stop(const float& dt)
{
	for (std::vector<ISound*>::iterator it = HistoryOfPlayTimes.begin(), end = HistoryOfPlayTimes.end(); it != end;)
	{
		ISound * Sound = (*it);
		if (Sound)
		{
			float decrement = 0;
			if (dt > Math::EPSILON)
				decrement = (float)dt;
			else
				decrement = (float)ElapsedTime;
			if (Sound->getVolume() - decrement > Math::EPSILON)
			{
				Sound->setVolume(Sound->getVolume() - decrement);
				++it;
			}
			else
			{
				Sound->stop();
				Sound->drop();
				Sound = 0;
				bool justBreakIt = false;
				std::vector<ISound*>::iterator it2;
				if (it + 1 == end)
				{
					justBreakIt = true;
				}
				else
					it2 = it + 1;
				HistoryOfPlayTimes.erase(it);
				if (justBreakIt)
					break;
				it = it2;
			}
		}
	}
}

float MusicEntity2D::GetVolume()
{
	return SoundVolume;
}

unsigned MusicEntity2D::GetPlayCount()
{
	return MaxPlayCount;
}

bool MusicEntity2D::GetLooping()
{
	return IsLooping;
}

void MusicEntity2D::SetISoundSource(const std::string &File)
{
	FileName = File;
	SoundSource =
		MusicSystem::Instance().SoundEngine->addSoundSourceFromFile(FileName.c_str());
}