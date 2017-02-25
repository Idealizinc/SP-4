#ifndef MUSIC_SYSTEM_H
#define MUSIC_SYSTEM_H

#include "../Internal/System.h"
#include "../Objects/MusicEntity2D.h"
#include "../Internal/SingletonTemplate.h"
#include <map>
#pragma comment(lib, "irrklang.lib")

class MusicSystem : public System, public SingletonTemplate<MusicSystem>
{
public:
    virtual void Init();
    virtual void Update(const float& dt);
	virtual void Render(){};
	virtual void Exit();
   
    ISoundEngine *SoundEngine;

    virtual bool PlaySong(const std::string &songName);

    virtual bool playBackgroundMusic(const std::string &songName);

	virtual void ClearMemoryUsage();
    virtual void setPlayerPos(Vector3 &pos);
    virtual void setTimeToUpdate(const double &dt);

    MusicEntity2D *FindSound(const std::string &songName);
	MusicEntity2D *BackgroundMusic;

private:
    std::map<std::string, MusicEntity2D*> cMS_SoundList;
    bool LoadSoundList(const std::string &fileName);
    size_t loopingAndFindKey(std::vector<std::string> &theKeys, const std::string &whatyouwant);
    Vector3 *playerPos;
    double TimeUpdate, m_accumulatedTime;
};

#endif