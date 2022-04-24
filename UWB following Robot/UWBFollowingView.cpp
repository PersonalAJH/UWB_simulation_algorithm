
// UWBFollowingView.cpp: CUWBFollowingView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "UWBFollowing.h"
#endif

#include "UWBFollowingDoc.h"
#include "UWBFollowingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUWBFollowingView

IMPLEMENT_DYNCREATE(CUWBFollowingView, CView)

BEGIN_MESSAGE_MAP(CUWBFollowingView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CUWBFollowingView 생성/소멸

CUWBFollowingView::CUWBFollowingView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CUWBFollowingView::~CUWBFollowingView()
{
}

BOOL CUWBFollowingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CUWBFollowingView 그리기

void CUWBFollowingView::OnDraw(CDC* /*pDC*/)
{
	CUWBFollowingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	wglMakeCurrent(m_hDC, m_hRC);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawView();
	SwapBuffers(m_hDC);
}


// CUWBFollowingView 인쇄

BOOL CUWBFollowingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CUWBFollowingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CUWBFollowingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CUWBFollowingView 진단

#ifdef _DEBUG
void CUWBFollowingView::AssertValid() const
{
	CView::AssertValid();
}

void CUWBFollowingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CUWBFollowingDoc* CUWBFollowingView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUWBFollowingDoc)));
	return (CUWBFollowingDoc*)m_pDocument;
}
#endif //_DEBUG


// CUWBFollowingView 메시지 처리기


void CUWBFollowingView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	LEFT_DOWN = false;
	RIGHT_DOWN = false;
	SetTimer(1, 10, NULL);
	InitGL();
	float width = 60;
	float height = 50;
	m_AnchorPos[0] = Vec3f(-width / 2.0, height / 2.0, 30);
	m_AnchorPos[1] = Vec3f(width / 2.0, height / 2.0, 30);
	m_AnchorPos[2] = Vec3f(width / 2.0, -height / 2.0, 30);
	m_AnchorPos[3] = Vec3f(-width / 2.0, -height / 2.0, 30);
	m_Tag[1] = 500;
	m_Plane.readObjData("../data/plane.txt");

	if (m_Serial.connect("COM5"))
		printf("Serial port is connected!!\n");
	else
		printf("Sercial connection fail!!\n");

	for (int i = 0; i < 4; i++)
	{
		float sdev = 10;
		m_Dis[i] = (m_AnchorPos[i] - m_Tag).norm();
		m_Distribution[i] = new std::normal_distribution<float>(m_Dis[i], sdev);
	}
	Counter = 0;
}


void CUWBFollowingView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	char lsChar;
	lsChar = char(nChar);

	float var = 10;
	if (lsChar == 'Q')
	{

	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CUWBFollowingView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	LEFT_DOWN = true;
	CView::OnLButtonDown(nFlags, point);
}


void CUWBFollowingView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	LEFT_DOWN = false;
	CView::OnLButtonUp(nFlags, point);
}


void CUWBFollowingView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_MousePos[0] = point.x;
	m_MousePos[1] = -point.y;
	m_DMousePos = m_MousePos - m_PreMousePos;

	if (LEFT_DOWN)
		m_Cam.RotCamPos(m_DMousePos);
	if (RIGHT_DOWN)
		m_Cam.MoveCamPos(m_DMousePos);
	m_PreMousePos = m_MousePos;
	CView::OnMouseMove(nFlags, point);
	
}


BOOL CUWBFollowingView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	Vec3d temp;
	m_Cam.m_Distance += zDelta * 0.1;
	m_Cam.RotCamPos(temp);
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CUWBFollowingView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	RIGHT_DOWN = true;
	CView::OnRButtonDown(nFlags, point);
}


void CUWBFollowingView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	RIGHT_DOWN = false;
	CView::OnRButtonUp(nFlags, point);
}


void CUWBFollowingView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSize size(cx, cy);
	m_WindowHeight = size.cy;
	m_WindowWidth = size.cx;
}


