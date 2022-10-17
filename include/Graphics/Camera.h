// Camera.h: interface for the CObj class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(_CAMERA_H_)
#define _CAMERA_H_

#include "../DataTypes/Vec.h"
#include "../DataTypes/Mat.h"

class CCamera  
{
public:
	Vec3d		m_Pos;
	Vec3d		m_Up;
	Vec3d		m_Center;
	Vec3d		m_Direction;

	Vec3d		m_RotAxis;
	float		m_Distance;
	float		m_RotAngle;
	Mat3x3d		m_RotMatrix;

	CCamera()
	{
		m_Pos[2]=2000;
		m_Distance=2000;
		m_Up[1]=1;
		m_RotAngle=0;
		m_RotMatrix.identity();
	};

	CCamera(double dis)
	{
		m_Pos[2]=dis;
		m_Distance=dis;
		m_Up[1]=1;
		m_RotAngle=0;
		m_RotMatrix.identity();
	};
	
	~CCamera()
	{

	};

	void Init(double dis)
	{
		m_Pos[2]=dis;
		m_Distance=dis;
	};

	void Init(Vec3d camPos, Vec3d centerPos)
	{
		m_Pos=camPos;
		m_Center=centerPos;
		m_Distance=(m_Center-m_Pos).norm();

		Vec3d initialPos;
		initialPos[2]=m_Distance;
		ComputeAxisAngle(initialPos,m_Pos-m_Center);
		UpdateCamPos();
	};

	void MoveCamPos(Vec3d trans)
	{
		trans=m_RotMatrix*trans;
		m_Center=m_Center-trans;
		m_Pos=m_Pos-trans;
	};

	//center point를 중심으로 camera의 위치를 회전시킨다
	void RotCamPos(Vec3d DMousePos)
	{
		Vec3d camPos;
		Vec3d camUp;
		camPos[2]=m_Distance;
		camUp[1]=1;

		ComputeAxisAngle(DMousePos);
		
		//회전된 각도에 따라서 rotation matrix를 정의한다.
		Mat3x3d rot;

		//rotation matrix를 계산한다.
		ComputeRotationMatrix(rot,m_RotAxis,m_RotAngle);
		if(m_RotAngle==0)
			rot.identity();

		m_RotMatrix=m_RotMatrix*rot;

		//camera의 위치를 update한다.
		m_Pos=m_RotMatrix*camPos+m_Center;

		//camera의 방향을 update한다.
		m_Up=m_RotMatrix*camUp;
		//m_RotMatrix
	};

	void UpdateCamPos()
	{
		Vec3d camPos;
		Vec3d camUp;
		camPos[2]=m_Distance;
		camUp[1]=1;
		
		//회전된 각도에 따라서 rotation matrix를 정의한다.
		Mat3x3d rot;

		//rotation matrix를 계산한다.
		ComputeRotationMatrix(rot,m_RotAxis,m_RotAngle);
		if(m_RotAngle==0)
			rot.identity();

		m_RotMatrix=m_RotMatrix*rot;

		//camera의 위치를 update한다.
		m_Pos=m_RotMatrix*camPos+m_Center;

		//camera의 방향을 update한다.
		m_Up=m_RotMatrix*camUp;
		//m_RotMatrix.normalizeColVector();
	};

	void ComputeAxisAngle(Vec3d DMousePos)
	{
		//회전축을 정의한다.
		m_RotAxis[0]=-DMousePos[1];
		m_RotAxis[1]=DMousePos[0];
		m_RotAxis.normalize();

		//회전각도를 정의 한다.
		m_RotAngle=DMousePos.norm()/80.0;
	};

	void ComputeAxisAngle(Vec3d initialPos, Vec3d finalPos)
	{
		initialPos.normalize(); finalPos.normalize();
		Vec3d axis=initialPos.cross(finalPos);
		axis.normalize();
		double angle=acos(initialPos*finalPos);

		m_RotAxis=axis; m_RotAngle=angle;
	}

	void ComputeRotationMatrix(Mat3x3d& rot, Vec3d& axis, double angle)
	{
		double x,y,z;
		x=axis[0];
		y=axis[1];
		z=axis[2];

		rot(0,0)=x*x+(y*y+z*z)*cos(angle);
		rot(1,1)=y*y+(x*x+z*z)*cos(angle);
		rot(2,2)=z*z+(x*x+y*y)*cos(angle);
		rot(0,1)=(1-cos(angle))*x*y+z*sin(angle);
		rot(1,0)=(1-cos(angle))*x*y-z*sin(angle);
		rot(0,2)=(1-cos(angle))*x*z-y*sin(angle);
		rot(2,0)=(1-cos(angle))*z*x+y*sin(angle);
		rot(1,2)=(1-cos(angle))*y*z+x*sin(angle);
		rot(2,1)=(1-cos(angle))*z*y-x*sin(angle);
	};
};

#endif