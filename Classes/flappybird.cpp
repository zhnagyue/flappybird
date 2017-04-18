#include "flappybird.h"
using namespace cocostudio::timeline;

Scene * flappybird::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, -900));
	auto layer = flappybird::create();
//	layer->setPhysicsWorld(scene->getPhysicsWorld());
	scene->addChild(layer);
	return scene;
}

bool flappybird::init()
{
	if (!Layer::init())
	{
		return false;
	}
	initUI();
	auto touchlisten = EventListenerTouchOneByOne::create();		//注册触摸事件
	touchlisten->onTouchBegan = CC_CALLBACK_2(flappybird::OnTouchBegan, this);
	touchlisten->onTouchEnded = CC_CALLBACK_2(flappybird::OnTouchEnded, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlisten,this);//添加事件监听器到事件分发器
	return true;
}

void flappybird::initUI()			//UI初始化
{
	/*放背景图*/	
	Size visiblesize = Director::getInstance()->getVisibleSize();																	
	bg1 = Sprite::create("bg_day.png");
	bg1->setAnchorPoint(Vec2::ZERO);
	bg1->setPosition(Vec2::ZERO);
	this->addChild(bg1,0);
	bg2 = Sprite::create("bg_day.png");
	bg2->setAnchorPoint(Vec2::ZERO);
	bg2->setPosition(Vec2(bg1->getPositionX()+bg1->getContentSize().width, bg1->getPositionY()));
	this->addChild(bg2,0);
	/*放标题*/
	auto title = Sprite::create("title.png");			//游戏开始图
	title->setPosition(visiblesize.width / 2, visiblesize.height / 1.5 + 60);
	title->setTag(1);
	title->setVisible(true);
	this->addChild(title,4);
	auto gameover = Sprite::create("text_game_over.png");			//游戏结束图
	gameover->setPosition(visiblesize.width / 2, visiblesize.height / 1.5 + 60);
	gameover->setTag(3);
	gameover->setVisible(false);
	this->addChild(gameover, 4);
	auto use = Sprite::create("tutorial.png");				//游戏玩法图
	use->setPosition(visiblesize.width / 2, visiblesize.height / 2);
	use->setTag(4);
	use->setVisible(false);
	this->addChild(use,1);
	/*放开始按钮*/
	auto startbutton = MenuItemImage::create("button_play.png","button_play.png"
		,CC_CALLBACK_0(flappybird::start_display,this));		//回调开始游戏方法
	auto menu = Menu::create(startbutton,NULL);
	menu->setPosition(visiblesize.width / 2, visiblesize.height / 2 + 30);
	menu->setAnchorPoint(Vec2(0.5, 0.5));
	menu->setTag(2);
	this->addChild(menu,1);
	/*记分牌*/
	scoreLabel = Label::createWithBMFont("bird.fnt", "0");
	scoreLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	scoreLabel->setPosition(visiblesize.width / 2, visiblesize.height / 2 + 200);
	scoreLabel->setVisible(false);
	this->addChild(scoreLabel,4);
	scorebord = Sprite::create("score_panel.png");
	scorebord->setPosition(visiblesize.width / 2, visiblesize.height / 2 + 30 );
	scorebord->setVisible(false);
	this->addChild(scorebord,4);
	/*加玩家鸟*/
	_bird = bird::create();
	Animation *_yi = Animation::create();
	_yi->addSpriteFrameWithFileName("bird0_0.png");
	_yi->addSpriteFrameWithFileName("bird0_1.png");
	_yi->addSpriteFrameWithFileName("bird0_2.png");
	_yi->setDelayPerUnit(0.5f / 3.0f);			//设置两帧之间的延迟时间
	_yi->setLoops(-1);						//设置循环
	Animate *_yh = Animate::create(_yi);
	_bird->runAction(_yh);
	auto up = MoveBy::create(0.4f, Point(0, 8));		//上下
	auto upback = up->reverse();						//晃动
	auto repeatforever = RepeatForever::create(Sequence::create(up, upback, NULL));
	_bird->runAction(repeatforever);
	repeatforever->setTag(11);
	_bird->setPosition(visiblesize.width / 2, visiblesize.height / 3);
	_bird->setTag(7);
	this->addChild(_bird, 4);

	this->scheduleUpdate();
}

