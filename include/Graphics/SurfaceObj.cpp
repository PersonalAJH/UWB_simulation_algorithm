#include "pch.h"
#include "SurfaceObj.h"

SurfaceObj::SurfaceObj(void)
{
	FTime=fopen("SurfaceObjTime.txt","w");
	Point0=NULL;
	Point=NULL;
	Face=NULL;
	FaceNormal=NULL;
	PointNormal=NULL;
	Edge=NULL;
}

SurfaceObj::~SurfaceObj(void)
{
	fclose(FTime);
	if(Point0){delete Point0; Point0=NULL;}
	if(Point){delete Point; Point=NULL;}
	if(Face){delete Face; Face=NULL;}
	if(Edge){delete Edge; Edge=NULL;}
	if(FaceNormal){delete FaceNormal; FaceNormal=NULL;}
	if(PointNormal){delete PointNormal; PointNormal=NULL;}
}

void SurfaceObj::readSTLData(char* filename)
{
	FILE *fp;
	FILE *f;
	int i;
	int factor = 10;

	fp = fopen(filename,"r");
	
	//file의 문자들을 읽어오는 변수
	char str[100];
	CString temp;
	CString normal; normal.Format(_T("normal"));
	CString vertex; vertex.Format(_T("vertex"));
	CString endloop; endloop.Format(_T("endloop"));
	CString endsolid; endsolid.Format(_T("endsolid"));

	//면의 개수를 세고 x,y,z방향으로 max값과 min값을 구한다
	int faceCount=0;
	int maxX,maxY,maxZ;
	int minX,minY,minZ;
	int hashLength, hashMin, hashIndex;
	CString hashDirec;
	Vec3f tempPoint;

	maxX=maxY=maxZ=MIN;
	minX=minY=minZ=MAX;

	while(1)
	{
		fscanf(fp, "%s\n", str);
		temp=str;
		if(temp==endloop)
			faceCount++;
			
		//point좌표를 읽는다
		if(temp==vertex)
		{
			//x좌표
			fscanf(fp, "%s\n", str);
			temp=str;
			tempPoint[0]=_ttof(temp) * factor;
			if(tempPoint[0]>maxX)
				maxX=tempPoint[0];
			if(tempPoint[0]<minX)
				minX=tempPoint[0];

			//y좌표
			fscanf(fp, "%s\n", str);
			temp=str;
			tempPoint[1]=_ttof(temp) * factor;
			if(tempPoint[1]>maxY)
				maxY=tempPoint[1];
			if(tempPoint[1]<minY)
				minY=tempPoint[1];

			//z좌표
			fscanf(fp, "%s\n", str);
			temp=str;
			tempPoint[2]=_ttof(temp) * factor;
			if(tempPoint[2]>maxZ)
				maxZ=tempPoint[2];
			if(tempPoint[2]<minZ)
				minZ=tempPoint[2];
		}

		//file을 끝까지 읽으면 빠져나온다
		if(temp==endsolid)
		{
			//x,y,z방향중 object가 긴방향으로 hash table의 방향과 길이를 결정한다
			if((maxX-minX)>(maxY-minY))
			{
				if((maxX-minX)>(maxZ-minZ))
				{
					hashLength=maxX-minX;
					hashMin=minX;
					hashDirec="x";
				}
				else
				{
					hashLength=maxZ-minZ;
					hashMin=minZ;
					hashDirec="z";
				}
			}
			else
			{
				if((maxY-minY)>(maxZ-minZ))
				{
					hashLength=maxY-minY;
					hashMin=minY;
					hashDirec="y";
				}
				else
				{
					hashLength=maxZ-minZ;
					hashMin=minZ;
					hashDirec="z";
				}
			}
			break;
		}
	}
	fclose(fp);

	Face = new std::vector<Vec3i>;
	FaceNormal = new std::vector<Vec3f>;
	Face->resize(faceCount);
	FaceNormal->resize(faceCount);
	hashLength+=1;

	fp=fopen(filename,"r");

	//hash table에 메모리를 할당한다
	m_Hash=new Hash[hashLength];

	//hash table초기화
	for(i=0;i<hashLength;i++)
		m_Hash[i].PointNum=0;

	//임시로 point좌표를 저장하는 배열에 memory를 할당한다
	Vec3f *ptempPoint=new Vec3f[faceCount];
		
	//face normal vector와 point좌표를 저장한다
	int pointCount=0;
	faceCount=0;

	//현재 저장되어야할 point가 면의 어떤점인지 표시
	CString currentSavePoint; currentSavePoint.Format(_T("ii"));
	CString ii; CString jj; CString kk;
	ii.Format(_T("ii")); jj.Format(_T("jj")); kk.Format(_T("kk"));
	while(1)
	{
		fscanf(fp, "%s\n", str);
		temp=str;

		//face normal vector를 저장한다
		if(temp==normal)
		{
			//x direction
			fscanf(fp, "%s\n", str);
			temp=str;
			(*FaceNormal)[faceCount][0]=_ttof(temp);

			//y direction
			fscanf(fp, "%s\n", str);
			temp=str;
			(*FaceNormal)[faceCount][1]=_ttof(temp);

			//z direction
			fscanf(fp, "%s\n", str);
			temp=str;
			(*FaceNormal)[faceCount][2]=_ttof(temp);

			faceCount++;
		}

		//vertex좌표를 저장한다
		Vec3d comparePoint;
		int pointIndex;
		BOOL isSamePoint;
		if(temp==vertex)
		{
			//x 좌표
			fscanf(fp, "%s\n", str);
			temp=str;
			comparePoint[0]=_ttof(temp) * factor;

			//y 좌표
			fscanf(fp, "%s\n", str);
			temp=str;
			comparePoint[1]=_ttof(temp) * factor;

			//z 좌표
			fscanf(fp, "%s\n", str);
			temp=str;
			comparePoint[2]=_ttof(temp) * factor;
				
			isSamePoint=false;

			//읽어온 point가 저장되어야할 hash table의 index를 구한다
			if(hashDirec=="x")
				hashIndex=comparePoint[0]-hashMin;
			if(hashDirec=="y")
				hashIndex=comparePoint[1]-hashMin;
			if(hashDirec=="z")
				hashIndex=comparePoint[2]-hashMin;

			
			//이미 저장된 point들과 같은 point인지 검사, hash table안에서 검사
			for(i=0;i<m_Hash[hashIndex].PointNum;i++)
			{
				if(m_Hash[hashIndex].Point[i]==comparePoint)
				{
					pointIndex=m_Hash[hashIndex].PointIndex[i];
					isSamePoint=true;
					break;
				}
			}

			//이미 저장된 point들과 다를때만 point를 추가한다
			if(!isSamePoint)
			{
				//임시 point저장위치에 point를 저장한다
				ptempPoint[pointCount]=comparePoint;

				//hash table에 point를 저장한다
				m_Hash[hashIndex].Point[m_Hash[hashIndex].PointNum]=comparePoint;
				m_Hash[hashIndex].PointIndex[m_Hash[hashIndex].PointNum]=pointCount;
				m_Hash[hashIndex].PointNum++;
					
				if(currentSavePoint==ii)
				{
					(*Face)[faceCount-1][0]=pointCount;
					currentSavePoint="jj";
				}
				else if(currentSavePoint==jj)
				{
					(*Face)[faceCount-1][1]=pointCount;
					currentSavePoint="kk";
				}
				else
				{
					(*Face)[faceCount-1][2]=pointCount;
					currentSavePoint="ii";
				}
				pointCount++;
			}

			//이미 저장된 point이면 point에는 추가하지 않고 면정보에만 point번호를 저장
			else
			{
				if(currentSavePoint==ii)
				{
					(*Face)[faceCount-1][0]=pointIndex;
					currentSavePoint="jj";
				}
				else if(currentSavePoint==jj)
				{
					(*Face)[faceCount-1][1]=pointIndex;
					currentSavePoint="kk";
				}
				else
				{
					(*Face)[faceCount-1][2]=pointIndex;
					currentSavePoint="ii";
				}
			}

		}

		//file을 끝까지 읽으면 빠져나온다
		if(temp==endsolid)
		{
			//point좌표를 저장한다
			Point0=new std::vector<Vec3f>; Point0->resize(pointCount);
			Point = new std::vector<Vec3f>; Point->resize(pointCount);
			PointNormal=new std::vector<Vec3f>; PointNormal->resize(pointCount);
			for(i=0;i<pointCount;i++)
			{
				(*Point)[i]=(*Point0)[i]=ptempPoint[i];
			}
			break;
		}
	}
	fclose(fp);
		
	//hash table의 memory 해제
	if(m_Hash)
	{
		delete [] m_Hash;
		m_Hash=NULL;
	}

	computeFaceNormal();
	computeCenterPoint();
}