void CUWBFollowingView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//m_Serial.readCommand(data);
	char data[100];
	char* AnchorData[4];
	float Distance[4];
	memset(data, 0, 100);

	m_Serial.readCommand(data);
	printf("%s\n", data);
	AnchorData[0] = strtok(data, ",");
	if (AnchorData[0] == NULL) return;
	AnchorData[1] = strtok(NULL, ",");
	if (AnchorData[1] == NULL) return;
	AnchorData[2] = strtok(NULL, ",");
	if (AnchorData[2] == NULL) return;
	AnchorData[3] = strtok(NULL, ",");
	if (AnchorData[3] == NULL) return;

	//UpdateDistance(); // 아래 코드로 대체
	for ( int i = 0 ; i < 4 ; i++)
		m_DisMeasure[i] = atof(AnchorData[i]+9)*100;

	m_TagCalc=ComputeTrilaterationCoord3(m_DisMeasure, m_AnchorPos, 4);

	if (Counter < NB_SAMPLE)
	{
		if((m_TagCalcPre-m_TagCalc).norm()>1000)
			m_TagCalcVec[Counter] = m_TagCalcPre;
		else
			m_TagCalcVec[Counter] = m_TagCalc;
		Vec3f sum;
		for (int i = 0; i < Counter; i++)
			sum += m_TagCalcVec[i];
		m_TagCalc = sum / (float)Counter;
	}
	else
	{
		if ((m_TagCalcPre - m_TagCalc).norm() > 1000)
			m_TagCalcVec[Counter % NB_SAMPLE] = m_TagCalcPre;
		else
			m_TagCalcVec[Counter % NB_SAMPLE] = m_TagCalc;
		Vec3f sum;
		for (int i = 0; i < NB_SAMPLE; i++)
			sum += m_TagCalcVec[i];
		m_TagCalc = sum / (float)NB_SAMPLE;
	}
	if (m_TagCalc.norm() > 200)
		int a = 0;
	m_TagCalcPre = m_TagCalc;
	Counter++;
	InvalidateRect(NULL, FALSE);
	CView::OnTimer(nIDEvent);
}

void CUWBFollowingView::UpdateDistance()
{
	for (int i = 0; i < 4; i++)
		m_DisMeasure[i] = (*m_Distribution[i])(m_Generator);
}

void CUWBFollowingView::InitGL()
{
	COpenGL Initgl;

	//Initgl에 windows handle을 넘겨준다
	Initgl.SetHWND(m_hWnd);
	Initgl.SetupPixelFormat();
	base = Initgl.base;

	m_hDC = Initgl.m_hDC;
	m_hRC = Initgl.m_hRC;

	//Initgl.SetupShader("../shader/local");
	//m_ShaderProg = Initgl.GetProgLog();
	//glUniform3f(glGetUniformLocation(m_ShaderProg, "LightPosition"), 1000.0, 1000.0, 1000.0);
}
void CUWBFollowingView::DrawView()
{
	GLUquadricObj* qobj = 0;
	qobj = gluNewQuadric();
	glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT);
	glPushMatrix();
	SetupView();
	UpdateView();
	drawPlane();
	glColor3f(1, 0, 0);
	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		glTranslatef(m_AnchorPos[i][0], m_AnchorPos[i][1], m_AnchorPos[i][2]);
		gluSphere(qobj, 10, 20, 20);
		glPopMatrix();
	}

	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(m_Tag[0], m_Tag[1], m_Tag[2]);
	gluSphere(qobj, 10, 20, 20);
	glPopMatrix();

	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(m_TagCalc[0], m_TagCalc[1], m_TagCalc[2]);
	gluSphere(qobj, 20, 20, 20);
	glPopMatrix();

	glPopMatrix();
	glPopAttrib();
}

