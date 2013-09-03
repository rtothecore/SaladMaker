#include "PolygonSplitter.h"
#include "UtilFunc.h"
#include "MainGameScene.h"
#include "Box2dDirector.h"

bool PolygonSplitter::init()
{
	return true;
}

int comparator(const void *a, const void *b) {
    const b2Vec2 *va = (const b2Vec2 *)a;
    const b2Vec2 *vb = (const b2Vec2 *)b;
 
    if (va->x > vb->x) {
        return 1;
    } else if (va->x < vb->x) {
        return -1;
    }
    return 0;    
}

b2Vec2* PolygonSplitter::arrangeVertices(b2Vec2* vertices, int count)
{
	float determinant;
	int iCounterClockWise = 1;
	int iClockWise = count - 1;
	int i;

	b2Vec2 referencePointA, referencePointB;
	b2Vec2 *sortedVertices = (b2Vec2*)calloc(count, sizeof(b2Vec2));

	//sort all vertices in ascending order according to their x-coordinate so you can get two points of a line
	qsort(vertices, count, sizeof(b2Vec2), comparator);

	sortedVertices[0] = vertices[0];
	referencePointA = vertices[0];          //leftmost point
	referencePointB = vertices[count-1];    //rightmost point

	//you arrange the points by filling our vertices in both clockwise and counter-clockwise directions using the determinant function
	for (i=1; i<count-1; i++)
	{
		determinant = calculate_determinant_2x3(referencePointA.x, referencePointA.y, referencePointB.x, 
			                                    referencePointB.y, vertices[i].x, vertices[i].y);
		if (determinant<0)
		{
			sortedVertices[iCounterClockWise++] = vertices[i];
		}
		else 
		{
			sortedVertices[iClockWise--] = vertices[i];
		}//endif
	}//endif

	sortedVertices[iCounterClockWise] = vertices[count-1];
	return sortedVertices;
}

