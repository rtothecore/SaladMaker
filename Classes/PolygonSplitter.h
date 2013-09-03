#ifndef __POLYGON_SPLITTER_H__
#define __POLYGON_SPLITTER_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "PolygonSprite.h"
#include "Box2dDirector.h"

USING_NS_CC;

#define calculate_determinant_2x2(x1,y1,x2,y2) x1*y2-y1*x2
#define calculate_determinant_2x3(x1,y1,x2,y2,x3,y3) x1*y2+x2*y3+x3*y1-y1*x2-y2*x3-y3*x1

//reset & cleanup fruits
#define midpoint(a,b) (float)(a+b)/2

#define PTM_RATIO 32

class PolygonSplitter : public Layer
{
public:
	CREATE_FUNC(PolygonSplitter);
	virtual bool init();

	b2Vec2* arrangeVertices(b2Vec2* vertices, int count);
	void splitPolygonSprite(PolygonSprite* sprite);
	bool areVerticesAcceptable(b2Vec2* vertices, int count);
	b2Body* createBodyWithPosition(b2Vec2 position, float rotation, 
								   b2Vec2* vertices, int vertexCount, 
								   float density, float friction, float restitution);
	void checkAndSliceObjects();
	//static void clearSlices();
};

#endif