void SurfaceObj::readObjData(char* filename)
{
	FILE *fp;
	int i;
	fp = fopen(filename,"r");
	int pointNum=0;
	int faceNum=0;

	fscanf(fp,"%d\n",&pointNum);
	Point0=new std::vector<Vec3f>; Point0->resize(pointNum);
	Point = new std::vector<Vec3f>; Point->resize(pointNum);
	PointNormal=new std::vector<Vec3f>; PointNormal->resize(pointNum);

	for(i=0;i<pointNum;i++)
	{
		fscanf(fp,"%f %f %f\n",&(*Point)[i][0], &(*Point)[i][1], &(*Point)[i][2]);
		(*Point0)[i]=(*Point)[i];
	}

	fscanf(fp,"%d\n",&faceNum);
	Face = new std::vector<Vec3i>;
	FaceNormal = new std::vector<Vec3f>;
	Face->resize(faceNum);
	FaceNormal->resize(faceNum);

	for(i=0;i<faceNum;i++)
		fscanf(fp,"%d %d %d\n",&(*Face)[i][0], &(*Face)[i][1], &(*Face)[i][2]);
	Edge=new std::vector<Vec2i>;

	//face normal vector 계산
	computeFaceNormal();
	computeCenterPoint();

	fclose(fp);
}

