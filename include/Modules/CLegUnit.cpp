#include "stdafx.h"
#include "CLegUnit.h"


CLegUnit::CLegUnit()
{
	jointOrg[2] = midUpper;
	jointOrg[1] = midLower;
	jointOrg[0] = midHand;

	jointPos[2] = midUpper;
	jointPos[1] = midLower;
	jointPos[0] = midHand;
}



CLegUnit::~CLegUnit()
{
}

void CLegUnit::setSide(int _side) {
	if (_side != 1) {
		side = _side;
		for (int j = 0; j < 3; j++) {
			jointOrg[j][0] = -jointOrg[j][0];
		}
	}
}


void CLegUnit::read()
{
	if (side == 1) {
		m_ArmUnit[2].readObjData("../data/obj/rightUpperLeg.txt");
		m_ArmUnit[1].readObjData("../data/obj/rightLowerLeg.txt");
		m_ArmUnit[0].readObjData("../data/obj/rightFoot.txt");
	}
	else {
		m_ArmUnit[2].readObjData("../data/obj/leftUpperLeg.txt");
		m_ArmUnit[1].readObjData("../data/obj/leftLowerLeg.txt");
		m_ArmUnit[0].readObjData("../data/obj/leftFoot.txt");
	}

}

void CLegUnit::draw()
{
	m_ArmUnit[2].drawObject(Vec3f(1, 1, 1));
	m_ArmUnit[1].drawObject(Vec3f(1, 1, 1));
	m_ArmUnit[0].drawObject(Vec3f(1, 1, 1));

}

void CLegUnit::updateJointAngle(Vec3f hand, Vec3f lower, Vec3f upper)
{
	// 손목 관절 원점 이동
	transPart(-jointOrg[0], 0);

	// 손목 회전
	rotateMat(0, calcMat(hand, 1));
	rotateMat(0, calcMat(-lower, -1));

	// 팔꿈치 관절 원점 이동
	transPart(jointOrg[0] - jointOrg[1], 1);

	// 손목, 팔꿈치 회전
	rotateMat(0, calcMat(lower, 1));
	rotateMat(1, calcMat(lower, 1));
	rotateMat(0, calcMat(-upper, -1));
	rotateMat(1, calcMat(-upper, -1));

	// 어깨 관절 원점으로 이동
	transPart(jointOrg[1] - jointOrg[2], 1);

	// 손목, 팔꿈치, 어깨 회전
	rotateMat(0, calcMat(upper, 1));
	rotateMat(1, calcMat(upper, 1));
	rotateMat(2, calcMat(upper, 1));

	// 어깨 관절 원위치
	transPart(jointOrg[2], 1);

	calcEnd(hand, lower, upper);
}

void CLegUnit::transPart(Vec3f distance, int idx)
{
	if (idx == 0) { // 초기위치 기준 이동
		m_ArmUnit[2].translate(distance);
		m_ArmUnit[1].translate(distance);
		m_ArmUnit[0].translate(distance);
	}
	else { // 현재위치 기준 이동
		m_ArmUnit[2].translateC(distance);
		m_ArmUnit[1].translateC(distance);
		m_ArmUnit[0].translateC(distance);
	}

}

void CLegUnit::rotateMat(int idx, Mat3x3f rot)
{
	m_ArmUnit[idx].rotateC(rot);
}

void CLegUnit::calcEnd(Vec3f hand, Vec3f lower, Vec3f upper)
{
	Vec3f p = Vec3f(-60, 30, -360);
	elbowPos = jointOrg[2] + calcMat(upper, 1) * (jointOrg[1] - jointOrg[2]);
	wristPos = elbowPos + calcMat(lower, 1) * (jointOrg[0] - jointOrg[1]);
	endPos = wristPos + calcMat(hand, 1) * (p);
}

Mat3x3f CLegUnit::calcMat(Vec3f angle, int dir) {
	float a = angle[2] * PI / 180;
	float b = angle[1] * PI / 180;
	float c = angle[0] * PI / 180;
	Vec3f r1;
	Vec3f r2;
	Vec3f r3;
	if (dir == 1) { // ZYX 회전
		r1 = Vec3f(cos(a)*cos(b), cos(a)*sin(b)*sin(c) - sin(a)*cos(c), cos(a)*sin(b)*cos(c) + sin(a)*sin(c));
		r2 = Vec3f(sin(a)*cos(b), sin(a)*sin(b)*sin(c) + cos(a)*cos(c), sin(a)*sin(b)*cos(c) - cos(a)*sin(c));
		r3 = Vec3f(-sin(b), cos(b)*sin(c), cos(b)*cos(c));
	}
	else {			// XYZ 회전
		r1 = Vec3f(cos(a)*cos(b), -sin(a)*cos(b), sin(b));
		r2 = Vec3f(cos(a)*sin(b)*sin(c) + sin(a)*cos(c), -sin(a)*sin(b)*sin(c) + cos(a)*cos(c), -cos(b)*sin(c));
		r3 = Vec3f(-cos(a)*sin(b)*cos(c) + sin(a)*sin(c), sin(a)*sin(b)*cos(c) + cos(a)*sin(c), cos(b)*cos(c));
	}

	return Mat3x3f(r1, r2, r3);
}

void CLegUnit::calcInv(Vec3f elb2, Vec3f wri2, Vec3f ang_elb)
{
	Vec3f sho = Vec3f(430, -50, 950);
	Vec3f elb = Vec3f(520, -50, 160);
	Vec3f wri = Vec3f(550, -50, -500);
	Vec3f L1_0 = elb - sho;
	Vec3f L2_0 = wri - elb;
	Vec3f L1_t = elb2 - sho;
	Vec3f L2_t = wri2 - elb2;
	Vec3f L3_t = L1_t + L2_t;
	float n_L1 = 795.1073;
	float n_L2 = 660.6862;
	float n_L3 = sqrt(L3_t(0)*L3_t(0) + L3_t(1)*L3_t(1) + L3_t(2)*L3_t(2));

	theta4 = acos((n_L1*n_L1 + n_L2 * n_L2 - n_L3 * n_L3) / (2 * n_L1*n_L2)) * 180 / PI;

	Mat3x3f R_sho = calcMat(ang_elb, 1) * calcMat(-Vec3f(180 - theta4, 0, 0), 1);


	invAngle(2) = atan2f(R_sho(1, 0), R_sho(0, 0)) * 180 / PI;
	invAngle(1) = atan2f(-R_sho(2, 0), sqrt(R_sho(2, 1)*R_sho(2, 1) + R_sho(2, 2)*R_sho(2, 2))) * 180 / PI;
	invAngle(0) = atan2f(R_sho(2, 1), R_sho(2, 2)) * 180 / PI;

}
