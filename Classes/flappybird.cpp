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
	auto touchlisten = EventListenerTouchOneByOne::create();		//ע�ᴥ���¼�
	touchlisten->onTouchBegan = CC_CALLBACK_2(flappybird::OnTouchBegan, this);
	touchlisten->onTouchEnded = CC_CALLBACK_2(flappybird::OnTouchEnded, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlisten,this);//����¼����������¼��ַ���
	return true;
}

void flappybird::initUI()			//UI��ʼ��
{
	/*�ű���ͼ*/	
	Size visiblesize = Director::getInstance()->getVisibleSize();																	
	bg1 = Sprite::create("bg_day.png");
	bg1->setAnchorPoint(Vec2::ZERO);
	bg1->setPosition(Vec2::ZERO);
	this->addChild(bg1,0);
	bg2 = Sprite::create("bg_day.png");
	bg2->setAnchorPoint(Vec2::ZERO);
	bg2->setPosition(Vec2(bg1->getPositionX()+bg1->getContentSize().width, bg1->getPositionY()));
	this->addChild(bg2,0);
	/*�ű���*/
	auto title = Sprite::create("title.png");			//��Ϸ��ʼͼ
	title->setPosition(visiblesize.width / 2, visiblesize.height / 1.5 + 60);
	title->setTag(1);
	title->setVisible(true);
	this->addChild(title,4);
	auto gameover = Sprite::create("text_game_over.png");			//��Ϸ����ͼ
	gameover->setPosition(visiblesize.width / 2, visiblesize.height / 1.5 + 60);
	gameover->setTag(3);
	gameover->setVisible(false);
	this->addChild(gameover, 4);
	auto use = Sprite::create("tutorial.png");				//��Ϸ�淨ͼ
	use->setPosition(visiblesize.width / 2, visiblesize.height / 2);
	use->setTag(4);
	use->setVisible(false);
	this->addChild(use,1);
	/*�ſ�ʼ��ť*/
	auto startbutton = MenuItemImage::create("button_play.png","button_play.png"
		,CC_CALLBACK_0(flappybird::start_display,this));		//�ص���ʼ��Ϸ����
	auto menu = Menu::create(startbutton,NULL);
	menu->setPosition(visiblesize.width / 2, visiblesize.height / 2 + 30);
	menu->setAnchorPoint(Vec2(0.5, 0.5));
	menu->setTag(2);
	this->addChild(menu,1);
	/*�Ƿ���*/
	scoreLabel = Label::createWithBMFont("bird.fnt", "0");
	scoreLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	scoreLabel->setPosition(visiblesize.width / 2, visiblesize.height / 2 + 200);
	scoreLabel->setVisible(false);
	this->addChild(scoreLabel,4);
	scorebord = Sprite::create("score_panel.png");
	scorebord->setPosition(visiblesize.width / 2, visiblesize.height / 2 + 30 );
	scorebord->setVisible(false);
	this->addChild(scorebord,4);
	/*�������*/
	_bird = bird::create();
	Animation *_yi = Animation::create();
	_yi->addSpriteFrameWithFileName("bird0_0.png");
	_yi->addSpriteFrameWithFileName("bird0_1.png");
	_yi->addSpriteFrameWithFileName("bird0_2.png");
	_yi->setDelayPerUnit(0.5f / 3.0f);			//������֮֡����ӳ�ʱ��
	_yi->setLoops(-1);						//����ѭ��
	Animate *_yh = Animate::create(_yi);
	_bird->runAction(_yh);
	auto up = MoveBy::create(0.4f, Point(0, 8));		//����
	auto upback = up->reverse();						//�ζ�
	auto repeatforever = RepeatForever::create(Sequence::create(up, upback, NULL));
	_bird->runAction(repeatforever);
	repeatforever->setTag(11);
	_bird->setPosition(visiblesize.width / 2, visiblesize.height / 3);
	_bird->setTag(7);
	this->addChild(_bird, 4);

	this->scheduleUpdate();
}

void flappybird::start_display()		//��ʼ��Ϸ֮ǰ��ʾ
{	
	Size visiblesize = Director::getInstance()->getVisibleSize();
	this->getChildByTag(1)->setVisible(false);
	this->getChildByTag(2)->setVisible(false);
	auto ready = Sprite::create("text_ready.png");
	ready->setPosition(visiblesize.width / 2, visiblesize.height / 1.5);
	ready->setTag(5);
	this->addChild(ready,1);
	this->getChildByTag(4)->setVisible(true);		//�淨ͼ��˸����
	Blink *bli = Blink::create(2,3);
	Hide *hi = Hide::create();
	this->getChildByTag(4)->runAction(Sequence::create(
		bli, hi, CCCallFunc::create(CC_CALLBACK_0(flappybird::startgame, this)), nullptr));
}

