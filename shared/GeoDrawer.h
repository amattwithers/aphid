/*
 *  GeoDrawer.h
 *  aphid
 *
 *  Created by jian zhang on 1/10/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "MeshDrawer.h"
class BaseTransform;
class TransformManipulator;
class SkeletonJoint;
class Anchor;
class SpaceHandle;
class GeodesicSphereMesh;
class PyramidMesh;
class CubeMesh;
class CircleCurve;
class SelectionArray;
class BaseCurve;
class Primitive;

class GeoDrawer : public MeshDrawer {
public:
	GeoDrawer();
	virtual ~GeoDrawer();
	
	void box(float width, float height, float depth);
	
	void sphere(float size = 1.f) const;
	void cube(const Vector3F & p, const float & size) const;
	void solidCube(float x, float y, float z, float size);
	
	void circleAt(const Vector3F & pos, const Vector3F & nor);
	void circleAt(const Matrix44F & mat, float radius);
	
	void arrow0(const Vector3F & at, const Vector3F & dir, float l, float w) const;
	void arrow2(const Vector3F& origin, const Vector3F& dest, float width) const;
	void arrow(const Vector3F& origin, const Vector3F& dest) const;
	
	void coordsys(float scale = 1.f) const;
	void coordsys(const Matrix33F & orient, float size = 1.f, Vector3F * p = 0) const;
	
	void manipulator(TransformManipulator * m);
	void spaceHandle(SpaceHandle * hand);
	void anchor(Anchor *a, char active = 0);
	
	void transform(BaseTransform * t);
	
	void skeletonJoint(SkeletonJoint * joint);
	void moveHandle(int axis, bool active);
	void spinHandle(TransformManipulator * m, bool active);
	void spinPlanes(BaseTransform * t);
	
	void components(SelectionArray * arr);
	
	void linearCurve(const BaseCurve & curve);
	void smoothCurve(BaseCurve & curve, short deg);
	
	void primitive(Primitive * prim);
	
	
private:
	GeodesicSphereMesh * m_sphere;
	PyramidMesh * m_pyramid;
	CubeMesh * m_cube;
	CircleCurve * m_circle;
};