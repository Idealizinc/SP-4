/*
Copyright (c) Clinton Teo Jun Quan 2016

Creator(s)    : Clinton Teo Jun Quan
Creation Date : 5 February 2016 15:13 PM
File Name     : EnemyController.h
Purpose       : Controls sequence for what enemy will do on his turn
*/

#ifndef ENEMYCONTROLLER_H
#define ENEMYCONTROLLER_H

#include "../../../Engine/State/StateManager.h"

class EnemyController : public StateManager
{
public:
	EnemyController();
	virtual ~EnemyController();

	virtual void Init(){};
	virtual void Update(const float& dt){};
	virtual void Exit(){};

private:


};

#endif