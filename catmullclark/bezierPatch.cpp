/*
 *  bezierPatch.cpp
 *  catmullclark
 *
 *  Created by jian zhang on 10/26/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include <math.h>
#include "bezierPatch.h"

BezierPatch::BezierPatch() {}
BezierPatch::~BezierPatch() {}
#include <iostream>
void BezierPatch::setTexcoord(float* u, float* v, unsigned* idx)
{
	_texcoords[0].x = u[idx[0]];
	_texcoords[1].x = u[idx[3]];
	_texcoords[2].x = u[idx[1]];
	_texcoords[3].x = u[idx[2]];
	
	_texcoords[0].y = v[idx[0]];
	_texcoords[1].y = v[idx[3]];
	_texcoords[2].y = v[idx[1]];
	_texcoords[3].y = v[idx[2]];
	
	float smin = _texcoords[0].x;
	if(_texcoords[1].x < smin) smin = _texcoords[1].x;
	if(_texcoords[2].x < smin) smin = _texcoords[2].x;
	if(_texcoords[3].x < smin) smin = _texcoords[3].x;
	
	float smax = _texcoords[0].x;
	if(_texcoords[1].x > smin) smax = _texcoords[1].x;
	if(_texcoords[2].x > smin) smax = _texcoords[2].x;
	if(_texcoords[3].x > smin) smax = _texcoords[3].x;
	
	float tmin = _texcoords[0].y;
	if(_texcoords[1].y < tmin) tmin = _texcoords[1].y;
	if(_texcoords[2].y < tmin) tmin = _texcoords[2].y;
	if(_texcoords[3].y < tmin) tmin = _texcoords[3].y;
	
	float tmax = _texcoords[0].y;
	if(_texcoords[1].y > tmin) tmax = _texcoords[1].y;
	if(_texcoords[2].y > tmin) tmax = _texcoords[2].y;
	if(_texcoords[3].y > tmin) tmax = _texcoords[3].y;
	
	float delta = smax - smin;
	if(tmax - tmin > delta) delta = tmax - tmin;
	_lodBase = (int)log2(1.0 / delta) + 1;
	printf("lod b %i ", _lodBase);
}

Vector3F BezierPatch::p(unsigned u, unsigned v) const
{
	return _contorlPoints[4 * v + u];
}

Vector3F BezierPatch::normal(unsigned u, unsigned v) const
{
	return _normals[4 * v + u];
}

Vector2F BezierPatch::tex(unsigned u, unsigned v) const
{
	return _texcoords[2 * v + u];
}

void BezierPatch::evaluateContolPoints()
{
	_contorlPoints[0] = Vector3F(-1.f, -.1f, -1.f);
	_contorlPoints[1] = Vector3F(0.f, -.05f, -1.1f);
	_contorlPoints[2] = Vector3F(1.f, -.3f, -1.4f);
	_contorlPoints[3] = Vector3F(2.f, -1.f, -1.f);
	
	_contorlPoints[4] = Vector3F(-1.f, .34f, 0.f);
	_contorlPoints[5] = Vector3F(0.f, 1.16f, 0.f);
	_contorlPoints[6] = Vector3F(1.4f, 1.f, 0.f);
	_contorlPoints[7] = Vector3F(2.5f, .2f, 0.17f);
	
	_contorlPoints[8] = Vector3F(-1.f, .5f, 1.f);
	_contorlPoints[9] = Vector3F(0.f, 1.1f, 1.2f);
	_contorlPoints[10] = Vector3F(1.2f, .9f, 1.6f);
	_contorlPoints[11] = Vector3F(2.3f, .1f, 1.5f);
	
	_contorlPoints[12] = Vector3F(-1.f, -1.f, 2.f);
	_contorlPoints[13] = Vector3F(0.f, -1.f, 2.3f);
	_contorlPoints[14] = Vector3F(1.f, -1.f, 2.5f);
	_contorlPoints[15] = Vector3F(2.f, -1.f, 1.98f);
}

void BezierPatch::evaluateTangents()
{
	for(unsigned j = 0; j < 4; j++)
	{
		for(unsigned i = 0; i < 3; i++)
		{
			_tangents[j * 3 + i] = p(i + 1, j) - p(i, j);
		}
	}
}

void BezierPatch::evaluateBinormals()
{
	for(unsigned j = 0; j < 3; j++)
	{
		for(unsigned i = 0; i < 4; i++)
		{
			_binormals[j * 4 + i] = p(i, j + 1) - p(i, j);
		}
	}
}

void BezierPatch::evaluateSurfacePosition(float u, float v, Vector3F * pos) const
{
	Vector2F L0(1.f-u,1.f-v);
	Vector2F L1(u,v);

	Vector2F B0 =     (L0 * L0) * L0;
	Vector2F B1 = (L0 * L0) * L1 * 3.f;
	Vector2F B2 = (L1 * L1) * L0 * 3.f;
	Vector2F B3 =     (L1 * L1) * L1;

	*pos = 
		(p(0,0) * B0.x + p(1,0) * B1.x + p(2,0) * B2.x + p(3,0) * B3.x) * B0.y +
		(p(0,1) * B0.x + p(1,1) * B1.x + p(2,1) * B2.x + p(3,1) * B3.x) * B1.y +
		(p(0,2) * B0.x + p(1,2) * B1.x + p(2,2) * B2.x + p(3,2) * B3.x) * B2.y +
		(p(0,3) * B0.x + p(1,3) * B1.x + p(2,3) * B2.x + p(3,3) * B3.x) * B3.y;
}

void BezierPatch::evaluateSurfaceTangent(float u, float v, Vector3F * tang) const
{
	Vector2F L0(1-u,1-v);
	Vector2F L1(u,v);
	
	Vector2F Q0 =     L0 * L0;
	Vector2F Q1 =  L0 * L1 * 2.f;
	Vector2F Q2 =     L1 * L1;

	Vector2F B0 =     L0 * Q0;
	Vector2F B1 = Q0 * L1 * 3.f;
	Vector2F B2 = Q2 * L0 * 3.f;
	Vector2F B3 =     Q2 * L1;
	
	*tang = 
		(_tangents[0 ] * B0.y + _tangents[1 ] * B1.y + _tangents[2 ] * B2.y + _tangents[3 ] * B3.y) * Q0.x +
		(_tangents[4 ] * B0.y + _tangents[5 ] * B1.y + _tangents[6 ] * B2.y + _tangents[7 ] * B3.y) * Q1.x +
		(_tangents[8 ] * B0.y + _tangents[9 ] * B1.y + _tangents[10] * B2.y + _tangents[11] * B3.y) * Q2.x;
}

void BezierPatch::evaluateSurfaceBinormal(float u, float v, Vector3F * binm) const
{
	Vector2F L0(1-u,1-v);
	Vector2F L1(u,v);
	
	Vector2F Q0 =     L0 * L0;
	Vector2F Q1 =  L0 * L1 * 2.f;
	Vector2F Q2 =     L1 * L1;

	Vector2F B0 =     L0 * Q0;
	Vector2F B1 = Q0 * L1 * 3.f;
	Vector2F B2 = Q2 * L0 * 3.f;
	Vector2F B3 =     Q2 * L1;
	
	*binm = 
		(_binormals[0 ] * B0.x + _binormals[1 ] * B1.x + _binormals[2 ] * B2.x + _binormals[3 ] * B3.x) * Q0.y +
		(_binormals[4 ] * B0.x + _binormals[5 ] * B1.x + _binormals[6 ] * B2.x + _binormals[7 ] * B3.x) * Q1.y +
		(_binormals[8 ] * B0.x + _binormals[9 ] * B1.x + _binormals[10] * B2.x + _binormals[11] * B3.x) * Q2.y;

}

void BezierPatch::evaluateSurfaceNormal(float u, float v, Vector3F * nor) const
{
	//Vector3F dpdu, dpdv;
	//evaluateSurfaceTangent(u, v, &dpdu);
	//evaluateSurfaceBinormal(u , v, &dpdv);
	//*nor = dpdv.cross(dpdu).normal();
	Vector2F L0(1.f-u,1.f-v);
	Vector2F L1(u,v);

	Vector2F B0 =     (L0 * L0) * L0;
	Vector2F B1 = (L0 * L0) * L1 * 3.f;
	Vector2F B2 = (L1 * L1) * L0 * 3.f;
	Vector2F B3 =     (L1 * L1) * L1;

	*nor = 
		(normal(0,0) * B0.x + normal(1,0) * B1.x + normal(2,0) * B2.x + normal(3,0) * B3.x) * B0.y +
		(normal(0,1) * B0.x + normal(1,1) * B1.x + normal(2,1) * B2.x + normal(3,1) * B3.x) * B1.y +
		(normal(0,2) * B0.x + normal(1,2) * B1.x + normal(2,2) * B2.x + normal(3,2) * B3.x) * B2.y +
		(normal(0,3) * B0.x + normal(1,3) * B1.x + normal(2,3) * B2.x + normal(3,3) * B3.x) * B3.y;
}

void BezierPatch::evaluateSurfaceTexcoord(float u, float v, Vector3F * texcoord) const
{
	Vector2F L0(1-u,1-v);
	Vector2F L1(u,v);

	Vector2F st = 
		(tex(0, 0) * L0.x + tex(1, 0) * L1.x) * L0.y + 
		(tex(0, 1) * L0.x + tex(1, 1) * L1.x) * L1.y;
	*texcoord = Vector3F(st.x, st.y, 0.f);
}

int BezierPatch::getLODBase() const
{
	return _lodBase;
}
