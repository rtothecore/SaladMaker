#include "Bomb.h"

Bomb::Bomb()
{
	_splurt = CCParticleSystemQuad::create("Particles/explosion.plist");
	_splurt->stopSystem();
}

PolygonSprite* Bomb::initWithWorld(b2World* world)
{
    int count = 7;
	String* strFile = String::create("bomb-ipadhd.png");

    b2Vec2 vertices[] = {
        b2Vec2(130/PTM_RATIO,38/PTM_RATIO),
        b2Vec2(198/PTM_RATIO,75/PTM_RATIO),
        b2Vec2(208/PTM_RATIO,146/PTM_RATIO),
        b2Vec2(149/PTM_RATIO,217/PTM_RATIO),
        b2Vec2(110/PTM_RATIO,215/PTM_RATIO),
        b2Vec2(52/PTM_RATIO,146/PTM_RATIO),
		b2Vec2(64/PTM_RATIO,75/PTM_RATIO)
    };

	Size screen = Director::getInstance()->getWinSize();
 
	b2Body *body = createBodyForWorld(world, b2Vec2(screen.width/2/PTM_RATIO, screen.height/2/PTM_RATIO), 0, vertices, count, 5.0, 0.2, 0.2);
 
	if( initWithFile(strFile->getCString(), body, true) )
    {
        // We will initialize more values for the fruit here later
		_type = kTypeBomb;
    }

    return this;
}