#ifndef __TILEMAP_H__
#define __TILEMAP_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"

USING_NS_CC;

class TileMapLayer : public Layer
{
public:
	CREATE_FUNC(TileMapLayer);
	virtual bool init();

	CCTMXTiledMap *map;

	void createWithTMX(const char* tmxFile);
	void prepareLayers();

	// box2d
	void createFixtures(CCTMXLayer* layer);
	void createRectangularFixture(CCTMXLayer* layer, int x, int y, float width, float height);
};

#endif