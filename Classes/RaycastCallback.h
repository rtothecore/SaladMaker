#ifndef __RAYCAST_CALLBACK_H__
#define __RAYCAST_CALLBACK_H__

#include "Box2d/Box2D.h"
#include "PolygonSprite.h"
#include "UtilFunc.h"

USING_NS_CC;

#define PTM_RATIO 32
#define collinear(x1,y1,x2,y2,x3,y3) fabsf((y1-y2) * (x1-x3) - (y1-y3) * (x1-x2))

class RaycastCallback : public b2RayCastCallback
{
public:
	RaycastCallback(){
	}

	float ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float fraction)
	{
		PolygonSprite *ps = (PolygonSprite*)fixture->GetBody()->GetUserData();
		if(ps)
		{
			if (!ps->_sliceEntered)
			{
				ps->_sliceEntered = true;

				//you need to get the point coordinates within the shape
				ps->_entryPoint  = ps->_body->GetLocalPoint(point);

				ps->_sliceEntryTime = UtilFunc::getTimeTick() + 1;
				log("Slice Entered at world coordinates:(%f,%f), polygon coordinates:(%f,%f)", point.x*PTM_RATIO, point.y*PTM_RATIO, ps->_entryPoint.x*PTM_RATIO, ps->_entryPoint.y*PTM_RATIO);
			}
			else if (!ps->_sliceExited)
			{
				ps->_exitPoint = ps->_body->GetLocalPoint(point);

				b2Vec2 entrySide = ps->_entryPoint - ps->_centroid;
				b2Vec2 exitSide = ps->_exitPoint - ps->_centroid;

				if (entrySide.x * exitSide.x < 0 || entrySide.y * exitSide.y < 0)
				{
					ps->_sliceExited = true;
				}
				else {
					//if the cut didn't cross the centroid, you check if the entry and exit point lie on the same line
					b2Fixture *fixture = ps->_body->GetFixtureList();
					b2PolygonShape *polygon = (b2PolygonShape*)fixture->GetShape();
					int count = polygon->GetVertexCount();

					bool onSameLine = false;
					for (int i = 0 ; i < count; i++)
					{
						b2Vec2 pointA = polygon->GetVertex(i);
						b2Vec2 pointB;

						if (i == count - 1)
						{
							pointB = polygon->GetVertex(0);
						}
						else {
							pointB = polygon->GetVertex(i+1);
						}//endif

						float collinear = collinear(pointA.x,pointA.y, ps->_entryPoint.x, ps->_entryPoint.y, pointB.x,pointB.y);

						if (collinear <= 0.00001)
						{
							float collinear2 = collinear(pointA.x,pointA.y,ps->_exitPoint.x,ps->_exitPoint.y,pointB.x,pointB.y);
							if (collinear2 <= 0.00001)
							{
								onSameLine = true;
							}
							break;
						}//endif
					}//endfor

					if (onSameLine)
					{
						ps->_entryPoint = ps->_exitPoint;
						ps->_sliceEntryTime = UtilFunc::getTimeTick() + 1;
						ps->_sliceExited = false;
					}
					else {
						ps->_sliceExited = true;
					}//endif
				}

				log("Slice Exited at world coordinates:(%f,%f), polygon coordinates:(%f,%f)", point.x*PTM_RATIO, point.y*PTM_RATIO, ps->_exitPoint.x*PTM_RATIO, ps->_exitPoint.y*PTM_RATIO);
			}
		}
		return 1;
	}
};

#endif