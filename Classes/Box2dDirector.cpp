#include "Box2dDirector.h"
#include "VisibleRect.h"
#include "MainGameScene.h"
#include "UtilFunc.h"
#include "Tags.h"

b2World* Box2dDirectorLayer::world;
Point Box2dDirectorLayer::_startPoint;
Point Box2dDirectorLayer::_endPoint;
RaycastCallback* Box2dDirectorLayer::_raycastCallback;

bool Box2dDirectorLayer::init()
{
	setAccelerometerEnabled( true );

	initPhysics();

	// polygon sprite
	FruitManager* fruitManager = FruitManager::create();
	addChild(fruitManager, 0, kTagLayerFruitManager);

	// polygon splitter
	PolygonSplitter* polygonSplitter = PolygonSplitter::create();
	addChild(polygonSplitter, 0, kTagLayerPolygonSplitter);

	scheduleUpdate();

	_raycastCallback = new RaycastCallback();

	return true;
}

void Box2dDirectorLayer::onExit()
{
}

void Box2dDirectorLayer::initPhysics()
{
	b2Vec2 gravity;
    //gravity.Set(0.0f, -10.0f);
	gravity.Set(0.0f, -4.25f);
    world = new b2World(gravity);

    // Do we want to let bodies sleep?
    world->SetAllowSleeping(true);
    world->SetContinuousPhysics(true);

	/*m_debugDraw = new GLESDebugDraw( PTM_RATIO );
	world->SetDebugDraw(m_debugDraw);*/

	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	//		flags += b2Draw::e_jointBit;
	//		flags += b2Draw::e_aabbBit;
	//		flags += b2Draw::e_pairBit;
	//		flags += b2Draw::e_centerOfMassBit;

	//m_debugDraw->SetFlags(flags);	

    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world->CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2EdgeShape groundBox;

    //// bottom
    //groundBox.Set(b2Vec2(VisibleRect::leftBottom().x/PTM_RATIO,VisibleRect::leftBottom().y/PTM_RATIO), b2Vec2(VisibleRect::rightBottom().x/PTM_RATIO,VisibleRect::rightBottom().y/PTM_RATIO));
    //groundBody->CreateFixture(&groundBox,0);

    //// top
    //groundBox.Set(b2Vec2(VisibleRect::leftTop().x/PTM_RATIO,VisibleRect::leftTop().y/PTM_RATIO), b2Vec2(VisibleRect::rightTop().x/PTM_RATIO,VisibleRect::rightTop().y/PTM_RATIO));
    //groundBody->CreateFixture(&groundBox,0);

    //// left
    //groundBox.Set(b2Vec2(VisibleRect::leftTop().x/PTM_RATIO,VisibleRect::leftTop().y/PTM_RATIO), b2Vec2(VisibleRect::leftBottom().x/PTM_RATIO,VisibleRect::leftBottom().y/PTM_RATIO));
    //groundBody->CreateFixture(&groundBox,0);

    //// right
    //groundBox.Set(b2Vec2(VisibleRect::rightBottom().x/PTM_RATIO,VisibleRect::rightBottom().y/PTM_RATIO), b2Vec2(VisibleRect::rightTop().x/PTM_RATIO,VisibleRect::rightTop().y/PTM_RATIO));
    //groundBody->CreateFixture(&groundBox,0);
}

void Box2dDirectorLayer::draw()
{
    //
    // IMPORTANT:
    // This is only for debug purposes
    // It is recommend to disable it
    //
    Layer::draw();

    GL::enableVertexAttribs( cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION );

    kmGLPushMatrix();

	//slice
	/*ccDrawLine(_startPoint, _endPoint);

    world->DrawDebugData();*/

    kmGLPopMatrix();
}

void Box2dDirectorLayer::update(float dt)
{
    //It is recommended that a fixed time step is used with Box2D for stability
    //of the simulation, however, we are using a variable time step here.
    //You need to make an informed choice, the following URL is useful
    //http://gafferongames.com/game-physics/fix-your-timestep/
    
    int velocityIterations = 8;
    int positionIterations = 1;

    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    world->Step(dt, velocityIterations, positionIterations);

	((PolygonSplitter*)getChildByTag(kTagLayerPolygonSplitter))->checkAndSliceObjects();
	((FruitManager*)getChildByTag(kTagLayerFruitManager))->cleanSprites();	
	((FruitManager*)getChildByTag(kTagLayerFruitManager))->spriteLoop();

	// sounds
	((MainGameLayer*)getParent())->_timeCurrent += dt;

}

void Box2dDirectorLayer::calculateRaycast()
{
	if (ccpLengthSQ(ccpSub(_startPoint, _endPoint)) > 25)
	{
		world->RayCast(_raycastCallback, 
			b2Vec2(_startPoint.x / PTM_RATIO, _startPoint.y / PTM_RATIO),
			b2Vec2(_endPoint.x / PTM_RATIO, _endPoint.y / PTM_RATIO));

		world->RayCast(_raycastCallback, 
			b2Vec2(_endPoint.x / PTM_RATIO, _endPoint.y / PTM_RATIO),
			b2Vec2(_startPoint.x / PTM_RATIO, _startPoint.y / PTM_RATIO));
		_startPoint = _endPoint;
	}

}
