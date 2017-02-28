#ifndef SPRITE_OBJECT_H
#define SPRITE_OBJECT_H

#include <vector>
#include <map>
#include "GameObject.h"
#include "../../Game/Mains/Application.h"
#include "../../Engine/Objects/Boundary.h"

class SpriteObject : public GameObject
{
public:
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void SetSprite(const std::string&);

private:
	
protected:
	float Timer = 0;
	float CurrentFrameMultiplier;
	int CurrentFrame = 0;
	float anim_Time = 1;

	std::map<std::string, std::vector<GLuint>> AnimMap;
	std::vector<GLuint> CurrentAnimation;
	std::string CurrentAnimationName;
};

#endif