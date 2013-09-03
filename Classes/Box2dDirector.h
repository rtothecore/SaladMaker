#ifndef __BOX2D_DIRECTOR_H__
#define __BOX2D_DIRECTOR_H__

#include "cocos2d.h"
#include "GLES-Render.h"
#include "Box2D/Box2D.h"
#include "cocos-ext.h"
#include "RaycastCallback.h"
#include "PolygonSplitter.h"
#include "FruitManager.h"

USING_NS_CC_EXT;
USING_NS_CC;

#define PTM_RATIO 32

class Box2dDirectorLayer : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(Box2dDirectorLayer);
	virtual void onExit();

	static b2World* world;
	GLESDebugDraw *m_debugDraw;

	void initPhysics();
	virtual void draw();
	void update(float dt);

	// slice
	static Point _startPoint;
	static Point _endPoint;
	
	// Raycast
	static RaycastCallback *_raycastCallback;
	static void calculateRaycast();

	//// Polygon Splitter
	//PolygonSplitter* polygonSplitter;

	//// fruits
	//FruitManager* fruitManager;

	// clear splice
};

#endif