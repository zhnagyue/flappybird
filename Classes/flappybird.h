#ifndef __CLASSES_FLAPPYBIRD_H__
#define __CLASSES_FLAPPTBIRD_H__

#include "cocos2d.h"
#include "cocostudio\CocoStudio.h"
#include "ui\CocosGUI.h"
#include "tube.h"
#include "tubelayer.h"
#include "bird.h"
#include "shake.h"
USING_NS_CC;

class flappybird : public Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(flappybird);
	void initUI();
	void rollbg(float i);
	void start_display();
	void startgame();
	void update(float f);
	void addtube(float yu);
	void _delete(float o);
	void deletube(Vector<tube*>& _tubes);
	virtual bool OnTouchBegan(Touch *atouch, Event *aevent);
	virtual bool OnTouchEnded(Touch *atouch, Event *aevent);
	void collision(Vector<tube*>& _tubes);
	void ended();
	void overgame();
	void over_display();
	virtual ~flappybird();
private:
	Sprite *bg1, *bg2, *land1, *land2;
	bird *_bird;
	tubelayer *_tube;
	int score,bestscore = 0;
	Label *scoreLabel;
	Sprite *scorebord;
};
#endif // !__CLASSES_FLAPPYBIRD_H__

