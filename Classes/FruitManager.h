#ifndef __FRUIT_MANAGER_H__
#define __FRUIT_MANAGER_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "PolygonSprite.h"
#include "Box2dDirector.h"

USING_NS_CC;

typedef enum _TossType
{
	kTossConsecutive = 0,
	kTossSimultaneous
}TossType;

//random toss fruits
#define frandom (float)rand()/100000
#define frandom_range(low,high) ((high-low)*frandom)+low
#define random_range(low,high) (rand()%(high-low+1))+low

class FruitManager : public Layer
{
public:
	CREATE_FUNC(FruitManager);
	bool init();
	virtual void onExit();

	// fruits
	Array *_cache;
	void initSprites();

	// random toss fruits
	double _nextTossTime;
	double _tossInterval;
	int _queuedForToss;
	TossType _currentTossType;

	void tossSprite(PolygonSprite* sprite);
	void spriteLoop();
	void cleanSprites();	
};

#endif