#include "MainGameScene.h"
#include "Tags.h"
#include "Missionaries.h"
#include "TileMap.h"
#include "Box2dDirector.h"
#include "PolygonSprite.h"
#include "Watermelon.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;

//#define PTM_RATIO 32

//------------------------------------------------------------------
//
// MainGameScene
//
//------------------------------------------------------------------
bool MainGameScene::init()
{
	MainGameLayer* layer = MainGameLayer::create();
	addChild(layer);

	return true;
}

//------------------------------------------------------------------
//
// MainGameLayer
//
//------------------------------------------------------------------

bool MainGameLayer::init()
{
	isLeftTouched = false;
	isSwipeStart = false;
	iSwipeStartY = 0;
	iSwipeEndY = 0;

	// Touch
	Director* director = Director::getInstance();
    director->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

	// box2d physics
	Box2dDirectorLayer* box2dDL = Box2dDirectorLayer::create();
	addChild(box2dDL, 1, kTagLayerBox2dD);

	// background
	initBackground();

	// HUD
	initHUD();

	// Particles
	_bladeSparkle = ParticleSystemQuad::create("Particles/blade_sparkle.plist");
	_bladeSparkle->stopSystem();
	addChild(_bladeSparkle, 3);

	// Sounds
	SimpleAudioEngine::getInstance()->preloadEffect("Sounds/swoosh.caf");
	SimpleAudioEngine::getInstance()->preloadEffect("Sounds/squash.caf");
	SimpleAudioEngine::getInstance()->preloadEffect("Sounds/toss_consecutive.caf");
	SimpleAudioEngine::getInstance()->preloadEffect("Sounds/toss_simultaneous.caf");
	SimpleAudioEngine::getInstance()->preloadEffect("Sounds/toss_bomb.caf");
	SimpleAudioEngine::getInstance()->preloadEffect("Sounds/lose_life.caf");
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Sounds/nature_bgm.aifc");
	_timeCurrent = 0;

	//// TMX
	/*TileMapLayer* tilemapL = TileMapLayer::create();
	addChild(tilemapL, 1, kTagTileMap);
	tilemapL->createWithTMX("grassMap.tmx");	*/

	//addMissionaries();

	// polygon sprite
	/*PolygonSprite *sprite = (new Watermelon())->initWithWorld(box2dDL->world);
	addChild(sprite, 1);
	sprite->activateCollisions();*/

	// Raycast
	//_raycastCallback = new RaycastCallback();

	return true;
}

void MainGameLayer::initBackground()
{
	Size screen = Director::getInstance()->getWinSize();
	Sprite* background = Sprite::create("bg-hd.png");
	background->setPosition( Point(screen.width/2, screen.height/2) );
	addChild(background, 0);

	// Particles
	CCParticleSystemQuad *sunPollen = CCParticleSystemQuad::create("Particles/sun_pollen.plist");
	addChild(sunPollen);
}

void MainGameLayer::addMissionaries()
{
	MissionariesLayer* missionariesL = MissionariesLayer::create();
	addChild(missionariesL, 1, kTagLyrMissionaries);
	missionariesL->setPosition(30, 200);
}

void MainGameLayer::addBox(Point p)
{
	// Define the dynamic body.
    //Set up a 1m squared box in the physics world
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x/PTM_RATIO, p.y/PTM_RATIO);

    b2Body *body = (Box2dDirectorLayer::world)->CreateBody(&bodyDef);
    
    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    //dynamicBox.SetAsBox(.5f, .5f);//These are mid points for our 1m box
	dynamicBox.SetAsBox(.5f, .8f);//These are mid points for our 1m box
    
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;   
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);    

	// body to sprite
	Texture2D *texture = TextureCache::getInstance()->addImage("gabriel.png");
	PhysicsSprite *sprite = PhysicsSprite::createWithTexture(texture);
    addChild(sprite);
    sprite->setB2Body(body);
    sprite->setPTMRatio(PTM_RATIO);
    sprite->setPosition( Point( p.x, p.y) );
}

bool MainGameLayer::ccTouchBegan(Touch* touch, Event* event)
{
	//Size winSize = Director::getInstance()->getWinSize();

	//if(winSize.width/2 < touch->getLocation().x)	// screen right click
	//{
	//	isLeftTouched = false;
	//} 
	//else
	//{
	//	isLeftTouched = true;
	//}

	//schedule(schedule_selector(MainGameLayer::checkTouchHold), 0.1f);

	Point location = touch->getLocationInView();
	location = Director::getInstance()->convertToGL(location);
	Box2dDirectorLayer::_startPoint = location;
	Box2dDirectorLayer::_endPoint = location;

	// blade
	CCBlade *blade = CCBlade::create("streak-hd.png", 10, 70);
	_map[touch] = blade;
	addChild(blade);

	blade->setColor(ccc3(255,0,0));
	blade->setOpacity(100);
	blade->setDrainInterval(1.0/40);

	CCPoint point = convertTouchToNodeSpace(touch);
	blade->push(point);

	// Particles
	_bladeSparkle->setPosition(location);
	_bladeSparkle->resetSystem();

    return true;
}

