/*
 *  SplitEvent.h
 *  kdtree
 *
 *  Created by jian zhang on 10/22/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once
#include <BoundingBox.h>
#include <IndexArray.h>
#include <PrimitiveArray.h>

class BuildKdTreeContext;

class SplitEvent {
public:
	typedef Primitive* PrimitivePtr;
	SplitEvent();
	
	void setPos(float val);
	void setAxis(int val);
	
	float getPos() const;
	int getAxis() const;
	
	void setLeftRightNumPrim(const unsigned &leftNumPrim, const unsigned &rightNumPrim);
	
	const float getCost() const;
	
	int leftCount() const;
	int rightCount() const;
	
	void calculateTightBBoxes(const BoundingBox &box, BoundingBox &leftBBox, BoundingBox &rightBBox);
	void calculateCost(float x);
	float area() const;
	int side(const BoundingBox &box) const;
	void updateLeftBox(const BoundingBox &box);
	void updateRightBox(const BoundingBox &box);
	float hasBothSides() const;
	void verbose() const;

	static int Dimension;
	static int NumBinPerDimension;
	static int NumEventPerDimension;
	
private:
	BoundingBox m_leftBox, m_rightBox;
	float m_pos;
	int m_axis;
	float m_cost;
	unsigned m_leftNumPrim, m_rightNumPrim;
	char m_isEmpty;
};