void PolygonSplitter::splitPolygonSprite(PolygonSprite* sprite)
{
	//declare & initialize variables to be used for later
    PolygonSprite *newSprite1, *newSprite2;
 
    //our original shape's attributes
    b2Fixture *originalFixture = sprite->_body->GetFixtureList();
    b2PolygonShape *originalPolygon = (b2PolygonShape*)originalFixture->GetShape();
    int vertexCount = originalPolygon->GetVertexCount();
 
    //our determinant(to be described later) and iterator
    float determinant;
    int i;
 
    //you store the vertices of our two new sprites here
    b2Vec2 *sprite1Vertices = (b2Vec2*)calloc(24, sizeof(b2Vec2));
    b2Vec2 *sprite2Vertices = (b2Vec2*)calloc(24, sizeof(b2Vec2));
    b2Vec2 *sprite1VerticesSorted, *sprite2VerticesSorted;
 
    //you store how many vertices there are for each of the two new sprites here
    int sprite1VertexCount = 0;
    int sprite2VertexCount = 0;
 
    //step 1:
    //the entry and exit point of our cut are considered vertices of our two new shapes, so you add these before anything else
    sprite1Vertices[sprite1VertexCount++] = sprite->_entryPoint;
    sprite1Vertices[sprite1VertexCount++] = sprite->_exitPoint;
    sprite2Vertices[sprite2VertexCount++] = sprite->_entryPoint;
    sprite2Vertices[sprite2VertexCount++] = sprite->_exitPoint;
 
    //step 2:
    //iterate through all the vertices and add them to each sprite's shape
    for (i=0; i<vertexCount; i++)
    {
        //get our vertex from the polygon
        b2Vec2 point = originalPolygon->GetVertex(i);
 
        //you check if our point is not the same as our entry or exit point first
        b2Vec2 diffFromEntryPoint = point - sprite->_entryPoint;
        b2Vec2 diffFromExitPoint = point - sprite->_exitPoint;
 
        if ((diffFromEntryPoint.x == 0 && diffFromEntryPoint.y == 0) || (diffFromExitPoint.x == 0 && diffFromExitPoint.y == 0))
        {
        }
        else 
        {
            determinant = calculate_determinant_2x3(sprite->_entryPoint.x, sprite->_entryPoint.y, 
				                                    sprite->_exitPoint.x, sprite->_exitPoint.y, point.x, point.y);
 
            if (determinant > 0)
            {
                //if the determinant is positive, then the three points are in clockwise order
                sprite1Vertices[sprite1VertexCount++] = point;
            }
            else
            {
                //if the determinant is 0, the points are on the same line. if the determinant is negative, then they are in counter-clockwise order
                sprite2Vertices[sprite2VertexCount++] = point;
 
            }//endif
        }//endif
    }//endfor
 
    //step 3:
    //Box2D needs vertices to be arranged in counter-clockwise order so you reorder our points using a custom function
	sprite1VerticesSorted = arrangeVertices(sprite1Vertices, sprite1VertexCount);
	sprite2VerticesSorted = arrangeVertices(sprite2Vertices, sprite2VertexCount);
 
    //step 4:
    //Box2D has some restrictions with defining shapes, so you have to consider these. You only cut the shape if both shapes pass certain requirements from our function
	bool sprite1VerticesAcceptable = areVerticesAcceptable(sprite1VerticesSorted, sprite1VertexCount);
	bool sprite2VerticesAcceptable = areVerticesAcceptable(sprite2VerticesSorted, sprite2VertexCount);
 
    //step 5:
    //you destroy the old shape and create the new shapes and sprites
    if (sprite1VerticesAcceptable && sprite2VerticesAcceptable)
    {
		// two pieces don't stick toghether when polygon splits
		b2Vec2 worldEntry = sprite->_body->GetWorldPoint(sprite->_entryPoint);
		b2Vec2 worldExit = sprite->_body->GetWorldPoint(sprite->_exitPoint);
		float angle = ccpToAngle(ccpSub(ccp(worldExit.x,worldExit.y), ccp(worldEntry.x,worldEntry.y)));
		Point vector1 = ccpForAngle(angle + 1.570796);
		Point vector2 = ccpForAngle(angle - 1.570796);
		float midX = midpoint(worldEntry.x, worldExit.x);
		float midY = midpoint(worldEntry.y, worldExit.y);

        //create the first sprite's body        
		b2Body *body1 = createBodyWithPosition(sprite->_body->GetPosition(), sprite->_body->GetAngle(), 
			                                   sprite1VerticesSorted, sprite1VertexCount,
			                                   originalFixture->GetDensity(), originalFixture->GetFriction(), originalFixture->GetRestitution());
 
        //create the first sprite
		newSprite1 = (new PolygonSprite())->spriteWithTexture(sprite->getTexture(), body1, false);
		addChild(newSprite1, 1);
		newSprite1->_body->ApplyLinearImpulse(b2Vec2(2*body1->GetMass()*vector1.x,2*body1->GetMass()*vector1.y), b2Vec2(midX,midY));
 
        //create the second sprite's body
		b2Body *body2 = createBodyWithPosition(sprite->_body->GetPosition(), sprite->_body->GetAngle(), 
			                                   sprite2VerticesSorted, sprite2VertexCount,
			                                   originalFixture->GetDensity(), originalFixture->GetFriction(), originalFixture->GetRestitution());
 
        //create the second sprite
		newSprite2 = (new PolygonSprite())->spriteWithTexture(sprite->getTexture(), body2, false);
		addChild(newSprite2, 1);
		newSprite2->_body->ApplyLinearImpulse(b2Vec2(2*body2->GetMass()*vector2.x,2*body2->GetMass()*vector2.y), b2Vec2(midX,midY));
 
        //you don't need the old shape & sprite anymore so you either destroy it or squirrel it away
        if (sprite->_original)
        {   
			sprite->_state = kStateIdle;
			sprite->deactivateCollisions();
			sprite->setPosition(ccp(-256,-256));   //cast them faraway
            sprite->_sliceEntered = false;
            sprite->_sliceExited = false;
            sprite->_entryPoint.SetZero();
            sprite->_exitPoint.SetZero();

			// bomb
			if (sprite->_type == kTypeBomb)
			{
				//MainGameLayer* maingameL = ((MainGameLayer*)getParent());
				MainGameLayer* maingameL = (MainGameLayer*)(getParent()->getParent());
				maingameL->subtractLife();

				//SimpleAudioEngine::getInstance()->playEffect("Sounds/explosion.caf");
			}
			else
			{
				//placeholder
				//SimpleAudioEngine::getInstance()->playEffect("Sounds/squash.caf");
			}

			// Particles
			b2Vec2 convertedWorldEntry = b2Vec2(worldEntry.x*PTM_RATIO,worldEntry.y*PTM_RATIO);
			b2Vec2 convertedWorldExit = b2Vec2(worldExit.x*PTM_RATIO,worldExit.y*PTM_RATIO);
			float midX = midpoint(convertedWorldEntry.x, convertedWorldExit.x);
			float midY = midpoint(convertedWorldEntry.y, convertedWorldExit.y);
			sprite->_splurt->setPosition(ccp(midX,midY));
			sprite->_splurt->resetSystem();
        }
        else 
        {
			//SimpleAudioEngine::getInstance()->playEffect("Sounds/smallcut.caf");
            Box2dDirectorLayer::world->DestroyBody(sprite->_body);
			removeChild(sprite);
        }

		// score system
		MainGameLayer* maingameL = (MainGameLayer*)(getParent()->getParent());
		maingameL->_cuts++;
		maingameL->_cutsLabel->setString( String::createWithFormat("%d", maingameL->_cuts)->getCString() );
    }
    else
    {
        sprite->_sliceEntered = false;
        sprite->_sliceExited = false;
    }
 
    //free up our allocated vectors
    free(sprite1VerticesSorted);
    free(sprite2VerticesSorted);
    free(sprite1Vertices);
    free(sprite2Vertices);
}

