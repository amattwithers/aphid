/*
 *  Plane.h
 *  mallard
 *
 *  Created by jian zhang on 8/29/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include <AllMath.h>
#include <Ray.h>
class Plane {
public:
	Plane();
	Plane(float a, float b, float c, float d);
	Plane(const Vector3F & nor, const Vector3F & pop);
	Plane(const Vector3F & p0, const Vector3F & p1, const Vector3F & p2, const Vector3F & p3);
	virtual ~Plane();
	void create(const Vector3F & p0, const Vector3F & p1, const Vector3F & p2, const Vector3F & p3);
	
	Vector3F normal() const;
	void getNormal(Vector3F & nor) const;
	void projectPoint(const Vector3F & p0, Vector3F & dst) const;
	float pointTo(const Vector3F & p0) const;
	bool rayIntersect(const Ray & ray, Vector3F & dst, float & t, bool twoSided = false) const;
	void verbose() const;
private:
	float m_a, m_b, m_c, m_d;
};