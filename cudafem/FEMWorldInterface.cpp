#include "FEMWorldInterface.h"
#include <AllMath.h>
#include <CudaDynamicWorld.h>
#include <FEMTetrahedronSystem.h>
#include <tetmesh.h>

#define COLLIDEJUST 0

FEMWorldInterface::FEMWorldInterface() {}
FEMWorldInterface::~FEMWorldInterface() {}

void FEMWorldInterface::create(CudaDynamicWorld * world)
{
#if COLLIDEJUST
    return DynamicWorldInterface::create(world);
#endif
    std::cout<<"num points "<<TetraNumVertices<<"\n";
	std::cout<<"num tetrahedrons "<<TetraNumTetrahedrons<<"\n";
	
    FEMTetrahedronSystem * tetra = new FEMTetrahedronSystem;
	tetra->create(TetraNumTetrahedrons+100, TetraNumVertices+400);
	float * hv = &tetra->hostV()[0];
	float vrx, vry, vrz, vr;
	float vy = 1.695f;
	unsigned i;
	Vector3F p, q;
	for(i=0; i<TetraNumVertices; i++) {
	    p.set(TetraP[i][0], TetraP[i][1], TetraP[i][2]);
	    tetra->addPoint(&p.x);
	    
	    vrx = 0.0932f * (RandomF01() - .5f);
		vry = .5f * (RandomF01() + 1.f)  * vy;
		vrz = 0.0932f * (RandomF01() - .5f);
		vr = 0.013f * RandomF01();
		
		vy = -vy;
			
	    hv[0] = 0.f;//vrx + vr;
		hv[1] = 0.f;
		hv[2] = 0.f;//vrz - vr;
		hv+=3;
	}
	
	for(i=0; i<TetraNumTetrahedrons; i++) {
	    tetra->addTetrahedron(TetraIndices[i][0], TetraIndices[i][1], TetraIndices[i][2], TetraIndices[i][3]);
	    
	    // p.set(TetraP[TetraIndices[i][0]][0], TetraP[TetraIndices[i][0]][1], TetraP[TetraIndices[i][0]][2]);
	    // q.set(TetraP[TetraIndices[i][1]][0], TetraP[TetraIndices[i][1]][1], TetraP[TetraIndices[i][1]][2]);
	    // std::cout<<"el "<<(p - q).length()<<" ";
	}
	tetra->setAnchoredPoint(122, 0);
	tetra->setAnchoredPoint(123, 20);
	tetra->setAnchoredPoint(116, 9);
	tetra->setAnchoredPoint(124, 78);
	tetra->setTotalMass(1000.f);
	world->addTetrahedronSystem(tetra);
}
