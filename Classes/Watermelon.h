#ifndef __WATERMELON_H__
#define __WATERMELON_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "PolygonSprite.h"

USING_NS_CC;

#define PTM_RATIO 32

class Watermelon : public PolygonSprite
{
public:
	Watermelon();

	PolygonSprite* initWithWorld(b2World* world);
};

#endif