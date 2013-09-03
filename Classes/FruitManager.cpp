#include "FruitManager.h"
#include "Watermelon.h"
#include "Strawberry.h"
#include "Pineapple.h"
#include "Grapes.h"
#include "Banana.h"
#include "Bomb.h"
#include "Box2dDirector.h"
#include "MainGameScene.h"
#include "UtilFunc.h"

bool FruitManager::init()
{
	_cache = NULL;
	_nextTossTime = UtilFunc::getTimeTick() + 1;
	_queuedForToss = 0;

	initSprites();

	return true;
}

void FruitManager::onExit()
{
	_cache->release();
	_cache = NULL;
}

void FruitManager::initSprites()
{
	_cache = Array::createWithCapacity(53);

	/*int xPos = -64;
	int yPos = -64;*/
	int xPos = 20;
	int yPos = 0;

    for (int i = 0; i < 10; i++)
    {
		PolygonSprite *sprite = (new Watermelon())->initWithWorld(Box2dDirectorLayer::world);
		sprite->setPosition( ccp(xPos*(i+1), yPos) );
		addChild(sprite, 1);
		_cache->addObject(sprite);

		// Particles
		addChild(sprite->_splurt, 3);
    }
	for (int i = 0; i < 10; i++)
	{
		PolygonSprite *sprite = (new Strawberry())->initWithWorld(Box2dDirectorLayer::world);
		sprite->setPosition( ccp(xPos*(i+1), yPos) );
		addChild(sprite, 1);
		_cache->addObject(sprite);

		// Particles
		addChild(sprite->_splurt, 3);
	}
	for (int i = 0; i < 10; i++)
	{
		PolygonSprite *sprite = (new Pineapple())->initWithWorld(Box2dDirectorLayer::world);
		sprite->setPosition( ccp(xPos*(i+1), yPos) );
		addChild(sprite, 1);
		_cache->addObject(sprite);
		
		// Particles
		addChild(sprite->_splurt, 3);
	}
	for (int i = 0; i < 10; i++)
	{
		PolygonSprite *sprite = (new Grapes())->initWithWorld(Box2dDirectorLayer::world);
		sprite->setPosition( ccp(xPos*(i+1), yPos) );
		addChild(sprite, 1);
		_cache->addObject(sprite);
		
		// Particles
		addChild(sprite->_splurt, 3);
	}
	for (int i = 0; i < 10; i++)
	{
		PolygonSprite *sprite = (new Banana())->initWithWorld(Box2dDirectorLayer::world);
		sprite->setPosition( ccp(xPos*(i+1), yPos) );
		addChild(sprite, 1);
		_cache->addObject(sprite);
		
		// Particles
		addChild(sprite->_splurt, 3);
	}

	for (int i = 0; i < 3; i++)
	{
		PolygonSprite *sprite = (new Bomb())->initWithWorld(Box2dDirectorLayer::world);
		sprite->setPosition( ccp(xPos*(i+1), yPos) );
		addChild(sprite, 1);
		_cache->addObject(sprite);
		
		// Particles
		addChild(sprite->_splurt, 3);
	}

	_cache->retain();
}

void FruitManager::tossSprite(PolygonSprite* sprite)
{
	Size screen = Director::getInstance()->getWinSize();

	//int startPosY = -64;
	int startPosY = 100;

    //Point randomPosition = ccp(frandom_range(100, screen.width-164), startPosY);
	Point randomPosition = ccp(frandom_range(300, screen.width), startPosY);
    float randomAngularVelocity = frandom_range((-1), 1);
 
    float xModifier = 50*(randomPosition.x - 100)/(screen.width - 264);
    float min = -25.0 - xModifier;
    float max = 75.0 - xModifier;
 
    float randomXVelocity = frandom_range(min,max);
    float randomYVelocity = frandom_range(250, 300);
 
    sprite->_state = kStateTossed;
	sprite->setPosition(randomPosition);
	sprite->activateCollisions();
    sprite->_body->SetLinearVelocity(b2Vec2(randomXVelocity/PTM_RATIO,randomYVelocity/PTM_RATIO));
    sprite->_body->SetAngularVelocity(randomAngularVelocity);
}

