#pragma once
#include <AllMath.h>
#include <BoundingRectangle.h>
#include <BoundingBox.h>
class MlRachis;
class CollisionRegion;
class MlFeather {
public:
    MlFeather();
    virtual ~MlFeather();
    void createNumSegment(short x);

	short numSegment() const;
	unsigned numVaneVertices() const;
	unsigned numWorldP() const;
    float * quilly();
    float * getQuilly() const;
	Vector2F baseUV() const;
	
	Vector2F * vane();
    Vector2F * vaneAt(short seg, short side);
    Vector2F * getVaneAt(short seg, short side) const;
	float getLength() const;
	float getWidth(short seg) const;
	BoundingRectangle getBoundingRectangle() const;
	
	void computeWorldP(unsigned faceIdx, float patchU, float patchV, const Vector3F & oriPos, const Matrix33F & oriRot, const float& pitch, const float & scale);
	Vector3F * worldP();
	Vector3F * segmentOriginWP(short seg);
	Vector3F * segmentVaneWP(short seg, short side, short idx);
	
	Vector3F getSegmentOriginWP(short seg) const;
	Vector3F getSegmentVaneWP(short seg, short side, short idx) const;
	
	Vector2F * texcoord();
	Vector2F * segmentQuillTexcoord(short seg);
	Vector2F * segmentVaneTexcoord(short seg, short side, short idx);
	Vector2F getSegmentQuillTexcoord(short seg) const;
	Vector2F getSegmentVaneTexcoord(short seg, short side, short idx) const;
	
	void setCollision(CollisionRegion * skin);
	
	void setFeatherId(short x);
	short featherId() const;
	void setBaseUV(const Vector2F & d);
	void translateUV(const Vector2F & d);
	
	void computeLength();
	void computeTexcoord();
	
	float* selectVertexInUV(const Vector2F & p, bool & yOnly, Vector2F & wp);
	
	void changeNumSegment(int d);
	void getBoundingBox(BoundingBox & box) const;
	void verbose();
private:
	void computeBounding();
	void simpleCreate(int ns = 5);
	void computeVaneWP(const Vector3F & origin, const Matrix33F& space, short seg, short side, float scale);
private:
	BoundingRectangle m_brect;
	Vector2F m_uv;
	CollisionRegion * m_skin;
	MlRachis * m_rachis;
    float *m_quilly;
    Vector2F * m_vaneVertices;
	Vector3F * m_worldP;
	Vector2F * m_st;
	float m_length;
    short m_numSeg, m_id;
};
