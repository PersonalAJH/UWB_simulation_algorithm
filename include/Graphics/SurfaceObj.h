#ifndef SURFACE_OBJ
#define SURFACE_OBJ

#include "./DataTypes/Define.h"
#include "./DataTypes/Vec.h"
#include "./DataTypes/Mat.h"
#include "./DataTypes/Quat.h"
#include "./Modules/TimeTick.h"
#include <afxwin.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <vector>

#define MAX		100000000
#define MIN		-100000000

class SurfaceObj
{
public:
	typedef struct
	{
		Vec3f Point[1000];
		int PointIndex[1000];
		int PointNum;
	}	Hash;

public:
	SurfaceObj(void);
	~SurfaceObj(void);

//variables
private:
	FILE* FTime;
	std::vector<Vec3f>*	Point0;
	std::vector<Vec3f>*	Point;
	std::vector<Vec3i>*	Face;
	std::vector<Vec2i>*	Edge;
	std::vector<Vec3f>*	FaceNormal;
	std::vector<Vec3f>* PointNormal;
	Vec3f   MidPoint;
	Hash*	m_Hash;
	
//functions
public:
	void readObjData(char* filename);
	void readSTLData(char* filename);
	void init(Vec3f* point, int nbPoint, Vec3i* face, int nbFace);
	void writeObjData(char* filename);
	void drawObject(bool mode, Vec3f color);
	void drawObject(Vec3f color);
	void drawWireFrame(Vec3f color);
	
	//get values
	std::vector<Vec3f>*	point0();
	std::vector<Vec3f>*	point();
	std::vector<Vec3i>*	face();
	std::vector<Vec2i>*	edge();
	std::vector<Vec3f>*	faceNormal();
	std::vector<Vec3f>* pointNormal();
	Vec3f   midPoint();

	void computeCenterPoint();
	void computeRotationMatrix(Vec3f axis, float angle, Mat3x3f& rot);
	void moveToCenter();
	void translateI(float x, float y, float z);
	void translateI(Vec3f trans);
	void translate(float x, float y, float z);
	void translateC(Vec3f trans);
	void translate(Vec3f trans);
	void scaleI(float scale);
	void scaleI(float x, float y, float z);
	void scaleC(float x, float y, float z);
	void scaleC(float scale);
	void scale(float scale);
	void scale(float x, float y, float z);
	void rotate(Mat3x3f& rot);
	void rotateC(Mat3x3f rot);
	void rotateC(Quat q);
	void rotate(Quat q);
	void rotate(float* rot);
	void rotateC(Vec3f axis, float angle);
	void rotate(Vec3f axis, float angle);

	std::vector<Vec3f> Temp;
private:
	void computeFaceNormal();
	Vec3f computeFaceNormal(Vec3f p1, Vec3f p2, Vec3f p3);
	void computePointNormal();
};

#endif