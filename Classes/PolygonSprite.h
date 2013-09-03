#ifndef __POLYGON_SPRITE_H__
#define __POLYGON_SPRITE_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "PRFilledPolygon.h"

USING_NS_CC;

#define PTM_RATIO 32

typedef enum _State
{
	kStateIdle = 0,
	kStateTossed
} State;

typedef enum _Type
{
	kTypeWatermelon = 0,
	kTypeStrawberry,
	kTypePineapple,
	kTypeGrapes,
	kTypeBanana,
	kTypeBomb
} Type;

class PolygonSprite : public PRFilledPolygon
{
public:
	PolygonSprite();

	b2Body *_body;
	bool _original;
	b2Vec2 _centroid;

	virtual PolygonSprite* initWithTexture(Texture2D* texture, b2Body* body, bool original);
	virtual PolygonSprite* initWithFile(const char* filename, b2Body* body, bool original);
	PolygonSprite* spriteWithFile(const char* filename, b2Body* body, bool original);
	PolygonSprite* spriteWithTexture(Texture2D* texture, b2Body* body, bool original);

	virtual PolygonSprite* initWithWorld(b2World* world);
	PolygonSprite* spriteWithWorld(b2World* world);

	virtual b2Body* createBodyForWorld(b2World* world, b2Vec2 position, float rotation, 
		                               b2Vec2* vertices, int vertextCount, 
									   float density, float friction, float restitution);

	virtual void activateCollisions();
	virtual void deactivateCollisions();

	virtual void setPosition(Point position);
	virtual AffineTransform getNodeToParentTransform() const override;

	// slice
	bool _sliceEntered;
	bool _sliceExited;
	b2Vec2 _entryPoint;
	b2Vec2 _exitPoint;
	double _sliceEntryTime;

	// fruits
	State _state;
	Type _type;

	// Particles
	CCParticleSystemQuad *_splurt;
};

#endif