void SurfaceObj::init(Vec3f* point, int nbPoint, Vec3i* face, int nbFace)
{
	Point0=new std::vector<Vec3f>; Point0->resize(nbPoint);
	Point = new std::vector<Vec3f>; Point->resize(nbPoint);
	PointNormal=new std::vector<Vec3f>; PointNormal->resize(nbPoint);

	for(int i=0;i<nbPoint;i++)
	{
		(*Point)[i]=point[i];
		(*Point0)[i]=(*Point)[i];
	}

	Face = new std::vector<Vec3i>;
	FaceNormal = new std::vector<Vec3f>;
	Face->resize(nbFace);
	FaceNormal->resize(nbFace);

	for(int i=0;i<nbFace;i++)
		(*Face)[i]=face[i];
	Edge=new std::vector<Vec2i>;

	//face normal vector 계산
	computeFaceNormal();
}

void SurfaceObj::writeObjData(char* filename)
{
	FILE *fp;
	int i;

	fp = fopen(filename,"w");

	fprintf(fp,"%d\n",Point->size());

	for(i=0;i<Point->size();i++)
		fprintf(fp,"%f %f %f\n",(*Point)[i][0], (*Point)[i][1], (*Point)[i][2]);

	fprintf(fp,"%d\n",Face->size());

	for(i=0;i<Face->size();i++)
		fprintf(fp,"%d %d %d\n",(*Face)[i][0], (*Face)[i][1], (*Face)[i][2]);

	fclose(fp);
}

