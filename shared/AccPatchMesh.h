/*
 *  AccPatchMesh.h
 *  mallard
 *
 *  Created by jian zhang on 8/30/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include <PatchMesh.h>
class BezierPatch;
class AccPatch;
class MeshTopology;
struct PatchSplitContext;

class AccPatchMesh : public PatchMesh {
public:
	AccPatchMesh();
	virtual ~AccPatchMesh();
	
	void setup(MeshTopology * topo);
	
	AccPatch* beziers() const;
	
	virtual const BoundingBox calculateBBox() const;
	virtual const BoundingBox calculateBBox(const unsigned &idx) const;
	virtual char intersect(unsigned idx, IntersectionContext * ctx) const;
	virtual char closestPoint(unsigned idx, const Vector3F & origin, IntersectionContext * ctx) const;
	
private:
	char recursiveBezierIntersect(BezierPatch* patch, IntersectionContext * ctx, const PatchSplitContext split, int level) const;
	void recursiveBezierClosestPoint(const Vector3F & origin, BezierPatch* patch, IntersectionContext * ctx, const PatchSplitContext split, int level) const;
	
	AccPatch* m_bezier;
};