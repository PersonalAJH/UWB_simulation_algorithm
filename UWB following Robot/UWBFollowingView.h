
// UWBFollowingView.h: CUWBFollowingView 클래스의 인터페이스
//

#pragma once

#include <random>

#define NB_SAMPLE 15

class CUWBFollowingView : public CView
{
protected: // serialization에서만 만들어집니다.
	CUWBFollowingView() noexcept;
	DECLARE_DYNCREATE(CUWBFollowingView)

// 특성입니다.
public:
	CUWBFollowingDoc* GetDocument() const;

// 작업입니다.
public:
	void InitGL();
	VOID GlPrint(const char* text);

	//Drawing
	void DrawView();
	void SetupView();
	void UpdateView();
	void drawPlane();
	void drawCircle(float radius);
	Vec3f ComputeTrilaterationCoord(float* dist, Vec3f* anchor, int nbAnchor);
	Vec3f ComputeTrilaterationCoord2(float* dist, Vec3f* anchor, int nbAnchor);
	Vec3f ComputeTrilaterationCoord3(float* dist, Vec3f* anchor, int nbAnchor);
	void UpdateDistance();

private:
	CSerialComm SerialComm;
	SurfaceObj m_Plane;

// variables
protected:
	HDC     m_hDC;
	HGLRC   m_hRC;
	GLuint	base;

	//shader id
	GLuint m_ShaderProg;

	//flag
	bool LEFT_DOWN;
	bool RIGHT_DOWN;

	//window크기
	int m_WindowHeight;
	int m_WindowWidth;

	//camera
	CCamera m_Cam;

	//mouse position
	Vec3d m_MousePos;
	Vec3d m_PreMousePos;
	Vec3d m_DMousePos;

	float m_Dis[4];
	float m_DisMeasure[4];
	Vec3f m_AnchorPos[4];
	Vec3f m_Tag;
	Vec3f m_TagCalc;
	Vec3f m_TagCalcPre;
	Vec3f m_TagCalcVec[NB_SAMPLE];
	int Counter;

	CSerialComm m_Serial;

	std::default_random_engine m_Generator;
	std::normal_distribution<float>* m_Distribution[4];

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CUWBFollowingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // UWBFollowingView.cpp의 디버그 버전
inline CUWBFollowingDoc* CUWBFollowingView::GetDocument() const
   { return reinterpret_cast<CUWBFollowingDoc*>(m_pDocument); }
#endif

