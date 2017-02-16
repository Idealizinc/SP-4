#ifndef _BULLET_H
#define _BULLET_H

#include "../../Engine/Objects/BaseObject.h"

class Bullet: public BaseObject
{
public:

	Bullet();
	virtual ~Bullet();

	virtual void Update(const float& dt);

	int Damage;
	Vector3 DirVector;
	Vector3 PrevPos;
private:

};

#endif