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
	void addtube();				//���ˮ��
	Vector<tube*> _tubes;		//���ˮ��
	tube* _tube1;
	tube* _tube2;
};
#endif // !__CLASSES_TUBELAYER_H__