// TimeTick.cpp: implementation of the CTimeTick class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TimeTick.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CTimeTick g_Time;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeTick::CTimeTick()
{
	QueryPerformanceFrequency(&iPerfFreq);
}

CTimeTick::~CTimeTick()
{

}

void CTimeTick::SetStart()
{
	QueryPerformanceCounter(&iPerfStart);  // start point
}

void CTimeTick::SetEnd()
{
	QueryPerformanceCounter(&iPerfEnd);    // end point
}

double CTimeTick::GetTick()
{
	return  ((double(iPerfEnd.QuadPart - iPerfStart.QuadPart)*1000)/double(iPerfFreq.QuadPart)); //msec로 반환
//	return  double(iPerfEnd.QuadPart - iPerfStart.QuadPart)/double(iPerfFreq.QuadPart); //sec로 반환
}