void MainGameLayer::checkTouchHold(float dt)
{
	MissionariesLayer* missionariesL = (MissionariesLayer*)getChildByTag(kTagLyrMissionaries);

	if(isLeftTouched)
	{
		log("move to left!");	

		//missionariesL->walk(true);
	}
	else
	{
		log("move to right!");

		//missionariesL->walk(false);
	}
}

void MainGameLayer::ccTouchMoved(Touch* touch, Event* event)
{
	/*if(!isSwipeStart)
	{
		iSwipeStartY = touch->getLocation().y;
		isSwipeStart = true;

		schedule(schedule_selector(MainGameLayer::checkSwipe), 0.1f);
	}

	if(isSwipeStart)
		iSwipeEndY = touch->getLocation().y;*/

	Point location = touch->getLocationInView();
	location = Director::getInstance()->convertToGL(location);
	Box2dDirectorLayer::_endPoint = location;

	// Raycast
	Box2dDirectorLayer::calculateRaycast();

	// sounds
	/*float deltaTime = _timeCurrent - _timePrevious;
	_timePrevious = _timeCurrent;
	Point oldPosition = _bladeSparkle->getPosition();*/

	// blade
	if (_map.find(touch) == _map.end()) 
		return;
	CCBlade *blade = _map[touch];
	CCPoint point = convertTouchToNodeSpace(touch);
	point = ccpAdd(ccpMult(point, 0.5f), ccpMult(touch->getPreviousLocation(), 0.5f));
	blade->push(point);

	// Particles
	_bladeSparkle->setPosition(location);
}

void MainGameLayer::checkSwipe(float dt)
{
	MissionariesLayer* missionariesL = (MissionariesLayer*)getChildByTag(kTagLyrMissionaries);

	if( iSwipeStartY < iSwipeEndY 
		&& 20 < (iSwipeEndY - iSwipeStartY) )
	{
		log("Jump!");
		if(isLeftTouched)
		{
			//missionariesL->jump(true);
		}
		else
		{
			//missionariesL->jump(false);
		}

		// stop move
		unschedule(schedule_selector(MainGameLayer::checkTouchHold));

		// stop check swipe
		unschedule(schedule_selector(MainGameLayer::checkSwipe));
	}
}

void MainGameLayer::ccTouchEnded(Touch* touch, Event* event)
{
	//// stop move
	//unschedule(schedule_selector(MainGameLayer::checkTouchHold));
	//log("stop move!");

	//// stop check swipe
	//isSwipeStart = false;
	//unschedule(schedule_selector(MainGameLayer::checkSwipe));

	//// createbox
	//addBox( Point(touch->getLocation().x, touch->getLocation().y) );

	((FruitManager*)(getChildByTag(kTagLayerBox2dD)->getChildByTag(kTagLayerFruitManager)))->cleanSprites();

	// blade
	if (_map.find(touch) == _map.end()) 
		return;
	CCBlade *blade = _map[touch];
	blade->autoCleanup();
	_map.erase(touch);

	// Particles
	_bladeSparkle->stopSystem();
}

void MainGameLayer::initHUD()
{
	Size screen = Director::getInstance()->getWinSize();
 
    _cuts = 0;
    _lives = 3;
 
    for (int i = 0; i < 3; i++)
    {
		Sprite *cross = Sprite::create("x_unfilled-hd.png");
		cross->setPosition( ccp(screen.width - cross->getContentSize().width/2 - i*cross->getContentSize().width, 
							    screen.height - cross->getContentSize().height/2) );
		addChild(cross, 4);
    }
 
    Sprite *cutsIcon = Sprite::create("fruit_cut-hd.png");
	cutsIcon->setPosition( ccp(cutsIcon->getContentSize().width/2, screen.height - cutsIcon->getContentSize().height/2) );
	addChild(cutsIcon);
 
	_cutsLabel = LabelTTF::create("0", "Helvetica Neue", 30);
    _cutsLabel->setAnchorPoint(ccp(0, 0.5));
	_cutsLabel->setPosition( ccp(cutsIcon->getPositionX() + cutsIcon->getContentSize().width/2 + _cutsLabel->getContentSize().width/2,
								 cutsIcon->getPositionY()));
	addChild(_cutsLabel, 4);
}
 
void MainGameLayer::restart(Object* pSender)
{
	MainGameScene* scene = MainGameScene::create();
	Director::getInstance()->replaceScene(scene);
}
 
void MainGameLayer::endGame()
{
	unscheduleUpdate();

	MenuItemLabel *label = MenuItemLabel::create(LabelTTF::create("RESTART", "Helvetica Neue", 50), CC_CALLBACK_1(MainGameLayer::restart, this));
    Menu *menu = Menu::create(label, NULL);
	Size screen = Director::getInstance()->getWinSize();
	menu->setPosition( ccp(screen.width/2, screen.height/2) );
	addChild(menu, 4);
}
 
void MainGameLayer::subtractLife()
{
    Size screen = Director::getInstance()->getWinSize();
    _lives--;
    Sprite *lostLife = Sprite::create("x_filled-hd.png");
	lostLife->setPosition( ccp(screen.width - lostLife->getContentSize().width/2 - _lives*lostLife->getContentSize().width, 
							   screen.height - lostLife->getContentSize().height/2) );
	addChild(lostLife, 4);

	SimpleAudioEngine::getInstance()->playEffect("Sounds/lose_life.caf");
 
    if (_lives <= 0)
    {
        endGame();
    }
}