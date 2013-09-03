#ifndef __BANANA_H__
#define __BANANA_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "PolygonSprite.h"

USING_NS_CC;

#define PTM_RATIO 32

class Banana : public PolygonSprite
{
public:
	Banana();

	PolygonSprite* initWithWorld(b2World* world);
};

#endif
