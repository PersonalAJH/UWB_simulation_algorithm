// TimeTick.h: interface for the CTimeTick class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMETICK_H__CC98BC27_54B5_49F4_8E49_1B5C97D0E157__INCLUDED_)
#define AFX_TIMETICK_H__CC98BC27_54B5_49F4_8E49_1B5C97D0E157__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.
#include <afxdisp.h>        // MFC 자동화 클래스입니다.

class CTimeTick  
{
public:
	double GetTick();
	void SetEnd();
	void SetStart();
	LARGE_INTEGER iPerfFreq;
	LARGE_INTEGER iPerfStart;
	LARGE_INTEGER iPerfEnd;
	CTimeTick();
	virtual ~CTimeTick();

};

extern CTimeTick g_Time;

#endif // !defined(AFX_TIMETICK_H__CC98BC27_54B5_49F4_8E49_1B5C97D0E157__INCLUDED_)