bool PolygonSplitter::areVerticesAcceptable(b2Vec2* vertices, int count)
{
	//check 1: polygons need to at least have 3 vertices
    if (count < 3)
    {
        return false;
    }
 
    //check 2: the number of vertices cannot exceed b2_maxPolygonVertices
    if (count > b2_maxPolygonVertices)
    {
        return false;
    }
 
    //check 3: Box2D needs the distance from each vertex to be greater than b2_epsilon
    int32 i;
    for (i=0; i<count; ++i)
    {
        int32 i1 = i;
        int32 i2 = i + 1 < count ? i + 1 : 0;
        b2Vec2 edge = vertices[i2] - vertices[i1];
        if (edge.LengthSquared() <= b2_epsilon * b2_epsilon)
        {
            return false;
        }
    }
 
    //check 4: Box2D needs the area of a polygon to be greater than b2_epsilon
    float32 area = 0.0f;
 
    b2Vec2 pRef(0.0f,0.0f);
 
    for (i=0; i<count; ++i)
    {
        b2Vec2 p1 = pRef;
        b2Vec2 p2 = vertices[i];
        b2Vec2 p3 = i + 1 < count ? vertices[i+1] : vertices[0];
 
        b2Vec2 e1 = p2 - p1;
        b2Vec2 e2 = p3 - p1;
 
        float32 D = b2Cross(e1, e2);
 
        float32 triangleArea = 0.5f * D;
        area += triangleArea;
    }
 
    if (area <= 0.0001)
    {
        return false;
    }
 
    //check 5: Box2D requires that the shape be Convex.
    float determinant;
    float referenceDeterminant;
    b2Vec2 v1 = vertices[0] - vertices[count-1];
    b2Vec2 v2 = vertices[1] - vertices[0];
    referenceDeterminant = calculate_determinant_2x2(v1.x, v1.y, v2.x, v2.y);
 
    for (i=1; i<count-1; i++)
    {
        v1 = v2;
        v2 = vertices[i+1] - vertices[i];
        determinant = calculate_determinant_2x2(v1.x, v1.y, v2.x, v2.y);
        //you use the determinant to check direction from one point to another. A convex shape's points should only go around in one direction. The sign of the determinant determines that direction. If the sign of the determinant changes mid-way, then you have a concave shape.
        if (referenceDeterminant * determinant < 0.0f)
        {
            //if multiplying two determinants result to a negative value, you know that the sign of both numbers differ, hence it is concave
            return false;
        }
    }
    v1 = v2;
    v2 = vertices[0]-vertices[count-1];
    determinant = calculate_determinant_2x2(v1.x, v1.y, v2.x, v2.y);
    if (referenceDeterminant * determinant < 0.0f)
    {
        return false;
    }

    return true;
}

b2Body* PolygonSplitter::createBodyWithPosition(b2Vec2 position, float rotation, 
							   b2Vec2* vertices, int vertexCount, 
							   float density, float friction, float restitution)
{
	b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = position;
    bodyDef.angle = rotation;
    b2Body *body = Box2dDirectorLayer::world->CreateBody(&bodyDef);
 
    b2FixtureDef fixtureDef;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;
 
    b2PolygonShape shape;
    shape.Set(vertices, vertexCount);
    fixtureDef.shape = &shape;
    body->CreateFixture(&fixtureDef);
 
    return body;
}

void PolygonSplitter::checkAndSliceObjects()
{
    double curTime = UtilFunc::getTimeTick();

    for (b2Body* b = Box2dDirectorLayer::world->GetBodyList(); b; b = b->GetNext())
    {
        if (b->GetUserData() != NULL) {
            PolygonSprite *sprite = (PolygonSprite*)b->GetUserData();
 
            if (sprite->_sliceEntered && curTime > sprite->_sliceEntryTime) 
            {
                sprite->_sliceEntered = false;
            }
            else if (sprite->_sliceEntered && sprite->_sliceExited)
            {
				splitPolygonSprite(sprite);
				return;
            }
        }
    }
}

//void PolygonSplitter::clearSlices()
//{
//	for (b2Body* b = Box2dDirectorLayer::world->GetBodyList(); b; b = b->GetNext())
//	{
//		if (b->GetUserData() != NULL) {
//			PolygonSprite *sprite = (PolygonSprite*)b->GetUserData();
//			sprite->_sliceEntered = false;
//			sprite->_sliceExited = false;
//		}
//	}
//}