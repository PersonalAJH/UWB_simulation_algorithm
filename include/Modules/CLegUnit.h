#pragma once

#include "Graphics\SurfaceObj.h"
#define ARM_JOINT 3
#define ARM_LINK 3

class CLegUnit
{
public:
	CLegUnit();
	~CLegUnit();

public:
	void setSide(int _side);
	void draw();
	void read();

	void updateJointAngle(Vec3f hand, Vec3f low, Vec3f up);

	void transPart(Vec3f distance, int idx);
	void rotateMat(int idx, Mat3x3f rot);
	void calcEnd(Vec3f hand, Vec3f lower, Vec3f upper);
	void calcInv(Vec3f elb2, Vec3f sho2, Vec3f elb_ang);

	Mat3x3f calcMat(Vec3f angle, int dir);



private:
	int side = 1;

	Vec3f midUpper = Vec3f(200, 10, -570);
	Vec3f midLower = Vec3f(170, 70, -1590);
	Vec3f midHand = Vec3f(170, 90, -2540);

	SurfaceObj m_ArmUnit[ARM_LINK];

	Vec3f jointPos[ARM_JOINT];
	Vec3f jointOrg[ARM_JOINT];


	Vec3f endPos;
	Vec3f wristPos;
	Vec3f sholderPos;
	Vec3f elbowPos;

	float theta4;
	Vec3f invAngle;
};