void SurfaceObj::drawObject(bool mode, Vec3f color) 
{
	int i;
	glColor3f((GLfloat)color[0],(GLfloat)color[1],(GLfloat)color[2]);
	glBegin(GL_TRIANGLES);
	if(mode)
	{
		for(i=0;i<Face->size();i++)
		{
			glNormal3f((GLfloat)(*PointNormal)[(*Face)[i][0]][0], (GLfloat)(*PointNormal)[(*Face)[i][0]][1], (GLfloat)(*PointNormal)[(*Face)[i][0]][2]);
			glVertex3f((GLfloat)(*Point)[(*Face)[i][0]][0], (GLfloat)(*Point)[(*Face)[i][0]][1], (GLfloat)(*Point)[(*Face)[i][0]][2]);
			glNormal3f((GLfloat)(*PointNormal)[(*Face)[i][1]][0], (GLfloat)(*PointNormal)[(*Face)[i][1]][1], (GLfloat)(*PointNormal)[(*Face)[i][1]][2]);
			glVertex3f((GLfloat)(*Point)[(*Face)[i][1]][0], (GLfloat)(*Point)[(*Face)[i][1]][1], (GLfloat)(*Point)[(*Face)[i][1]][2]);
			glNormal3f((GLfloat)(*PointNormal)[(*Face)[i][2]][0], (GLfloat)(*PointNormal)[(*Face)[i][2]][1], (GLfloat)(*PointNormal)[(*Face)[i][2]][2]);
			glVertex3f((GLfloat)(*Point)[(*Face)[i][2]][0], (GLfloat)(*Point)[(*Face)[i][2]][1], (GLfloat)(*Point)[(*Face)[i][2]][2]);
		}
	}
	else
	{
		for(i=0;i<Face->size();i++)
		{
			glNormal3f(-(GLfloat)(*PointNormal)[(*Face)[i][0]][0], -(GLfloat)(*PointNormal)[(*Face)[i][0]][1], -(GLfloat)(*PointNormal)[(*Face)[i][0]][2]);
			glVertex3f((GLfloat)(*Point)[(*Face)[i][0]][0], (GLfloat)(*Point)[(*Face)[i][0]][1], (GLfloat)(*Point)[(*Face)[i][0]][2]);
			glNormal3f(-(GLfloat)(*PointNormal)[(*Face)[i][2]][0], -(GLfloat)(*PointNormal)[(*Face)[i][2]][1], -(GLfloat)(*PointNormal)[(*Face)[i][2]][2]);
			glVertex3f((GLfloat)(*Point)[(*Face)[i][2]][0], (GLfloat)(*Point)[(*Face)[i][2]][1], (GLfloat)(*Point)[(*Face)[i][2]][2]);
			glNormal3f(-(GLfloat)(*PointNormal)[(*Face)[i][1]][0], -(GLfloat)(*PointNormal)[(*Face)[i][1]][1], -(GLfloat)(*PointNormal)[(*Face)[i][1]][2]);
			glVertex3f((GLfloat)(*Point)[(*Face)[i][1]][0], (GLfloat)(*Point)[(*Face)[i][1]][1], (GLfloat)(*Point)[(*Face)[i][1]][2]);
		}
	}
	
	glEnd();
}

void SurfaceObj::drawObject(Vec3f color) 
{
	int i;
	glColor3f((GLfloat)color[0],(GLfloat)color[1],(GLfloat)color[2]);
	glBegin(GL_TRIANGLES);

	if(Face)
	{
		for(i=0;i<Face->size();i++)
		{
			glNormal3f((GLfloat)(*FaceNormal)[i][0], (GLfloat)(*FaceNormal)[i][1], (GLfloat)(*FaceNormal)[i][2]);
			glVertex3f((GLfloat)(*Point)[(*Face)[i][0]][0], (GLfloat)(*Point)[(*Face)[i][0]][1], (GLfloat)(*Point)[(*Face)[i][0]][2]);
			glVertex3f((GLfloat)(*Point)[(*Face)[i][1]][0], (GLfloat)(*Point)[(*Face)[i][1]][1], (GLfloat)(*Point)[(*Face)[i][1]][2]);
			glVertex3f((GLfloat)(*Point)[(*Face)[i][2]][0], (GLfloat)(*Point)[(*Face)[i][2]][1], (GLfloat)(*Point)[(*Face)[i][2]][2]);
			//glVertex3f((GLfloat)(*Point)[(*Face)[i][1]][0], (GLfloat)(*Point)[(*Face)[i][1]][1], (GLfloat)(*Point)[(*Face)[i][1]][2]);
		}
	}
	glEnd();
}

