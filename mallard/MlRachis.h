/*
 *  MlRachis.h
 *  mallard
 *
 *  Created by jian zhang on 9/20/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include <AllMath.h>

class CollisionRegion;

class MlRachis {
public:
	MlRachis();
	~MlRachis();
	
	void create(unsigned x);
	void computeAngles(float * segL, float fullL);
	void reset();
	void update(const Vector3F & oriP, const Matrix33F & space, const float & scale, CollisionRegion * collide, const float & fullPitch);
	
	Matrix33F getSpace(short idx) const;
private:
	unsigned m_numSpace;
	Matrix33F * m_spaces;
	float * m_angles;
};