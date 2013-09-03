#ifndef __PINEAPPLE_H__
#define __PINEAPPLE_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "PolygonSprite.h"

USING_NS_CC;

#define PTM_RATIO 32

class Pineapple : public PolygonSprite
{
public:
	Pineapple();

	PolygonSprite* initWithWorld(b2World* world);
};

#endif