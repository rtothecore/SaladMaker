#include "Missionaries.h"
#include "Tags.h"
#include "MainGameScene.h"
#include "Box2dDirector.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;

bool MissionariesLayer::init()
{
	walkStep = 8;
	jumpStep = 10;

	setAnchorPoint( Point(0.5, 0) );
	initWithSprite();

	return true;
}

void MissionariesLayer::initWithSprite()
{
	Sprite* sprtGabriel = Sprite::create("gabriel.png");
	sprtGabriel->setAnchorPoint(Point(0, 0));
	addChild(sprtGabriel, 0, kTagSprtGabriel);
}

void MissionariesLayer::walk(bool toTheLeft)
{
	int walkStepWithD = walkStep;
	toTheLeft ? walkStepWithD = -walkStep : 0;

	auto walk = MoveBy::create( 0.25, Point(walkStepWithD, 0) );
	runAction(walk);
}

void MissionariesLayer::jump(bool toTheLeft)
{
	int jumpStepWithD = jumpStep;
	toTheLeft ? jumpStepWithD = -jumpStep : 0;

	Sprite* sprtGabriel = (Sprite*)getChildByTag(kTagSprtGabriel);
	auto *jump = JumpBy::create(0.25f, Point(jumpStepWithD, 0), sprtGabriel->getContentSize().height, 1);

	runAction(jump);
}

Size MissionariesLayer::getSpriteSize()
{
	return getChildByTag(kTagSprtGabriel)->getContentSize();
}