void flappybird::start_display()		//开始游戏之前显示
{	
	Size visiblesize = Director::getInstance()->getVisibleSize();
	this->getChildByTag(1)->setVisible(false);
	this->getChildByTag(2)->setVisible(false);
	auto ready = Sprite::create("text_ready.png");
	ready->setPosition(visiblesize.width / 2, visiblesize.height / 1.5);
	ready->setTag(5);
	this->addChild(ready,1);
	this->getChildByTag(4)->setVisible(true);		//玩法图闪烁两次
	Blink *bli = Blink::create(2,3);
	Hide *hi = Hide::create();
	this->getChildByTag(4)->runAction(Sequence::create(
		bli, hi, CCCallFunc::create(CC_CALLBACK_0(flappybird::startgame, this)), nullptr));
}

void flappybird::startgame()			//开始游戏初始化
{
	Size visiblesize = Director::getInstance()->getVisibleSize();
	//标题不可见
	this->getChildByTag(5)->setVisible(false);
	this->getChildByTag(1)->setVisible(false);	
	_bird->stopActionByTag(11);			//主角鸟停止上下晃动
	_bird->setPosition(visiblesize.width / 3, visiblesize.height / 1.5);
	score = 0;				//重置分数
	scoreLabel->setString(String::createWithFormat("%d", score)->getCString());
	scoreLabel->setVisible(true);
	this->schedule(schedule_selector(flappybird::rollbg), 0.02f);		//滚动背景
	/*加水管*/
	_tube = tubelayer::create();
	this->addChild(_tube, 3);

	this->schedule(schedule_selector(flappybird::addtube), 1.8f);
	this->schedule(schedule_selector(flappybird::_delete), 0.2f);
}

void flappybird::rollbg(float i)		//背景滚动
{
	if (bg2->getPositionX() <= 0)
	{
		bg1->setPositionX(0);
	}
	bg1->setPositionX(bg1->getPositionX() - 2);
	bg2->setPositionX(bg1->getPositionX() + bg1->getContentSize().width - 2);
	_bird->setPositionY(_bird->getPositionY() - 1.3);
}

void flappybird::update(float f)			//更新分数
{
	bestscore = UserDefault::getInstance()->getIntegerForKey("BEST");
	if (score>bestscore)
	{
		bestscore = score;
		UserDefault::getInstance()->setIntegerForKey("BEST",bestscore);
	}
}        

void flappybird::addtube(float yu)			//添加水管
{
	_tube->addtube();
//	CCLOG("%d",_tube->_tubes.size());
}

void flappybird::_delete(float o)			//删除
{	
	this->collision(_tube->_tubes);
	this->deletube(_tube->_tubes);			
	if (_bird->getPositionY() < -10)		//主角鸟出屏幕游戏结束
	{
		ended();
	}
}

void flappybird::deletube(Vector<tube*>& _tubes)		//删除出屏幕的水管
{
	for (int i = 0; i < _tubes.size(); i++)
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		tube* _tube1 = _tubes.at(i);
		if (_tube1->getPosition().x < -3)
		{
			_tubes.erase(i);
			_tube1->removeFromParentAndCleanup(true);
		}

		if (_tube1->getName() == "new")						//计算分数
		{
			if (_tube1->getPositionX()<_bird->getPositionX())
			{
				score++;
				scoreLabel->setString(String::createWithFormat("%d", score)->getCString());
				_tube1->setName("pass");
			}
		}
	}
}

