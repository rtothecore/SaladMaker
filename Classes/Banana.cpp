#include "Banana.h"

Banana::Banana()
{
	_splurt = CCParticleSystemQuad::create("Particles/banana_splurt.plist");
	_splurt->stopSystem();
}

PolygonSprite* Banana::initWithWorld(b2World* world)
{
    int count = 5;
	String* strFile = String::create("banana-ipadhd.png");

    b2Vec2 vertices[] = {
        b2Vec2(107/PTM_RATIO,25/PTM_RATIO),
        b2Vec2(204/PTM_RATIO,72/PTM_RATIO),
        b2Vec2(235/PTM_RATIO,151/PTM_RATIO),
        b2Vec2(208/PTM_RATIO,235/PTM_RATIO),
        b2Vec2(18/PTM_RATIO,59/PTM_RATIO)
    };

	Size screen = Director::getInstance()->getWinSize();
 
	b2Body *body = createBodyForWorld(world, b2Vec2(screen.width/2/PTM_RATIO, screen.height/2/PTM_RATIO), 0, vertices, count, 5.0, 0.2, 0.2);
 
	if( initWithFile(strFile->getCString(), body, true) )
    {
        // We will initialize more values for the fruit here later
		_type = kTypeBanana;
    }

    return this;
}