#pragma once
#include <AllMath.h>
class BezierPatch;
class PointInsidePolygonTest;
class InverseBilinearInterpolate;
class BezierPatchHirarchy {
public:
    BezierPatchHirarchy();
    virtual ~BezierPatchHirarchy();
	void cleanup();
    void create(BezierPatch * parent);
    char isEmpty() const;
    char endLevel(int level) const;
    
    BezierPatch * patch(unsigned idx) const;
	PointInsidePolygonTest * plane(unsigned idx) const;
    BezierPatch * childPatch(unsigned idx) const;
    unsigned childStart(unsigned idx) const;
	Vector2F restoreUV(unsigned idx, const Vector3F & p) const;
private:
    void recursiveCreate(short level, unsigned & current, unsigned & start);
    BezierPatch * m_elm;
	PointInsidePolygonTest * m_planes;
	InverseBilinearInterpolate * m_invbil;
    unsigned * m_childIdx;
};