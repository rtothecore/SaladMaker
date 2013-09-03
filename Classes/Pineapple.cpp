#include "Pineapple.h"

Pineapple::Pineapple()
{
	_splurt = CCParticleSystemQuad::create("Particles/pineapple_splurt.plist");
	_splurt->stopSystem();
}

PolygonSprite* Pineapple::initWithWorld(b2World* world)
{
    int count = 6;
	String* strFile = String::create("pineapple-ipadhd.png");

    b2Vec2 vertices[] = {
        b2Vec2(235/PTM_RATIO,248/PTM_RATIO),
        b2Vec2(171/PTM_RATIO,249/PTM_RATIO),
        b2Vec2(-6/PTM_RATIO,74/PTM_RATIO),
        b2Vec2(30/PTM_RATIO,19/PTM_RATIO),
        b2Vec2(89/PTM_RATIO,-2/PTM_RATIO),
        b2Vec2(255/PTM_RATIO,173/PTM_RATIO)
    };

	Size screen = Director::getInstance()->getWinSize();
 
	b2Body *body = createBodyForWorld(world, b2Vec2(screen.width/2/PTM_RATIO, screen.height/2/PTM_RATIO), 0, vertices, count, 5.0, 0.2, 0.2);
 
	if( initWithFile(strFile->getCString(), body, true) )
    {
        // We will initialize more values for the fruit here later
		_type = kTypePineapple;
    }

    return this;
}
