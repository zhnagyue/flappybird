#ifndef __CLASSES_TUBELAYER_H__
#define __CLASSES_TUBELAYER_H__
#include "cocos2d.h"
USING_NS_CC;
#include "tube.h"
class tubelayer : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(tubelayer);
	void addtube();				//添加水管
	Vector<tube*> _tubes;		//存放水管
	tube* _tube1;
	tube* _tube2;
};
#endif // !__CLASSES_TUBELAYER_H__