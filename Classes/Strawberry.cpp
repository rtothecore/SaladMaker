#include "Strawberry.h"

Strawberry::Strawberry()
{
	_splurt = CCParticleSystemQuad::create("Particles/strawberry_splurt.plist");
	_splurt->stopSystem();
}

PolygonSprite* Strawberry::initWithWorld(b2World* world)
{
    int count = 6;
	String* strFile = String::create("strawberry-ipadhd.png");

    b2Vec2 vertices[] = {
        b2Vec2(59/PTM_RATIO,85/PTM_RATIO),
        b2Vec2(158/PTM_RATIO,19/PTM_RATIO),
        b2Vec2(213/PTM_RATIO,44/PTM_RATIO),
        b2Vec2(213/PTM_RATIO,166/PTM_RATIO),
        b2Vec2(123/PTM_RATIO,233/PTM_RATIO),
        b2Vec2(62/PTM_RATIO,209/PTM_RATIO)
    };

	Size screen = Director::getInstance()->getWinSize();
 
	b2Body *body = createBodyForWorld(world, b2Vec2(screen.width/2/PTM_RATIO, screen.height/2/PTM_RATIO), 0, vertices, count, 5.0, 0.2, 0.2);
 
	if( initWithFile(strFile->getCString(), body, true) )
    {
        // We will initialize more values for the fruit here later
		_type = kTypeStrawberry;
    }

    return this;
}