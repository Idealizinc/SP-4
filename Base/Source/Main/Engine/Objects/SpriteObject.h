#ifndef SPRITE_OBJECT_H
#define SPRITE_OBJECT_H

#include <vector>
#include <map>
#include "BaseObject.h"
//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

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