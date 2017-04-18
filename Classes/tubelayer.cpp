#include "tubelayer.h"

Scene * tubelayer::createScene()
{
	auto scene = Scene::create();
	auto layer = tubelayer::create();
	scene->addChild(layer);
	return scene;
}

bool tubelayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	_tubes = Vector<tube*>();
	srand((unsigned)time(NULL));		//随机发生器
	this->scheduleUpdate();
	return true;
}

void tubelayer::addtube()
{
		Size visiblesize = Director::getInstance()->getVisibleSize();
		_tube1 = tube::create();				//第一种水管上部分
		_tube1->initWithFile("pipe_down.png");
		this->addChild(_tube1);
		_tubes.pushBack(_tube1);			//加入容器中
		_tube2 = tube::create();
		_tube2->initWithFile("pipe_up.png");		//第一种水管下部分
		this->addChild(_tube2);
		_tubes.pushBack(_tube2);
		Size tubesize = _tube1->getContentSize();
		int maxUpY = visiblesize.height + tubesize.height / 4;			//上部分上下范围
		int minUpY = visiblesize.height - tubesize.height / 4;
		int y1 = CCRANDOM_0_1()*(maxUpY - minUpY) + minUpY;
		int maxDownY = tubesize.height / 4;								//下部分上下范围
		int minDownY = -tubesize.height / 4;
		int y2 = CCRANDOM_0_1()*(maxDownY - minDownY) + minDownY;
		_tube1->setPosition(ccp(visiblesize.width + tubesize.width / 2 , y1));
		_tube2->setPosition(ccp(visiblesize.width + tubesize.width / 2 , y2));
		auto move1 = MoveTo::create(2.5, Vec2(-80, y1));
		_tube1->runAction(move1);
		auto move2 = MoveTo::create(2.5, Vec2(-80, y2));
		_tube2->runAction(move2);
		_tube1->setTag(10); 
		_tube2->setTag(20);
		_tube1->setName("new");
}
