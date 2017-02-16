#include "MusicSystem.h"
#include <fstream>
#include "SceneSystem.h"
#include <sstream>
#include <set>

void MusicSystem::Init()
{
    SoundEngine = createIrrKlangDevice();
    BackgroundMusic = nullptr;
	LoadSoundList("CSVFiles/MusicDriven.csv");
    SoundEngine->setListenerPosition(vec3df(0, 0, 0), vec3df(0, 0, 0));
    SoundEngine->update();
    m_accumulatedTime = TimeUpdate = 0;
}

//This is gonna be a pain
void MusicSystem::Update(const float& dt)
{
    //How to update listener position
    m_accumulatedTime += dt;
    if (m_accumulatedTime > TimeUpdate)
    {
        SoundEngine->update();
    }
    for (std::map<std::string, MusicEntity2D*>::iterator it = cMS_SoundList.begin(), end = cMS_SoundList.end(); it != end; ++it)
    {
        it->second->Update(dt);
    }
}

bool MusicSystem::LoadSoundList(const std::string &fileName)
{
    std::ifstream file(fileName.c_str());
    if (file.is_open())
    {
        MusicEntity2D *theMusic = nullptr;
        std::string data = "";
        std::vector<std::string> keys;
        std::vector<std::string> values;
        std::set<std::string> keepingTrackOfFileNames;
        while (getline(file, data))
        {
            if (data == "")
                continue;
            std::string token = "";
            std::istringstream iss(data);
            if (keys.empty())
            {
                while (getline(iss, token, ','))
                {
                    CapitalizeString(token);
                    keys.push_back(token);
                }
            }
            else {
                while (getline(iss, token, ','))
                    values.push_back(token);

                size_t pos = loopingAndFindKey(keys, "SOUNDTYPE");
                if (values[pos] != "")
                    theMusic = new MusicEntity2D();
                if (theMusic) {
                    pos = loopingAndFindKey(keys, "SOUNDFILENAME");
                    theMusic->SetISoundSource(values[pos]);

                    pos = loopingAndFindKey(keys, "VOLUME");
                    theMusic->SetVolume(stof(values[pos]));

                    pos = loopingAndFindKey(keys, "TIMESTOPLAY");
                    theMusic->SetMaxPlayCount(stoi(values[pos]));

                    pos = loopingAndFindKey(keys, "LOOP");
                    if (values[pos] != "")
                        theMusic->SetConstantLooping(true);

                    pos = loopingAndFindKey(keys, "SOUNDID");
                    theMusic->SetEntityID(values[pos]);

                    cMS_SoundList.insert(std::pair<std::string, MusicEntity2D*>(theMusic->GetEntityID(), theMusic));
                }
                theMusic = nullptr;
                values.clear();
            }
        }

        file.close();
        return true;
    }
    return false;
}

bool MusicSystem::playBackgroundMusic(const std::string &songName)
{
    std::map<std::string, MusicEntity2D*>::iterator it = cMS_SoundList.find(songName);
    if (it != cMS_SoundList.end())
    {
        if (BackgroundMusic && BackgroundMusic->GetEntityID() != it->second->GetEntityID())
        {
            BackgroundMusic->Stop(1);
            BackgroundMusic = nullptr;
        }
        if (BackgroundMusic == nullptr)
        {
            BackgroundMusic = it->second;
            BackgroundMusic->Play();
        }
        return true;
    }
    return false;
}

bool MusicSystem::PlaySong(const std::string &songName)
{
    std::map<std::string, MusicEntity2D*>::iterator it = cMS_SoundList.find(songName);
    if (it != cMS_SoundList.end())
    {
        it->second->Play();
        return true;
    }
    return false;
}

void MusicSystem::ClearMemoryUsage()
{
    for (auto it : cMS_SoundList)
    {
        delete it.second;
    }
    cMS_SoundList.clear();
    BackgroundMusic = nullptr;
    SoundEngine->drop();
}

size_t MusicSystem::loopingAndFindKey(std::vector<std::string> &theKeys, const std::string &whatyouwant)
{
    size_t thePosOfVec = 0;
    for (std::vector<std::string>::iterator it = theKeys.begin(), end = theKeys.end(); it != end; ++it)
    {
        if ((*it).find(whatyouwant) != std::string::npos)
        {
            break;
        }
        else
            ++thePosOfVec;
    }
    return thePosOfVec;
}

void MusicSystem::setPlayerPos(Vector3 &pos)
{
    playerPos = &pos;
}

void MusicSystem::setTimeToUpdate(const double &dt)
{
    TimeUpdate = dt;
}

MusicEntity2D *MusicSystem::FindSound(const std::string &songName)
{
    std::map<std::string, MusicEntity2D*>::iterator it = cMS_SoundList.find(songName);
    if (it != cMS_SoundList.end())
    {
        return it->second;
    }
    return nullptr;
}