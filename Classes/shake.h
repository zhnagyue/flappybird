#ifndef __CLASSES_SHAKE_H__
#define __CLASSES_SHAKE_H__
#include "cocos2d.h"
USING_NS_CC;
//#include "CCApplication.h"
/**
 * ��ָ��Ƶ�ȷ�Χ�ڶ���[-strength_x,strength_x][-strength_y, strength_y]
 */
class shake : public cocos2d::ActionInterval
{
public:
    shake();
    // Create the action with a time and a strength (same in x and y)  
    static shake* create(float d, float strength);
    // Create the action with a time and strengths (different in x and y)  
    static shake* createWithStrength(float d, float strength_x, float strength_y);
    bool initWithDuration(float d, float strength_x, float strength_y);
	static float fgRangeRand(float min, float max);
protected:
    virtual void startWithTarget(cocos2d::Node *pTarget);
    virtual void update(float time);
    virtual void stop(void);

    // Initial position of the shaked node  
    float m_initial_x, m_initial_y;
    float m_strength_x, m_strength_y;
	Node *m_pTarget;
};


/**
 * ���Զ���(ʣ��ʱ��Խ�̣�������ΧԽС)
 */
class FallOffShake : public shake
{
public:
    FallOffShake();

    // Create the action with a time and a strength (same in x and y)  
    static FallOffShake* create(float d, float strength);
    // Create the action with a time and strengths (different in x and y)  
   static FallOffShake* createWithStrength(float d, float strength_x, float strength_y);

protected:
    virtual void update(float time);
};

#endif // !__CLASSES_SHAKE_H__

