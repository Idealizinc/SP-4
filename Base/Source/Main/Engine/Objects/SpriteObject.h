#ifndef SPRITE_OBJECT_H
#define SPRITE_OBJECT_H

#include <vector>
#include <map>
#include "BaseObject.h"
#include "../../Game/Mains/Application.h"

class SpriteObject : public BaseObject
{
public:
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();

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