void flappybird::startgame()			//��ʼ��Ϸ��ʼ��
{
	Size visiblesize = Director::getInstance()->getVisibleSize();
	//���ⲻ�ɼ�
	this->getChildByTag(5)->setVisible(false);
	this->getChildByTag(1)->setVisible(false);	
	_bird->stopActionByTag(11);			//������ֹͣ���»ζ�
	_bird->setPosition(visiblesize.width / 3, visiblesize.height / 1.5);
	score = 0;				//���÷���
	scoreLabel->setString(String::createWithFormat("%d", score)->getCString());
	scoreLabel->setVisible(true);
	this->schedule(schedule_selector(flappybird::rollbg), 0.02f);		//��������
	/*��ˮ��*/
	_tube = tubelayer::create();
	this->addChild(_tube, 3);

	this->schedule(schedule_selector(flappybird::addtube), 1.8f);
	this->schedule(schedule_selector(flappybird::_delete), 0.2f);
}

void flappybird::rollbg(float i)		//��������
{
	if (bg2->getPositionX() <= 0)
	{
		bg1->setPositionX(0);
	}
	bg1->setPositionX(bg1->getPositionX() - 2);
	bg2->setPositionX(bg1->getPositionX() + bg1->getContentSize().width - 2);
	_bird->setPositionY(_bird->getPositionY() - 1.3);
}

void flappybird::update(float f)			//���·���
{
	bestscore = UserDefault::getInstance()->getIntegerForKey("BEST");
	if (score>bestscore)
	{
		bestscore = score;
		UserDefault::getInstance()->setIntegerForKey("BEST",bestscore);
	}
}        

void flappybird::addtube(float yu)			//���ˮ��
{
	_tube->addtube();
//	CCLOG("%d",_tube->_tubes.size());
}

void flappybird::_delete(float o)			//ɾ��
{	
	this->collision(_tube->_tubes);
	this->deletube(_tube->_tubes);			
	if (_bird->getPositionY() < -10)		//���������Ļ��Ϸ����
	{
		ended();
	}
}

void flappybird::deletube(Vector<tube*>& _tubes)		//ɾ������Ļ��ˮ��
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

		if (_tube1->getName() == "new")						//�������
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

bool flappybird::OnTouchBegan(Touch * atouch, Event * aevent)			//��������¼�
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

void flappybird::collision(Vector<tube*>& _tubes)			//��������ˮ�ܵ���ײ���
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

void flappybird::ended()					//��Ϸ����
{
	this->unschedule(schedule_selector(flappybird::addtube));
	this->unschedule(schedule_selector(flappybird::rollbg));
	this->unschedule(schedule_selector(flappybird::_delete));
	_eventDispatcher->removeAllEventListeners();			//�Ƴ�����
	Director *director = Director::sharedDirector();
	director->getActionManager()->pauseTarget(_bird->getChildByTag(7));		//������ֹͣ�˶�
	for (int i = 0; i < _tube->_tubes.size(); i++)		//ˮ��ֹͣ�˶�
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
	ScaleTo* scaleTo = ScaleTo::create(0.1, 1.25);			//ˮ�ܶ���
	ScaleTo* scaleTo2 = ScaleTo::create(0.1, 1);
	_tube->runAction(CCSequence::createWithTwoActions(scaleTo, scaleTo2));

	auto down = MoveTo::create(0.5f, Point(_bird->getPositionX(), -15));	//������׹��
	_bird->runAction(Sequence::create(down, CCCallFunc::create(CC_CALLBACK_0(flappybird::over_display, this)),nullptr));
}

void flappybird::overgame()			//���¿�ʼ��Ϸ
{
	Director::getInstance()->replaceScene(flappybird::createScene());
}

void flappybird::over_display()		//��Ϸ������ʾ
{
	Size visiblesize = Director::getInstance()->getVisibleSize();
	auto ti2 = this->getChildByTag(3);			//��ʾ����ͼ
	ti2->setVisible(true);
	scorebord->setVisible(true);				//��ʾ�Ƿ���
	auto curScore = Label::create(String::createWithFormat("%d", score)->getCString(), "Arial", 20);
	curScore->setPosition(scorebord->getContentSize().width - 40, scorebord->getContentSize().height - 45);
	curScore->setColor(Color3B(255, 0, 0));
	scorebord->addChild(curScore);
	auto beScore = Label::create(String::createWithFormat("%d", bestscore)->getCString(), "Arial", 20);
	beScore->setPosition(scorebord->getContentSize().width - 40, scorebord->getContentSize().height - 90);
	beScore->setColor(Color3B(0, 255, 0));
	scorebord->addChild(beScore);
	auto againbutton = MenuItemImage::create("button_play.png", "button_play.png"
			, CC_CALLBACK_0(flappybird::overgame, this));		//�ص���ʼ��Ϸ����
	auto menu1 = Menu::create(againbutton, NULL);
	menu1->setPosition(visiblesize.width / 2, visiblesize.height / 2 - 100);
	menu1->setAnchorPoint(Vec2(0.5, 0.5));
	menu1->setTag(4);
	this->addChild(menu1, 4);
}
flappybird::~flappybird(){}
