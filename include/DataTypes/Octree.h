/********************************************	
*		Original source code: SOFA			*
*		Modified by Hoeryong Jung			*
*		Date: 2009. 10. 21					*
*		contact: junghl80@kaist.ac.kr		*
/*******************************************/

#ifndef OCTREE_H
#define OCTREE_H

#include "Vec.h"
#include "Mat.h"
#include <math.h>
#include <assert.h>
#include <iostream>

class OctreeNode 
{
public:
	bool		End;
	OctreeNode	*Descendant[8];
	int			NbDescendant;

	//bounding box
	Vec3d LeftDownPoint;
	Vec3d RightUpPoint;

public:
	OctreeNode()
	{
		End=false;
		for(int i=0;i<8;i++)
			Descendant[i]=NULL;
		NbDescendant=8;
	};
	~OctreeNode()
	{
	};

	void Init(Vec3d leftDownPoint, Vec3d rightUpPoint)
	{
		End=false;
		for(int i=0;i<8;i++)
			Descendant[i]=NULL;
		NbDescendant=8;
		LeftDownPoint=leftDownPoint;
		RightUpPoint=rightUpPoint;
	};

	void DrawBox()
	{
		glBegin(GL_LINES);
		glVertex3f((GLfloat)LeftDownPoint[0], (GLfloat)LeftDownPoint[1], (GLfloat)LeftDownPoint[2]);
		glVertex3f((GLfloat)RightUpPoint[0], (GLfloat)LeftDownPoint[1], (GLfloat)LeftDownPoint[2]);
		glVertex3f((GLfloat)RightUpPoint[0], (GLfloat)LeftDownPoint[1], (GLfloat)LeftDownPoint[2]);
		glVertex3f((GLfloat)RightUpPoint[0], (GLfloat)LeftDownPoint[1], (GLfloat)RightUpPoint[2]);
		glVertex3f((GLfloat)RightUpPoint[0], (GLfloat)LeftDownPoint[1], (GLfloat)RightUpPoint[2]);
		glVertex3f((GLfloat)LeftDownPoint[0], (GLfloat)LeftDownPoint[1], (GLfloat)RightUpPoint[2]);
		glVertex3f((GLfloat)LeftDownPoint[0], (GLfloat)LeftDownPoint[1], (GLfloat)RightUpPoint[2]);
		glVertex3f((GLfloat)LeftDownPoint[0], (GLfloat)LeftDownPoint[1], (GLfloat)LeftDownPoint[2]);

		glVertex3f((GLfloat)LeftDownPoint[0], (GLfloat)RightUpPoint[1], (GLfloat)LeftDownPoint[2]);
		glVertex3f((GLfloat)RightUpPoint[0], (GLfloat)RightUpPoint[1], (GLfloat)LeftDownPoint[2]);
		glVertex3f((GLfloat)RightUpPoint[0], (GLfloat)RightUpPoint[1], (GLfloat)LeftDownPoint[2]);
		glVertex3f((GLfloat)RightUpPoint[0], (GLfloat)RightUpPoint[1], (GLfloat)RightUpPoint[2]);
		glVertex3f((GLfloat)RightUpPoint[0], (GLfloat)RightUpPoint[1], (GLfloat)RightUpPoint[2]);
		glVertex3f((GLfloat)LeftDownPoint[0], (GLfloat)RightUpPoint[1], (GLfloat)RightUpPoint[2]);
		glVertex3f((GLfloat)LeftDownPoint[0], (GLfloat)RightUpPoint[1], (GLfloat)RightUpPoint[2]);
		glVertex3f((GLfloat)LeftDownPoint[0], (GLfloat)RightUpPoint[1], (GLfloat)LeftDownPoint[2]);

		glVertex3f((GLfloat)LeftDownPoint[0], (GLfloat)LeftDownPoint[1], (GLfloat)LeftDownPoint[2]);
		glVertex3f((GLfloat)LeftDownPoint[0], (GLfloat)RightUpPoint[1], (GLfloat)LeftDownPoint[2]);
		glVertex3f((GLfloat)RightUpPoint[0], (GLfloat)LeftDownPoint[1], (GLfloat)RightUpPoint[2]);
		glVertex3f((GLfloat)RightUpPoint[0], (GLfloat)RightUpPoint[1], (GLfloat)RightUpPoint[2]);
		glVertex3f((GLfloat)LeftDownPoint[0], (GLfloat)LeftDownPoint[1], (GLfloat)RightUpPoint[2]);
		glVertex3f((GLfloat)LeftDownPoint[0], (GLfloat)RightUpPoint[1], (GLfloat)RightUpPoint[2]);
		glVertex3f((GLfloat)RightUpPoint[0], (GLfloat)LeftDownPoint[1], (GLfloat)LeftDownPoint[2]);
		glVertex3f((GLfloat)RightUpPoint[0], (GLfloat)RightUpPoint[1], (GLfloat)LeftDownPoint[2]);
		glEnd();
	};
};

