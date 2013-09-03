#include "PolygonSprite.h"

PolygonSprite::PolygonSprite()
: _body(NULL)
, _original(true)
, _centroid(b2Vec2_zero)
{}

PolygonSprite* PolygonSprite::spriteWithFile(const char* filename, b2Body* body, bool original)
{
	return initWithFile(filename, body, original);
}

PolygonSprite* PolygonSprite::spriteWithTexture(Texture2D* texture, b2Body* body, bool original)
{
	return initWithTexture(texture, body, original);
}

PolygonSprite* PolygonSprite::spriteWithWorld(b2World* world)
{
	return initWithWorld(world);
}

PolygonSprite* PolygonSprite::initWithFile(const char* filename, b2Body* body, bool original)
{
	CCAssert(filename, "Invalid filename for sprite");
	Texture2D *texture = TextureCache::sharedTextureCache()->addImage(filename);

	return initWithTexture(texture, body, original);
}

PolygonSprite* PolygonSprite::initWithTexture(Texture2D* texture, b2Body* body, bool original)
{
	//gather all the vertices from our Box2D shape
	b2Fixture *originalFixture = body->GetFixtureList();
	b2PolygonShape *shape = (b2PolygonShape*)originalFixture->GetShape();
	int vertexCount = shape->GetVertexCount();

	Vector2dVector points;

	for(int i = 0; i < vertexCount; i++) {
		points.push_back( Vector2d(shape->GetVertex(i).x * PTM_RATIO, shape->GetVertex(i).y * PTM_RATIO) );
	}

	if(initWithPointsAndTexture(points, texture))
	{
		_body = body;
		_body->SetUserData(this);
		_original = original;
		// gets the center of the polygon
		_centroid = this->_body->GetLocalCenter();
		// assign an anchor point based on the center
		_anchorPoint = ccp(_centroid.x * PTM_RATIO / texture->getContentSize().width, 
								 _centroid.y * PTM_RATIO / texture->getContentSize().height);

		// more init stuff here later when you expand PolygonSprite

		// slice
		_sliceExited = false;
		_sliceEntered = false;
		_entryPoint.SetZero();
		_exitPoint.SetZero();
		_sliceEntryTime = 0;

		// fruits
		_state = kStateIdle;
	}

	return this;
}

PolygonSprite* PolygonSprite::initWithWorld(b2World* world)
{
	return NULL;
}

void PolygonSprite::setPosition(Point position)
{
	PRFilledPolygon::setPosition(position);
	_body->SetTransform(b2Vec2(position.x/PTM_RATIO,position.y/PTM_RATIO), _body->GetAngle());
}

b2Body* PolygonSprite::createBodyForWorld(b2World* world, b2Vec2 position, float rotation, 
										  b2Vec2* vertices, int vertextCount, 
										  float density, float friction, float restitution)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = position;
	bodyDef.angle = rotation;
	b2Body *body = world->CreateBody(&bodyDef);

	b2FixtureDef fixtureDef;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;
	fixtureDef.filter.categoryBits = 0;
	fixtureDef.filter.maskBits = 0;

	fixtureDef.isSensor = true;

	b2PolygonShape shape;
	shape.Set(vertices, vertextCount);
	fixtureDef.shape = &shape;
	body->CreateFixture(&fixtureDef);

	return body;
}

void PolygonSprite::activateCollisions()
{
	b2Fixture *fixture = _body->GetFixtureList();
	b2Filter filter = fixture->GetFilterData();
	filter.categoryBits = 0x0001;
	filter.maskBits = 0x0001;
	fixture->SetFilterData(filter);
}

void PolygonSprite::deactivateCollisions()
{
	b2Fixture *fixture = _body->GetFixtureList();
    b2Filter filter = fixture->GetFilterData();
    filter.categoryBits = 0;
    filter.maskBits = 0;
    fixture->SetFilterData(filter);
}

AffineTransform PolygonSprite::getNodeToParentTransform() const
{
    // Although scale is not used by physics engines, it is calculated just in case
	// the sprite is animated (scaled up/down) using actions.
	// For more info see: http://www.cocos2d-iphone.org/forum/topic/68990

    b2Vec2 pos  = _body->GetPosition();

	float x = pos.x * PTM_RATIO;
	float y = pos.y * PTM_RATIO;

	if (_ignoreAnchorPointForPosition)
    {
		x += _anchorPointInPoints.x;
		y += _anchorPointInPoints.y;
	}

	// Make matrix
	float radians = _body->GetAngle();
	float c = cosf(radians);
	float s = sinf(radians);

	if (!_anchorPointInPoints.equals(Point::ZERO))
    {
		x += ((c * -_anchorPointInPoints.x * _scaleX) + (-s * -_anchorPointInPoints.y * _scaleY));
		y += ((s * -_anchorPointInPoints.x * _scaleX) + (c * -_anchorPointInPoints.y * _scaleY));
	}

	// Rot, Translate Matrix
	_transform = AffineTransformMake( c * _scaleX,	s * _scaleX,
                                     -s * _scaleY,	c * _scaleY,
                                     x,	y );

	return _transform;
}
