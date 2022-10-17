/********************************************	
*		Original source code: SOFA			*
*		Modified by Hoeryong Jung			*
*		Date: 2009. 10. 21					*
*		contact: junghl80@kaist.ac.kr		*
/*******************************************/

#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "Vec.h"
#include "Mat.h"
#include <math.h>
#include <assert.h>
#include <iostream>

class BinaryTreeNode 
{
public:
	bool			End;
	BinaryTreeNode	*Descendant[2];
	int				NbDescendant;
	double			LeftEnd;
	double			RightEnd;

public:
	BinaryTreeNode()
	{
		End=false;
		for(int i=0;i<2;i++)
			Descendant[i]=NULL;
		LeftEnd=0;
		RightEnd=0;
		NbDescendant=2;
	};
	~BinaryTreeNode()
	{
	};

	void Init(double leftEnd, double rightEnd)
	{
		End=false;
		for(int i=0;i<2;i++)
			Descendant[i]=NULL;
		NbDescendant=2;
		LeftEnd=leftEnd;
		RightEnd=rightEnd;
	};
};

class BinaryTree
{
private:
	int MaxDepth;

public:
	BinaryTree()
	{
		MaxDepth=0;
	};

    ~BinaryTree()
	{
	};

	void init(int maxDepth)
	{
		MaxDepth=maxDepth;
	};
};

#endif