void CUWBFollowingView::SetupView()
{
	GLfloat diffuseLight[] = { 0.4f,0.4f,0.4f,1.0f };
	GLfloat ambientLight[] = { 0.2f,0.2f,0.2f,1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//	glEnable(GL_BLEND);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glEnable(GL_LIGHT0);

	glFrontFace(GL_CCW);
	//glFrontFace(GL_CW);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT, GL_FILL);
}

void CUWBFollowingView::UpdateView()
{
	glViewport(0, 0, m_WindowWidth, m_WindowHeight);
	float fovy = 45;
	float aspect = float(m_WindowWidth) / float(m_WindowHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(fovy, aspect, 1, 10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(m_Cam.m_Pos[0], m_Cam.m_Pos[1], m_Cam.m_Pos[2],
		m_Cam.m_Center[0], m_Cam.m_Center[1], m_Cam.m_Center[2],
		m_Cam.m_Up[0], m_Cam.m_Up[1], m_Cam.m_Up[2]);
}

void CUWBFollowingView::drawPlane()
{
	int nbLines = 300;
	float width = 50;
	float z = 5;
	//m_Plane.drawObject(Vec3f(1, 1, 1));
	glColor3f(0, 0, 0);
	for (int i = 0; i < 50; i++)
	{
		if (i % 5 == 0)
			glLineWidth(2.0);
		else
			glLineWidth(1.0);
		drawCircle(i*100);
	}

	Vec3f direc(10000, 0, 200);
	float ds = PI / 6.0;
	Mat3x3f rot; rot.setRotationMatrix(Vec3d(0, 0, 1), ds);

	glBegin(GL_LINES);
	for (int i = 0; i < 12; i++)
	{
		glVertex3f(0, 0, 0);
		glVertex3f(direc[0], direc[1], direc[2]);
		direc = rot*direc;
	}
	glEnd();
}

void CUWBFollowingView::drawCircle(float radius)
{
	Vec3f direc(radius, 0, 10);
	int nbSeg=500;
	float ds = PI / (float)nbSeg*2;
	Mat3x3f rot; rot.setRotationMatrix(Vec3d(0, 0, 1), ds);
	glBegin(GL_LINES);
	for (int i = 0; i < nbSeg; i++)
	{
		glVertex3f(direc[0], direc[1], direc[2]);
		direc = rot * direc;
		glVertex3f(direc[0], direc[1], direc[2]);
	}
	glEnd();
}

Vec3f CUWBFollowingView::ComputeTrilaterationCoord(float* dist, Vec3f* anchor, int nbAnchor)
{
	// dist: float[n] 측정거리 (anchor station number i with mobile station, i = 0, 1, 2, ... , n)
	// anchor: Vec3f[n] anchor 좌표 (Vec3f[0] = Vec3f(x_crd_f, y_crd_f, z_crd_f))
	// nbAnchor: anchor 갯수

	Vec3f coord(0, 0, 30);

	Mat<4, 4, float> a;
	a[0] = Vec4f(anchor[0].x(), anchor[0].y(), anchor[0].z(), -0.5);
	a[1] = Vec4f(anchor[1].x(), anchor[1].y(), anchor[1].z(), -0.5);
	a[2] = Vec4f(anchor[2].x(), anchor[2].y(), anchor[2].z(), -0.5);
	a[3] = Vec4f(anchor[3].x(), anchor[3].y(), anchor[3].z(), -0.5);

	Vec4f b;
	b[0] = 0.5 * (powf(anchor[0].x(), 2.) + powf(anchor[0].y(), 2.) + powf(anchor[0].z(), 2.) - powf(dist[0], 2.));
	b[1] = 0.5 * (powf(anchor[1].x(), 2.) + powf(anchor[1].y(), 2.) + powf(anchor[1].z(), 2.) - powf(dist[1], 2.));
	b[2] = 0.5 * (powf(anchor[2].x(), 2.) + powf(anchor[2].y(), 2.) + powf(anchor[2].z(), 2.) - powf(dist[2], 2.));
	b[3] = 0.5 * (powf(anchor[3].x(), 2.) + powf(anchor[3].y(), 2.) + powf(anchor[3].z(), 2.) - powf(dist[3], 2.));

	Mat4x4f aa = a.transposed() * a;

	Mat<4, 4, float> ata_inverse;
	invertMatrix(ata_inverse, a.transposed() * a);

	Vec4f theta = ata_inverse * a.transposed() * b;
	coord.x() = theta.x();
	coord.y() = theta.y();

	return coord;
}

Vec3f CUWBFollowingView::ComputeTrilaterationCoord2(float* dist, Vec3f* anchor, int nbAnchor)
{
	// dist: float[n] 측정거리 (anchor station number i with mobile station, i = 0, 1, 2, ... , n)
	// anchor: Vec3f[n] anchor 좌표 (Vec3f[0] = Vec3f(x_crd_f, y_crd_f, z_crd_f))
	// nbAnchor: anchor 갯수

	Vec3f coord(0, 0, 30);

	Mat<3, 3, float> a;
	a[0] = Vec4f(anchor[0].x(), anchor[0].y(), -0.5);
	a[1] = Vec4f(anchor[1].x(), anchor[1].y(), -0.5);
	a[2] = Vec4f(anchor[2].x(), anchor[2].y(), -0.5);

	Vec3f b;
	b[0] = 0.5 * (powf(anchor[0].x(), 2.) + powf(anchor[0].y(), 2.) - powf(dist[0], 2.));
	b[1] = 0.5 * (powf(anchor[1].x(), 2.) + powf(anchor[1].y(), 2.) - powf(dist[1], 2.));
	b[2] = 0.5 * (powf(anchor[2].x(), 2.) + powf(anchor[2].y(), 2.) - powf(dist[2], 2.));

	Mat3x3f aa = a.transposed() * a;

	Mat<3, 3, float> ata_inverse;
	invertMatrix(ata_inverse, a.transposed() * a);

	Vec3f c = a.multTranspose(b);
	Vec3f theta = ata_inverse * c;
	coord.x() = theta.x();
	coord.y() = theta.y();

	return coord;
}

Vec3f CUWBFollowingView::ComputeTrilaterationCoord3(float* dist, Vec3f* anchor, int nbAnchor)
{
	// dist: float[n] 측정거리 (anchor station number i with mobile station, i = 0, 1, 2, ... , n)
	// anchor: Vec3f[n] anchor 좌표 (Vec3f[0] = Vec3f(x_crd_f, y_crd_f, z_crd_f))
	// nbAnchor: anchor 갯수

	Vec3f coord(0, 0, 30);

	Mat<4, 3, float> a;
	a[0] = Vec4f(anchor[0].x(), anchor[0].y(), -0.5);
	a[1] = Vec4f(anchor[1].x(), anchor[1].y(), -0.5);
	a[2] = Vec4f(anchor[2].x(), anchor[2].y(), -0.5);
	a[3] = Vec4f(anchor[3].x(), anchor[3].y(), -0.5);

	Vec4f b;
	b[0] = 0.5 * (powf(anchor[0].x(), 2.) + powf(anchor[0].y(), 2.) - powf(dist[0], 2.));
	b[1] = 0.5 * (powf(anchor[1].x(), 2.) + powf(anchor[1].y(), 2.) - powf(dist[1], 2.));
	b[2] = 0.5 * (powf(anchor[2].x(), 2.) + powf(anchor[2].y(), 2.) - powf(dist[2], 2.));
	b[3] = 0.5 * (powf(anchor[3].x(), 2.) + powf(anchor[3].y(), 2.) - powf(dist[3], 2.));


	Mat3x3f aa = a.transposed() * a;

	Mat<3, 3, float> ata_inverse;
	invertMatrix(ata_inverse, a.transposed() * a);

	Vec3f c = a.multTranspose(b);
	Vec3f theta = ata_inverse * c;
	coord.x() = theta.x();
	coord.y() = theta.y();

	return coord;
}