void SurfaceObj::drawWireFrame(Vec3f color) 
{
	int i;
	glLineWidth(1.0);
	glColor3f((GLfloat)color[0],(GLfloat)color[1],(GLfloat)color[2]);
	glBegin(GL_LINES);
	for (i = 0; i<Face->size(); i++)
	{
		glVertex3f((GLfloat)(*Point)[(*Face)[i][0]][0], (GLfloat)(*Point)[(*Face)[i][0]][1], (GLfloat)(*Point)[(*Face)[i][0]][2]);
		glVertex3f((GLfloat)(*Point)[(*Face)[i][1]][0], (GLfloat)(*Point)[(*Face)[i][1]][1], (GLfloat)(*Point)[(*Face)[i][1]][2]);
		
		glVertex3f((GLfloat)(*Point)[(*Face)[i][1]][0], (GLfloat)(*Point)[(*Face)[i][1]][1], (GLfloat)(*Point)[(*Face)[i][1]][2]);
		glVertex3f((GLfloat)(*Point)[(*Face)[i][2]][0], (GLfloat)(*Point)[(*Face)[i][2]][1], (GLfloat)(*Point)[(*Face)[i][2]][2]);

		glVertex3f((GLfloat)(*Point)[(*Face)[i][2]][0], (GLfloat)(*Point)[(*Face)[i][2]][1], (GLfloat)(*Point)[(*Face)[i][2]][2]);
		glVertex3f((GLfloat)(*Point)[(*Face)[i][0]][0], (GLfloat)(*Point)[(*Face)[i][0]][1], (GLfloat)(*Point)[(*Face)[i][0]][2]);
	}
	/*for(int i=0;i<Edge->size();i++)
	{
		glVertex3f((GLfloat)(*Point)[(*Edge)[i][0]][0],(GLfloat)(*Point)[(*Edge)[i][0]][1],(GLfloat)(*Point)[(*Edge)[i][0]][2]);
		glVertex3f((GLfloat)(*Point)[(*Edge)[i][1]][0],(GLfloat)(*Point)[(*Edge)[i][1]][1],(GLfloat)(*Point)[(*Edge)[i][1]][2]);
	}*/
	glEnd();
}

void SurfaceObj::computeFaceNormal()
{
	int i;
	Vec3f vector1, vector2;

	for(i=0;i<FaceNormal->size();i++)
	{
		vector1=(*Point)[(*Face)[i][1]]-(*Point)[(*Face)[i][0]]; 
		vector2=(*Point)[(*Face)[i][2]]-(*Point)[(*Face)[i][0]]; 
		(*FaceNormal)[i] = vector1.cross(vector2);
		(*FaceNormal)[i].normalize();
	}
	int a = 0;
	//computePointNormal();
}

Vec3f SurfaceObj::computeFaceNormal(Vec3f p1, Vec3f p2, Vec3f p3)
{
	int i;
	Vec3f vector1, vector2;
	Vec3f normal;
	vector1=p2-p1; 
	vector2=p3-p1; 
	normal = vector1.cross(vector2);
	normal.normalize();
	return normal;
}

void SurfaceObj::computePointNormal()
{
}

std::vector<Vec3f>*	SurfaceObj::point0()
{
	return Point0;
}
std::vector<Vec3f>*	SurfaceObj::point()
{
	return Point;
}
std::vector<Vec3i>*	SurfaceObj::face()
{
	return Face;
}
std::vector<Vec2i>*	SurfaceObj::edge()
{
	return Edge;
}
std::vector<Vec3f>*	SurfaceObj::faceNormal()
{
	return FaceNormal;
}
std::vector<Vec3f>* SurfaceObj::pointNormal()
{
	return PointNormal;
}
Vec3f SurfaceObj::midPoint()
{
	return MidPoint;
}

void SurfaceObj::rotate(Vec3f axis, float angle)
{
	Mat3x3f rot;
	computeRotationMatrix(axis, angle, rot);

	for(int i=0;i<Point->size();i++)
		(*Point)[i]=rot*(*Point0)[i];
	computeFaceNormal();
	computeCenterPoint();
}

void SurfaceObj::rotateC(Vec3f axis, float angle)
{
	Mat3x3f rot;
	computeRotationMatrix(axis, angle, rot);

	for(int i=0;i<Point->size();i++)
		(*Point)[i]=rot*(*Point)[i];
	computeFaceNormal();
	computeCenterPoint();
};

void SurfaceObj::rotate(float* rot)
{
	Mat3x3f Rot;
	Rot(0,0)=rot[0]; Rot(0,1)=rot[1]; Rot(0,2)=rot[2];
	Rot(1,0)=rot[4]; Rot(1,1)=rot[5]; Rot(1,2)=rot[6];
	Rot(2,0)=rot[8]; Rot(2,1)=rot[9]; Rot(2,2)=rot[10];
	rotate(Rot);
	computeCenterPoint();
	computeFaceNormal();
};

void SurfaceObj::rotate(Quat q)
{
	for(int i=0;i<Point->size();i++)
		(*Point)[i]=q.rotate((*Point0)[i]);
	computeCenterPoint();
	computeFaceNormal();
};

