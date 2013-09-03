#include "TileMap.h"
#include "Tags.h"
#include "Box2dDirector.h"
#include "MainGameScene.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;

bool TileMapLayer::init()
{
	return true;
}

void TileMapLayer::createWithTMX(const char* tmxFile)
{
	// create a TMX map --- WARNING : if tsx path in the tmx file is subclass of Resources, libs can't find *.tsx, *.png
	map = TMXTiledMap::create(tmxFile);
	prepareLayers();
	addChild(map);

	// All the tiles by default will be aliased. If you want to create anti-alias tiles, you should do:
	// iterate over all the "layers" (atlas sprite managers)
	// and set them as 'antialias' 
	Array * pChildrenArray = map->getChildren();
	SpriteBatchNode* child = NULL;
	Object* pObject = NULL;

	CCARRAY_FOREACH(pChildrenArray, pObject)
	{
		child = (SpriteBatchNode*)pObject;

		if(!child)
			break;

		child->getTexture()->setAntiAliasTexParameters();
	}
}

void TileMapLayer::prepareLayers()
{
	Object* object = NULL;

	CCARRAY_FOREACH(map->getChildren(), object)
	{
		// is this map child a tile layer?
		TMXLayer* layer = dynamic_cast<TMXLayer*>(object);
		if( layer != nullptr )
			this->createFixtures(layer);
	}
}

void TileMapLayer::createFixtures(TMXLayer* layer)
{
	// create all the rectangular fixtures for each tile in the level
	Size layerSize = layer->getLayerSize();
	for( int y=0; y < layerSize.height; y++ )
	{
		for( int x=0; x < layerSize.width; x++ )
		{
			// create a fixture if this tile has a sprite
			Sprite* tileSprite = layer->tileAt(ccp(x, y));
			if( tileSprite )
			{
				this->createRectangularFixture(layer, x, y, 1.1f, 1.1f);
				log("createRectangularFixture(%d,%d)", x, y);
			}
		}
	}
}

void TileMapLayer::createRectangularFixture(TMXLayer* layer, int x, int y, float width, float height)
{
	// get position & size
	Point p = layer->positionAt(ccp(x,y));
	Size tileSize = this->map->getTileSize();

	// create the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set( (p.x + (tileSize.width / 2)) / PTM_RATIO,
		                  (p.y + (tileSize.height / 2)) / PTM_RATIO );
	//b2Body* body = world->CreateBody(&bodyDef);
	b2Body* body = (Box2dDirectorLayer::world)->CreateBody(&bodyDef);

	// define the shape
	b2PolygonShape shape;
	shape.SetAsBox( (tileSize.width / PTM_RATIO) * 0.5f * width,
					(tileSize.width / PTM_RATIO) * 0.5f * height );

	// create the fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;   
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.0f;
	//uint32 flags = 0;	//added
	//fixtureDef.filter.categoryBits = flags;
	//fixtureDef.filter.maskBits = 0xffff;
	body->CreateFixture(&fixtureDef);
}