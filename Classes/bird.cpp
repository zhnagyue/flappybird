#include "bird.h"

bird* bird::create()
{
	bird *b1 = new bird();
	Size visiblisize = Director::getInstance()->getVisibleSize();
	b1->init1();
	if (b1)
	{
		b1->setPosition(visiblisize.width / 4, visiblisize.height / 1.5);
		return b1;
	}
	delete b1;
	return nullptr;
}

void bird::init1()
{
	initWithFile("bird0_0.png");
}
 