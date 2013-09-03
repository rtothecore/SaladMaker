#ifndef __STRAWBERRY_H__
#define __STRAWBERRY_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "PolygonSprite.h"

USING_NS_CC;

#define PTM_RATIO 32

class Strawberry : public PolygonSprite
{
public:
	Strawberry();

	PolygonSprite* initWithWorld(b2World* world);
};

#endif