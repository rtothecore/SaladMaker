#ifndef __BOMB_H__
#define __BOMB_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "PolygonSprite.h"

USING_NS_CC;

#define PTM_RATIO 32

class Bomb : public PolygonSprite
{
public:
	Bomb();

	PolygonSprite* initWithWorld(b2World* world);
};

#endif