#ifndef __MISSIONARIES_H__
#define __MISSIONARIES_H__

#include "cocos2d.h"

USING_NS_CC;

class MissionariesLayer : public Layer
{
public:
	CREATE_FUNC(MissionariesLayer);
	virtual bool init();

	void initWithSprite();
	Size getSpriteSize();

	// move
	unsigned int walkStep;
	unsigned int jumpStep;

	void walk(bool toTheLeft);
	void jump(bool toTheLeft);
};

#endif