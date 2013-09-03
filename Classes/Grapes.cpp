#include "Grapes.h"

Grapes::Grapes()
{
	_splurt = CCParticleSystemQuad::create("Particles/grapes_splurt.plist");
	_splurt->stopSystem();
}

PolygonSprite* Grapes::initWithWorld(b2World* world)
{
    int count = 6;
	String* strFile = String::create("grapes-ipadhd.png");

    b2Vec2 vertices[] = {
        b2Vec2(159/PTM_RATIO,242/PTM_RATIO),
        b2Vec2(91/PTM_RATIO,209/PTM_RATIO),
        b2Vec2(55/PTM_RATIO,167/PTM_RATIO),
        b2Vec2(77/PTM_RATIO,69/PTM_RATIO),
        b2Vec2(147/PTM_RATIO,12/PTM_RATIO),
        b2Vec2(209/PTM_RATIO,174/PTM_RATIO)
    };

	Size screen = Director::getInstance()->getWinSize();
 
	b2Body *body = createBodyForWorld(world, b2Vec2(screen.width/2/PTM_RATIO, screen.height/2/PTM_RATIO), 0, vertices, count, 5.0, 0.2, 0.2);
 
	if( initWithFile(strFile->getCString(), body, true) )
    {
        // We will initialize more values for the fruit here later
		_type = kTypeGrapes;
    }

    return this;
}