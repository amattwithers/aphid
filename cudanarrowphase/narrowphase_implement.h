#ifndef NARROWPHASE_IMPLEMENT_H
#define NARROWPHASE_IMPLEMENT_H

#include "bvh_common.h"

extern "C" {
    
void narrowphaseComputeSeparateAxis(ContactData * dstContact,
		uint2 * pairs,
		float3 * pos,
		float3 * vel,
		uint4 * ind,
		uint * pointStart, uint * indexStart, 
		uint numOverlappingPairs);

void narrowphaseComputeTimeOfImpact(ContactData * dstContact,
		uint2 * pairs,
		float3 * pos,
		float3 * vel,
		uint4 * ind,
		uint * pointStart, uint * indexStart, 
		uint numOverlappingPairs);

void narrowphase_computeInitialSeparation(ContactData * dstContact,
		uint2 * pairs,
		float3 * pos,
		float3 * vel,
		uint4 * ind,
		uint * pointStart, uint * indexStart, 
		uint numOverlappingPairs);

void narrowphase_advanceTimeOfImpactIterative(ContactData * dstContact,
		uint2 * pairs,
		float3 * pos,
		float3 * vel,
		uint4 * ind,
		uint * pointStart, uint * indexStart, 
		uint numOverlappingPairs);

void narrowphaseComputeValidPairs(uint * dstCounts, 
        ContactData * srcContact, 
        uint numContacts, 
        uint scanBufferLength);

void narrowphaseSqueezeContactPairs(uint2 * dstPairs, uint2 * srcPairs,
                                    ContactData * dstContact, ContactData *srcContact,
									uint * counts, uint * packLoc, 
									uint maxInd);

void narrowphaseResetX(float3 * dst, float3 *src, uint maxInd);
}
#endif        //  #ifndef NARROWPHASE_IMPLEMENT_H

