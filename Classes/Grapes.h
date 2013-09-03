#ifndef __GRAPES_H__
#define __GRAPES_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "PolygonSprite.h"

USING_NS_CC;

#define PTM_RATIO 32

class Grapes : public PolygonSprite
{
public:
	Grapes();

	PolygonSprite* initWithWorld(b2World* world);
};

#endif