void FruitManager::spriteLoop()
{
	double curTime = UtilFunc::getTimeTick();

	//step 1
	if (curTime > _nextTossTime)
	{
		int random = random_range(0, 4);
		//step 2
		Type type = (Type)random;

		PolygonSprite *sprite;
		Object* pObject;

		// bomb
		if( 0 == (rand()%8) )
		{
			CCARRAY_FOREACH(_cache, pObject)
			{
				sprite = (PolygonSprite*)pObject;
				if (sprite->_state == kStateIdle && sprite->_type == kTypeBomb)
				{
					tossSprite(sprite);
					//SimpleAudioEngine::getInstance()->playEffect("Sounds/toss_bomb.caf");
					break;
				}
			}
		}

		// fruits
		if (_currentTossType == kTossConsecutive && _queuedForToss > 0)
		{
			CCARRAY_FOREACH(_cache, pObject)
			{
				sprite = (PolygonSprite*)pObject;
				if (sprite->_state == kStateIdle && sprite->_type == type)
				{
					tossSprite(sprite);
					_queuedForToss--;
					//SimpleAudioEngine::getInstance()->playEffect("Sounds/toss_consecutive.caf");
					break;
				}
			}
		}
		else
		{ //step 3
			_queuedForToss = random_range(3, 8);
			int tossType = random_range(0,1);

			_currentTossType = (TossType)tossType;
			//step 4
			if (_currentTossType == kTossSimultaneous)
			{
				CCARRAY_FOREACH(_cache, pObject)
				{
					sprite = (PolygonSprite*)pObject;
					if (sprite->_state == kStateIdle && sprite->_type == type)
					{
						tossSprite(sprite);
						_queuedForToss--;
						random = random_range(0, 4);
						type = (Type)random;

						//SimpleAudioEngine::getInstance()->playEffect("Sounds/toss_simultaneous.caf");

						if (_queuedForToss == 0)
						{
							break;
						}
					}
				}
			} //step 5
			else if (_currentTossType == kTossConsecutive)
			{
				CCARRAY_FOREACH(_cache, pObject)
				{
					sprite = (PolygonSprite*)pObject;
					if (sprite->_state == kStateIdle && sprite->_type == type)
					{
						tossSprite(sprite);
						_queuedForToss--;
						//SimpleAudioEngine::getInstance()->playEffect("Sounds/toss_consecutive.caf");
						break;
					}
				}
			}
		}
		//step 6
		if (_queuedForToss == 0)
		{
			_tossInterval = frandom_range(2,3);
			_nextTossTime = curTime + _tossInterval;
		}
		else 
		{
			_tossInterval = frandom_range(0.3,0.8);
			_nextTossTime = curTime + _tossInterval;
		}
	}
}

void FruitManager::cleanSprites()
{
    PolygonSprite *sprite;
	Object* pObject;

	//we check for all tossed sprites that have dropped offscreen and reset them
	CCARRAY_FOREACH(_cache, pObject)
	{
		sprite = (PolygonSprite*)pObject;

		if (sprite->_state == kStateTossed)
		{
			Point spritePosition = ccp(sprite->_body->GetPosition().x*PTM_RATIO, sprite->_body->GetPosition().y*PTM_RATIO);
			float yVelocity = sprite->_body->GetLinearVelocity().y;

			//this means the sprite has dropped offscreen
			if (spritePosition.y < -64 && yVelocity < 0)
			{
				log("SPRITES RESET!");
				sprite->_state = kStateIdle;
				sprite->_sliceEntered = false;
				sprite->_sliceExited = false;
				sprite->_entryPoint.SetZero();
				sprite->_exitPoint.SetZero();
				sprite->setPosition(ccp(-64, -64));
				sprite->_body->SetLinearVelocity(b2Vec2(0.0,0.0));
				sprite->_body->SetAngularVelocity(0.0);
				sprite->deactivateCollisions();

				// score system
				// If any original sprites fall to the bottom, then you subtract a life from the player
				if (sprite->_type != kTypeBomb)
				{
					((MainGameLayer*)getParent()->getParent())->subtractLife();					
				}
			}
		}
	}

	//we check for all sliced pieces that have dropped offscreen and remove them
	Size screen = Director::getInstance()->getWinSize();
	for (b2Body* b = Box2dDirectorLayer::world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != NULL) {
			PolygonSprite *sprite = (PolygonSprite*)b->GetUserData();
			Point position = ccp(b->GetPosition().x*PTM_RATIO, b->GetPosition().y*PTM_RATIO);
			if (position.x < -64 || position.x > screen.width || position.y < -64)
			{
				if (!sprite->_original)
				{
					// --------- FIX ME: Ghost sprite appeared when below line runs -----------
					//Box2dDirectorLayer::world->DestroyBody(sprite->_body);					
					removeChild(sprite);
					return;
				}
			}
		}
	}
}