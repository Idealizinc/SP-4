/*
Copyright (c) Lim Rui An, Ryan 2016

Creator(s)    : Lim Rui An, Ryan
Creation Date : 7 November 2016 16:42 PM
File Name     : SingletonTemplate.h
Purpose       : Defines a template class for anything that requires a singleton
*/

#ifndef SINGLETON_TEMPLATE_H
#define SINGLETON_TEMPLATE_H

//#include "DetectMemoryLeak.h"

template<class Singleton>
class SingletonTemplate
{
public:
	static Singleton &Instance() {
		static Singleton TheInstance;
		return TheInstance;
    }

protected:
    SingletonTemplate() {};
    virtual ~SingletonTemplate() {};
};

#endif