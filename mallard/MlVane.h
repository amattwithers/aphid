#pragma once
#include <BezierCurve.h>
class MlVane {
public:
    MlVane();
    virtual ~MlVane();
    
    void create(unsigned gridU, unsigned gridV);
    BezierCurve * profile(unsigned idx) const;
    void computeKnots();
    void pointOnVane(float u, float v, Vector3F & dst);
private:
    BezierCurve m_profile;
    BezierCurve * m_rails;
    unsigned m_gridU, m_gridV;
};