class Octree
{
private:
	int MaxDepth;

public:
	Octree()
	{
		MaxDepth=0;
	};

    ~Octree()
	{
	};

	void init(int maxDepth)
	{
		MaxDepth=maxDepth;
	};

	int GenOctree(Vec3d *point, int nbPoint, OctreeNode *root, Vec3d leftDown, Vec3d rightUp, int depth)
	{
		//node ÃÊ±âÈ­
		root->Init(leftDown, rightUp);

		if(depth==MaxDepth)
		{
			root->End=true;
			return 0;
		}

		if(IsPointInBox(point, nbPoint, leftDown, rightUp))
		{
			OctreeNode **descendant=new OctreeNode*[8];
			for(int i=0;i<8;i++)
			{
				descendant[i]=new OctreeNode;
				root->Descendant[i]=descendant[i];
			}

			depth++;
			Vec3d p1=leftDown;
			Vec3d p2=(leftDown+rightUp)/2.0;
			GenOctree(point, nbPoint, descendant[0], p1, p2, depth);  //LeftBottomBehind

			p1=leftDown+Vec3d((rightUp[0]-leftDown[0])/2.0,0,0);
			p2=Vec3d(rightUp[0], (leftDown[1]+rightUp[1])/2.0, (leftDown[2]+rightUp[2])/2.0);
			GenOctree(point, nbPoint, descendant[1], p1, p2, depth);  //RightBottomBehind

			p1=Vec3d(leftDown[0], leftDown[1], (leftDown[2]+rightUp[2])/2.0);
			p2=Vec3d((leftDown[0]+rightUp[0])/2.0, (leftDown[1]+rightUp[1])/2.0, rightUp[2]);
			GenOctree(point, nbPoint, descendant[2], p1, p2, depth);  //LeftBottomFront

			p1=Vec3d((leftDown[0]+rightUp[0])/2.0, leftDown[1], (leftDown[2]+rightUp[2])/2.0);
			p2=Vec3d(rightUp[0], (leftDown[1]+rightUp[1])/2.0, rightUp[2]) ;
			GenOctree(point, nbPoint, descendant[3], p1, p2, depth);  //RightBottomFront

			p1=Vec3d(leftDown[0], (leftDown[1]+rightUp[1])/2.0, leftDown[2]);
			p2=Vec3d((leftDown[0]+rightUp[0])/2.0, rightUp[1], (leftDown[2]+rightUp[2])/2.0);
			GenOctree(point, nbPoint, descendant[4], p1, p2, depth);  //LeftTopBehind

			p1=Vec3d((leftDown[0]+rightUp[0])/2.0, (leftDown[1]+rightUp[1])/2.0, leftDown[2]);
			p2=Vec3d(rightUp[0], rightUp[1], (leftDown[2]+rightUp[2])/2.0);
			GenOctree(point, nbPoint, descendant[5], p1, p2, depth);  //RightTopBehind

			p1=Vec3d(leftDown[0], (leftDown[1]+rightUp[1])/2.0, (leftDown[2]+rightUp[2])/2.0);
			p2=Vec3d((leftDown[0]+rightUp[0])/2.0, rightUp[1], rightUp[2]);
			GenOctree(point, nbPoint, descendant[6], p1, p2, depth);  //LeftBottomBehind

			p1=(leftDown+rightUp)/2.0;
			p2=rightUp;
			GenOctree(point, nbPoint, descendant[7], p1, p2, depth);  //LeftBottomBehind

			delete [] descendant;
		}
		else
		{
			root->End=true;
			return 0;
		}
		return 0;
	};

	bool IsPointInBox(Vec3d* point, int nbPoint, Vec3d leftDown, Vec3d rightUp)
	{
		for(int i=0;i<nbPoint;i++)
		{
			if((point[i][0]>leftDown[0])&&(point[i][0]<rightUp[0]))
			{
				if((point[i][1]>leftDown[1])&&(point[i][1]<rightUp[1]))
				{
					if((point[i][2]>leftDown[2])&&(point[i][2]<rightUp[2]))
						return true;
				}
			}
		}
		return false;
	};	

	int Draw(OctreeNode* root)
	{
		root->DrawBox();
		if(root->End)
			return 0;
		else
		{
			for(int i=0;i<8;i++)
				Draw(root->Descendant[i]);
		}
		return 0;
	};

	int DeleteOctree(OctreeNode* root)
	{
		if(root->End)
		{
			delete root;
			return 0;
		}
		else
		{
			for(int i=0;i<8;i++)
				DeleteOctree(root->Descendant[i]);
		}
		delete root;
		return 0;
	};
};

#endif
