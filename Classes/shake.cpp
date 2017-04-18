#include "shake.h"

shake::shake()
{
}

shake * shake::create(float d, float strength)
{
	return createWithStrength(d, strength, strength);
}

shake * shake::createWithStrength(float d, float strength_x, float strength_y)
{
	shake *pRet = new shake();
    if (pRet && pRet->initWithDuration(d, strength_x, strength_y))
    {
       pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool shake::initWithDuration(float d, float strength_x, float strength_y)
{
	if (ActionInterval::initWithDuration(d))
    {
        m_strength_x = strength_x;
        m_strength_y = strength_y;
        return true;
    }
	return false;
}

float shake::fgRangeRand(float min, float max)
{
	float rnd = CCRANDOM_0_1();
    return rnd*(max - min) + min;
}

void shake::startWithTarget(cocos2d::Node * pTarget)
{
	ActionInterval::startWithTarget(pTarget);
    // save the initial position  
    m_initial_x = pTarget->getPosition().x;
    m_initial_y = pTarget->getPosition().y;
}

void shake::update(float time)
{
	float randx = fgRangeRand(-m_strength_x, m_strength_x)*time;
    float randy = fgRangeRand(-m_strength_y, m_strength_y)*time;
    // move the target to a shaked position  
    m_pTarget->setPosition(ccpAdd(ccp(m_initial_x, m_initial_y), ccp(randx, randy)));

}

void shake::stop(void)
{
	this->getTarget()->setPosition(ccp(m_initial_x, m_initial_y));
    ActionInterval::stop();
}

FallOffShake::FallOffShake()
{
}

FallOffShake * FallOffShake::create(float d, float strength)
{
	return createWithStrength(d, strength, strength);
}

FallOffShake * FallOffShake::createWithStrength(float d, float strength_x, float strength_y)
{
	FallOffShake *pRet = new FallOffShake();
    if (pRet && pRet->initWithDuration(d, strength_x, strength_y))
    {
        pRet->autorelease();
    }
	else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;

}

void FallOffShake::update(float time)
{
	float rate = (getDuration() - getElapsed()) / getDuration();
    if (rate < 0.f) {
        rate = 0.f;

	}
    float randx = fgRangeRand(-m_strength_x, m_strength_x)*rate;
    float randy = fgRangeRand(-m_strength_y, m_strength_y)*rate;
    // move the target to a shaked position  
    m_pTarget->setPosition(ccpAdd(ccp(m_initial_x, m_initial_y), ccp(randx, randy)));

}
