/*
 *  BuildKdTreeContext.cpp
 *  kdtree
 *
 *  Created by jian zhang on 10/20/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "BuildKdTreeContext.h"
BuildKdTreeContext::BuildKdTreeContext() {}

void BuildKdTreeContext::appendMesh(BaseMesh* mesh)
{
	unsigned numFace = mesh->getNumFaces();
	m_primitives.expandBy(numFace);
	for(unsigned i = 0; i < numFace; i++) {
		Primitive *p = m_primitives.asPrimitive();
		p->setGeom((char *)mesh->getFace(i));
		p->setType(0);
		m_primitives.next();
	}
	m_primitives.verbose();
}

void BuildKdTreeContext::initIndices()
{
	unsigned numPrim = getNumPrimitives();
	m_indices.expandBy(numPrim);
	
	for(unsigned i = 0; i < numPrim; i++) {
		unsigned * p = m_indices.asIndex(i);
		*p = i;
	}
	m_indices.setIndex(numPrim);
	m_indices.verbose();
}

void BuildKdTreeContext::partition(const SplitCandidate & split, PartitionBound & bound)
{	
	unsigned numPrim = bound.numPrimitive();
	
	ClassificationStorage classification;
	classification.setPrimitiveCount(numPrim);
	
	for(unsigned i = bound.parentMin; i < bound.parentMax; i++) {
		unsigned idx = *m_indices.asIndex(i);
		const Triangle *tri = (Triangle *)(m_primitives.asPrimitive(idx)->geom());
		int side = tri->classify(split);
		classification.set(i - bound.parentMin, side);
	}
	
	bound.leftChildMin = m_indices.index();

	m_indices.expandBy(numPrim);
	//printf("left side ");
	for(unsigned i = bound.parentMin; i < bound.parentMax; i++) {
		int side = classification.get(i - bound.parentMin);
		if(side < 2) {
			unsigned idx = *m_indices.asIndex(i);
			unsigned *cur = m_indices.asIndex();
			*cur = idx;
			//printf(" %i ", *cur);
			m_indices.next();
		}
	}
	bound.rightChildMin = bound.leftChildMax = m_indices.index();
	
	//printf("left index %i - %i\n", bound.leftChildMin, bound.leftChildMax);
	//printf("right side ");
	m_indices.expandBy(numPrim);
	for(unsigned i = bound.parentMin; i < bound.parentMax; i++) {
		int side = classification.get(i - bound.parentMin);
		if(side > 0) {
			unsigned idx = *m_indices.asIndex(i);
			unsigned *cur = m_indices.asIndex();
			*cur = idx;
			//printf(" %i ", *cur);
			m_indices.next();
		}
	}
	bound.rightChildMax = m_indices.index();
	
	//printf("right index %i - %i\n", bound.rightChildMin, bound.rightChildMax);
	
	//printf("ctx partition %i primitives\n", bound.numPrimitive());
	
	//unsigned leftCount = bound.leftCount();
	//unsigned rightCount = bound.rightCount();
	//printf("%i to left side\n", leftCount);
	//for(unsigned i = bound.leftChildMin; i < bound.leftChildMax; i++) {
	//	printf("%i ", *m_indices.asIndex(i));
	//}
	//printf("\n");
	//printf("%i to right side\n", rightCount);
	//for(unsigned i = bound.rightChildMin; i < bound.rightChildMax; i++) {
	//	printf("%i ", *m_indices.asIndex(i));
	//}
	//printf("\n");
	
}

const unsigned BuildKdTreeContext::getNumPrimitives() const
{
	return m_primitives.index();
}

const BoundingBox BuildKdTreeContext::calculateTightBBox()
{
	BoundingBox bbox;
	unsigned numPrim = getNumPrimitives();	
	for(unsigned i = 0; i < numPrim; i++) {
		Triangle *tri = (Triangle *)(m_primitives.asPrimitive(i)->geom());
		tri->expandBBox(bbox);
	}
	return bbox;
}

void BuildKdTreeContext::verbose() const
{
	m_indices.verbose();
}