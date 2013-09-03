#include "Watermelon.h"

Watermelon::Watermelon()
{
	_splurt = CCParticleSystemQuad::create("Particles/watermelon_splurt.plist");
	_splurt->stopSystem();
}

PolygonSprite* Watermelon::initWithWorld(b2World* world)
{
    int count = 7;
	String* strFile = String::create("watermelon-ipadhd.png");

    b2Vec2 vertices[] = {
        b2Vec2(235.0/PTM_RATIO,59.0/PTM_RATIO),
        b2Vec2(135.0/PTM_RATIO,235.0/PTM_RATIO),
        b2Vec2(116.0/PTM_RATIO,236.0/PTM_RATIO),
        b2Vec2(25.0/PTM_RATIO,64.0/PTM_RATIO),
        b2Vec2(63.0/PTM_RATIO,37.0/PTM_RATIO),
        b2Vec2(114.0/PTM_RATIO,24.0/PTM_RATIO),
        b2Vec2(179.0/PTM_RATIO,31.0/PTM_RATIO)
    };

	Size screen = Director::getInstance()->getWinSize();
 
	b2Body *body = createBodyForWorld(world, b2Vec2(screen.width/2/PTM_RATIO, screen.height/2/PTM_RATIO), 0, vertices, count, 5.0, 0.2, 0.2);
 
	if( initWithFile(strFile->getCString(), body, true) )
    {
        // We will initialize more values for the fruit here later
		_type = kTypeWatermelon;
    }

    return this;
}