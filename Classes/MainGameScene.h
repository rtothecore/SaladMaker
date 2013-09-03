#ifndef __MAINGAME_SCENE_H__
#define __MAINGAME_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "CCBlade.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

class MainGameScene : public Scene
{
public:
	virtual bool init();
	CREATE_FUNC(MainGameScene);
};

class MainGameLayer : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(MainGameLayer);

	void addMissionaries();
	void addBox(Point p);

	// check touch holding 
	bool isLeftTouched;
	void checkTouchHold(float dt);

	// check swipe
	bool isSwipeStart;
	int iSwipeStartY;
	int iSwipeEndY;
	void checkSwipe(float dt);

	virtual bool ccTouchBegan(Touch* touch, Event* event);
    virtual void ccTouchMoved(Touch* touch, Event* event);
    virtual void ccTouchEnded(Touch* touch, Event* event);

	// sprite
	void initBackground();

	// blade
	std::map< cocos2d::CCTouch *, CCBlade * >  _map;

	// scoring system
	int _cuts;
	int _lives;
	LabelTTF *_cutsLabel;

	void initHUD();
	void restart(Object* pSender);
	void endGame();
	void subtractLife();

	// particles
	CCParticleSystemQuad *_bladeSparkle;

	// sounds
	float _timeCurrent;
	float _timePrevious;
};

#endif 
