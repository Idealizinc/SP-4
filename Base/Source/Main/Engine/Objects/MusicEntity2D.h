#ifndef MUSIC_ENTITY_2D_H
#define MUSIC_ENTITY_2D_H

#include "../Internal/Entity.h"
#include "irrklang.h"
#include <queue>
#include "Vector3.h"
#include <vector>

using namespace irrklang;

class MusicEntity2D : public Entity
{
public:
    MusicEntity2D();
    virtual ~MusicEntity2D();

	virtual void Init(){};
	virtual void Init(const std::string &Name, const std::string &FileName, const float &Volume, const int &MaxPlayCount = 1, const bool &Looping = false);
    virtual void Play();
	virtual void Render(){};
    virtual void Update(const float& dt);
    virtual void SetVolume(const float &);
    virtual void SetConstantLooping(const bool &);
    virtual void SetMaxPlayCount(const int &);
    virtual void SetISoundSource(const std::string &);
	virtual void Exit(){};

	virtual void Stop(const float& dt);

	float GetVolume();
	unsigned GetPlayCount();
    bool GetLooping();
    std::string GetFileName();

protected:
    ISoundSource *SoundSource;
    float SoundVolume;
    std::vector<ISound*> HistoryOfPlayTimes;
    unsigned MaxPlayCount;
    bool IsLooping, StoppedPlaying;
    std::string FileName;
    double ElapsedTime;
};

#endif