void SurfaceObj::rotateC(Quat q)
{
	for(int i=0;i<Point->size();i++)
		(*Point)[i]=q.rotate((*Point)[i]);
	computeCenterPoint();
	computeFaceNormal();
};

void SurfaceObj::rotateC(Mat3x3f rot)
{
	for(int i=0;i<Point->size();i++)
		(*Point)[i]=rot*(*Point)[i];
	computeCenterPoint();
	computeFaceNormal();
};

void SurfaceObj::rotate(Mat3x3f& rot)
{
	for(int i=0;i<Point->size();i++)
		(*Point)[i]=rot*(*Point0)[i];
	computeFaceNormal();
	computeCenterPoint();
};

void SurfaceObj::scale(float x, float y, float z)
{
	int i;
	for(i=0;i<Point->size();i++)
	{
		(*Point)[i][0]=(*Point0)[i][0]*x;
		(*Point)[i][1]=(*Point0)[i][1]*y;
		(*Point)[i][2]=(*Point0)[i][2]*z;
	}
	computeFaceNormal();
	computeCenterPoint();
};

void SurfaceObj::scale(float scale)
{
	int i;
	for(i=0;i<Point->size();i++)
	{
		(*Point)[i]=(*Point0)[i]*scale;
	}
	computeFaceNormal();
	computeCenterPoint();
};

void SurfaceObj::scaleC(float scale)
{
	int i;
	for(i=0;i<Point->size();i++)
	{
		(*Point)[i]=(*Point)[i]*scale;
	}
	computeFaceNormal();
	computeCenterPoint();
};

void SurfaceObj::scaleC(float x, float y, float z)
{
	int i;
	for(i=0;i<Point->size();i++)
	{
		(*Point)[i][0]=(*Point)[i][0]*x;
		(*Point)[i][1]=(*Point)[i][1]*y;
		(*Point)[i][2]=(*Point)[i][2]*z;
	}
};

void SurfaceObj::scaleI(float x, float y, float z)
{
	int i;
	for(i=0;i<Point->size();i++)
	{
		(*Point0)[i][0]=(*Point0)[i][0]*x;
		(*Point0)[i][1]=(*Point0)[i][1]*y;
		(*Point0)[i][2]=(*Point0)[i][2]*z;
	}
};

void SurfaceObj::scaleI(float scale)
{
	int i;
	for(i=0;i<Point->size();i++)
	{
		(*Point0)[i]=(*Point0)[i]*scale;
	}
};

void SurfaceObj::translate(Vec3f trans)
{
	int i;

	for(i=0;i<Point->size();i++)
		(*Point)[i]=(*Point0)[i]+trans;
	computeCenterPoint();
};

void SurfaceObj::translateC(Vec3f trans)
{
	int i;

	for(i=0;i<Point->size();i++)
		(*Point)[i]=(*Point)[i]+trans;
	computeCenterPoint();
};

void SurfaceObj::translate(float x, float y, float z)
{
	int i;
	Vec3f trans(x,y,z);

	for(i=0;i<Point->size();i++)
		(*Point)[i]=(*Point0)[i]+trans;
	computeCenterPoint();
};

void SurfaceObj::translateI(Vec3f trans)
{
	int i;

	for(i=0;i<Point->size();i++)
	{
		(*Point0)[i]=(*Point0)[i]+trans;
		(*Point)[i]=(*Point0)[i];
	}
};

void SurfaceObj::translateI(float x, float y, float z)
{
	int i;
	Vec3f trans(x,y,z);

	for(i=0;i<Point->size();i++)
	{
		(*Point0)[i]=(*Point0)[i]+trans;
		(*Point)[i]=(*Point0)[i];
	}
};

void SurfaceObj::moveToCenter()
{
	translate(MidPoint*-1);
	MidPoint.clear();
}

void SurfaceObj::computeRotationMatrix(Vec3f axis, float angle, Mat3x3f& rot)
{
	float x,y,z;
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

	rot.transpose();
};

void SurfaceObj::computeCenterPoint()
{
	int i;
	Vec3f sum;
	for(i=0;i<Point->size();i++)
		sum=sum+(*Point)[i];
	sum=sum/Point->size();
	MidPoint=sum;
}