#ifndef __CLASSES_BIRD_H__
#define __CLASSES_BIRD_H__
#include "cocos2d.h"
USING_NS_CC;
class bird : public Sprite
{
public:
	static bird* create();
	void init1();
};
#endif // !__CLASSES_BIRD_H__