bool flappybird::OnTouchBegan(Touch * atouch, Event * aevent)			//触摸点击事件
{
	if (_bird)
	{	
		_bird->setPosition(Vec2(_bird->getPositionX(), _bird->getPositionY() + 30));
	}
	return false;
}

bool flappybird::OnTouchEnded(Touch * atouch, Event * aevent)
{
	return false;
}

void flappybird::collision(Vector<tube*>& _tubes)			//主角鸟与水管的碰撞检测
{
	auto birdrect = _bird->getBoundingBox();
	for(int i = 0 ; i < _tubes.size() ; i++)
	{
		tube *_tuber = _tubes.at(i);
		auto tubewract = _tuber->getBoundingBox();
		bool collis = birdrect.intersectsRect(tubewract);
		if (collis)
		{
			ended();
		}
	}
}

void flappybird::ended()					//游戏结束
{
	this->unschedule(schedule_selector(flappybird::addtube));
	this->unschedule(schedule_selector(flappybird::rollbg));
	this->unschedule(schedule_selector(flappybird::_delete));
	_eventDispatcher->removeAllEventListeners();			//移除监听
	Director *director = Director::sharedDirector();
	director->getActionManager()->pauseTarget(_bird->getChildByTag(7));		//主角鸟停止运动
	for (int i = 0; i < _tube->_tubes.size(); i++)		//水管停止运动
	{	
	auto _tube11s = _tube->getChildByTag(10);
	auto _tube22s = _tube->getChildByTag(20);
	auto _tube11 = _tube->_tube1;
	auto _tube22 = _tube->_tube2;
	director->getActionManager()->pauseTarget(_tube11);
	director->getActionManager()->pauseTarget(_tube22);
	director->getActionManager()->pauseTarget(_tube11s);
	director->getActionManager()->pauseTarget(_tube22s);
	}
	ScaleTo* scaleTo = ScaleTo::create(0.1, 1.25);			//水管抖动
	ScaleTo* scaleTo2 = ScaleTo::create(0.1, 1);
	_tube->runAction(CCSequence::createWithTwoActions(scaleTo, scaleTo2));

	auto down = MoveTo::create(0.5f, Point(_bird->getPositionX(), -15));	//主脚鸟坠落
	_bird->runAction(Sequence::create(down, CCCallFunc::create(CC_CALLBACK_0(flappybird::over_display, this)),nullptr));
}

void flappybird::overgame()			//重新开始游戏
{
	Director::getInstance()->replaceScene(flappybird::createScene());
}

void flappybird::over_display()		//游戏结束显示
{
	Size visiblesize = Director::getInstance()->getVisibleSize();
	auto ti2 = this->getChildByTag(3);			//显示结束图
	ti2->setVisible(true);
	scorebord->setVisible(true);				//显示记分牌
	auto curScore = Label::create(String::createWithFormat("%d", score)->getCString(), "Arial", 20);
	curScore->setPosition(scorebord->getContentSize().width - 40, scorebord->getContentSize().height - 45);
	curScore->setColor(Color3B(255, 0, 0));
	scorebord->addChild(curScore);
	auto beScore = Label::create(String::createWithFormat("%d", bestscore)->getCString(), "Arial", 20);
	beScore->setPosition(scorebord->getContentSize().width - 40, scorebord->getContentSize().height - 90);
	beScore->setColor(Color3B(0, 255, 0));
	scorebord->addChild(beScore);
	auto againbutton = MenuItemImage::create("button_play.png", "button_play.png"
			, CC_CALLBACK_0(flappybird::overgame, this));		//回调开始游戏方法
	auto menu1 = Menu::create(againbutton, NULL);
	menu1->setPosition(visiblesize.width / 2, visiblesize.height / 2 - 100);
	menu1->setAnchorPoint(Vec2(0.5, 0.5));
	menu1->setTag(4);
	this->addChild(menu1, 4);
}
flappybird::~flappybird(){}
