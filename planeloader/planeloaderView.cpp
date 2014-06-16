
/**************************************
Create: 6/11/2014 22:33:40
Author: Lee
Function: 
		Demo to load 3ds file and rotate 
		the object.
***************************************/

#include "stdafx.h"
#include "planeloader.h"
#include "planeloaderDoc.h"
#include "planeloaderView.h"
#include "3DSREADER.H"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define TIMER_ROTATE		2014
#define TIMER_FLY_IN_RECT	2015

#define START_X_POS			-1000
#define MAX_X_POS			2000
#define START_Z_POS			-4000
#define MAX_Z_POS			-6000
#define START_Y_POS			-10
#define SCALE_FACTOR		0.05
#define MOVE_Y_OFFSET		-200

IMPLEMENT_DYNCREATE(CplaneloaderView, CView)

BEGIN_MESSAGE_MAP(CplaneloaderView, CView)
	// Standard printing commands
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_TIMER()
	
	ON_COMMAND(ID_ROTATE_X, &CplaneloaderView::OnRotateX)
	ON_COMMAND(ID_ROTATE_Y, &CplaneloaderView::OnRotateY)
	ON_COMMAND(ID_ROTATE_Z, &CplaneloaderView::OnRotateZ)
	ON_UPDATE_COMMAND_UI(ID_ROTATE_X, &CplaneloaderView::OnUpdateRotateX)
	ON_UPDATE_COMMAND_UI(ID_ROTATE_Y, &CplaneloaderView::OnUpdateRotateY)
	ON_UPDATE_COMMAND_UI(ID_ROTATE_Z, &CplaneloaderView::OnUpdateRotateZ)

	ON_COMMAND(ID_ROTATE_FLYINRECT, &CplaneloaderView::OnRotateFlyinrect)
	ON_UPDATE_COMMAND_UI(ID_ROTATE_FLYINRECT, &CplaneloaderView::OnUpdateRotateFlyinrect)
END_MESSAGE_MAP()

CplaneloaderView::CplaneloaderView()
{
	m_fAngleX = 0.0;
	m_fAngleY = 0.0;
	m_fAngleZ = 0.0;
	m_bRatateXEnable = FALSE;
	m_bRatateYEnable = FALSE;
	m_bRatateZEnable = FALSE;

	m_bFlyWithRect = FALSE;
	m_fFlyStep = 0.0;
	m_nDirection = 0;
	m_bDrawDir0 = FALSE;
	m_bDrawDir1 = FALSE;
	m_bDrawDir2 = FALSE;
	m_bDrawDir3 = FALSE;
}

CplaneloaderView::~CplaneloaderView()
{
}

BOOL CplaneloaderView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

int CplaneloaderView::OnCreate(LPCREATESTRUCT lpcs)
{
	if (CView::OnCreate(lpcs) == -1)
		return -1;

	InitOpenGL();
	InitLight();
	LoadPlaneModel();
	SetTimer(TIMER_ROTATE, 2000, NULL);
	
	return 0;
}

void CplaneloaderView::InitLight()
{
	//下面一行必须要加上，否则飞机的材质不能表现。
	glEnable( GL_COLOR_MATERIAL); 
	glShadeModel(GL_SMOOTH);

	//设置light0
	GLfloat ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat diffuse[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat specular[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat position[] = { 1.0f,-8.0f,-100.0f,0.0 };//light0 位置
	GLfloat lmodel_ambient[] = { 0.2, 0.7, 0.5, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);

}

BOOL CplaneloaderView::LoadPlaneModel()
{
	C3dsReader Loader;
	if (m_planeModel.getNumObjects() > 0)
	{
		m_planeModel.removeAllObjects();
	}
	m_bPlaneLoaded = Loader.Reader("ModelData\\F16plane.3DS", &m_planeModel);
	m_planeModel.doAfterMath();

	return m_bPlaneLoaded;
}

void CplaneloaderView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_ROTATE)
	{
		m_fAngleX += 90.0f;
		m_fAngleY += 90.0f;
		m_fAngleZ += 90.0f;
	}
	else if (nIDEvent == TIMER_FLY_IN_RECT)
	{
		if (m_nDirection == 0)
		{
			//-x---->+x
			//已经到最右边的X最大值处了。调转方向====>m_nDirection=1
			if (m_fFlyStep + START_X_POS >= MAX_X_POS)
			{
				m_bDrawDir0 = TRUE;
				m_nDirection = 1;
				m_fFlyStep = 0.0;
			}
			else 
			{
				m_fFlyStep += 100.0f;
			}
		}
		else if (m_nDirection == 1)
		{
			//+z---->-z
			//已经飞到Z的最深处，调转方向====>m_nDirection=2
			if (START_Z_POS +(-m_fFlyStep) <= MAX_Z_POS)
			{
				m_bDrawDir1 = TRUE;
				m_nDirection = 2;
				m_fFlyStep = 0.0f;
			}
			else
			{
				m_fFlyStep += 100.0f;
			}
		}
		else if (m_nDirection == 2)
		{
			//+x---->-x
			if (MAX_X_POS-m_fFlyStep <= START_X_POS)
			{
				m_bDrawDir2 = TRUE;
				m_nDirection = 3;
				m_fFlyStep = 0.0f;
			}
			else
			{
				m_fFlyStep += 100.0f;
			}
		}
		else if (m_nDirection == 3)
		{
			//-z---->+z
			if (MAX_Z_POS+m_fFlyStep >= START_Z_POS)
			{
				m_bDrawDir3 = TRUE;
				m_nDirection = 0;
				m_fFlyStep = 0.0f;
			}
			else
			{
				m_fFlyStep += 100.0f;
			}
		}
	}

	Render();
}
void CplaneloaderView::OnDraw(CDC* /*pDC*/)
{
	CplaneloaderDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	Render();
}

void CplaneloaderView::Render()
{
	glClearColor(0, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	DrawAxes();

	if (m_bFlyWithRect)
	{
		DrawPlaneWithRect();
		DrawFlyRoute();
	}
	else
	{
		DrawPlane();
	}
	
	SwapBuffers(m_hDC);
}

void CplaneloaderView::DrawAxes()
{
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslated(-6.0,-2.5,-8);
	glBegin(GL_LINES);
	glColor3f(1.0,0.0,0.0);
	glVertex3f(0.0f, 0.0,0.0);
	glVertex3f(1.0f, 0.0,0.0);
	glColor3f(0.0,1.0,0.0);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(0.0,1.0,0.0);
	glColor3f(0.0,0.0,1.0);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(0.0,0.0,1.0);
	glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING);

}
void CplaneloaderView::DrawPlane()
{
	glPushMatrix();
	glTranslated (START_X_POS, START_Y_POS, START_Z_POS);
	glScalef(SCALE_FACTOR, SCALE_FACTOR, SCALE_FACTOR);

	if (m_bRatateXEnable)
	{
		glRotated(m_fAngleX, 1.0, 0.0, 0.0);
	}
	if (m_bRatateYEnable)
	{
		glRotated(m_fAngleX, 0.0, 1.0, 0.0);
	}
	if (m_bRatateZEnable)
	{
		glRotated(m_fAngleZ, 0.0, 0.0, 1.0);
	}
	
	//由于原点在飞机的模型的箭头部位，
	//对于绕x，z轴的选择，看起来像分针(代表飞机)绕着表轴转一样。
	//所以下面两句的顺序不同得到的结果也不同！
	glRotated(90.0,0.0,0.0,1.0);//2.绕飞机的箭头原点整个机身逆时针转90度。
	glRotated(90.0,0.0,1.0,0.0);//1.先绕Y转个90度得到侧面。

	//必须启用光照，否则飞机模型纸片化
	glEnable(GL_LIGHTING);
	if (m_bPlaneLoaded)
	{
		m_planeModel.drawGL();
	}
	glDisable(GL_LIGHTING);
	
	glPopMatrix();
}

void CplaneloaderView::DrawPlaneWithRect()
{
	glPushMatrix();

		if (m_nDirection == 0)
		{
			//-x---->+x
			glTranslated (START_X_POS+m_fFlyStep, START_Y_POS, START_Z_POS);
		}
		else if (m_nDirection == 1)
		{
			//+z---->-z
			glTranslated(MAX_X_POS, START_Y_POS, START_Z_POS-m_fFlyStep);
		}
		else if (m_nDirection == 2)
		{
			//+x------>-x
			glTranslated(MAX_X_POS-m_fFlyStep, START_Y_POS, MAX_Z_POS);
		}
		else if (m_nDirection == 3)
		{
			//-z----->+z
			glTranslated(START_X_POS, START_Y_POS, MAX_Z_POS + m_fFlyStep);
		}

		glRotated(m_nDirection*90, 0.0, 1.0, 0.0);//3.根据方向不同，调转机头。
		glRotated(90.0,0.0,0.0,1.0);//2.绕飞机的箭头原点整个机身逆时针转90度。
		glRotated(90.0,0.0,1.0,0.0);//1.先绕Y转个90度得到侧面。
		
		glScalef(SCALE_FACTOR, SCALE_FACTOR, SCALE_FACTOR);
		glEnable(GL_LIGHTING);
		if (m_bPlaneLoaded)
		{
			m_planeModel.drawGL();
		}
		glDisable(GL_LIGHTING);

	glPopMatrix();
	
}

void CplaneloaderView::DrawFlyRoute()
{
	if (m_bDrawDir0)
	{
 		glPushMatrix();
 		glBegin(GL_LINES);
 		glColor3f(1.0, 0.0, 0.0);
 		glVertex3f(START_X_POS, START_Y_POS+MOVE_Y_OFFSET, START_Z_POS);
 		glVertex3f(MAX_X_POS,	START_Y_POS+MOVE_Y_OFFSET, START_Z_POS);
 		glEnd();
 		glPopMatrix();
	}
	if (m_bDrawDir1)
	{
 		glPushMatrix();
 		glBegin(GL_LINES);
 		glColor3f(0.0, 1.0, 0.0);
 		glVertex3f(MAX_X_POS, START_Y_POS+MOVE_Y_OFFSET, START_Z_POS);
 		glVertex3f(MAX_X_POS, START_Y_POS+MOVE_Y_OFFSET, MAX_Z_POS);
 		glEnd();
 		glPopMatrix();

	}
	if (m_bDrawDir2)
	{
		glPushMatrix();
		glBegin(GL_LINES);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(MAX_X_POS,   START_Y_POS+MOVE_Y_OFFSET, MAX_Z_POS);
		glVertex3f(START_X_POS, START_Y_POS+MOVE_Y_OFFSET, MAX_Z_POS);
		glEnd();
		glPopMatrix();
	}
	if (m_bDrawDir3)
	{
		glPushMatrix();
		glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 1.0);
		glVertex3f(START_X_POS, START_Y_POS+MOVE_Y_OFFSET, MAX_Z_POS);
		glVertex3f(START_X_POS, START_Y_POS+MOVE_Y_OFFSET, START_Z_POS);
		glEnd();
		glPopMatrix();
	}
}

void CplaneloaderView::OnSize(UINT nType, int width, int height)
{
	// avoid divide by zero
	if (height==0)  
	{
		height=1;
	}

	// set the viewport to the new dimensions
	glViewport(0, 0, width, height);

	// select the projection matrix and clear it out
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// set the perspective with the appropriate aspect ratio
	gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 20000.0f);

	// select modelview matrix
	glMatrixMode(GL_MODELVIEW);
}


BOOL CplaneloaderView::InitOpenGL()
{
	CDC* pDC = GetDC();
	m_hDC = pDC->GetSafeHdc();

	/*if (!(m_hDC = GetDC(m_hWnd)))
	{
		AfxMessageBox(_T("Unable to create device context"));
		return FALSE;
	}*/

	// set the pixel format we want
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // size of structure
		1,                              // default version
		PFD_DRAW_TO_WINDOW |            // window drawing support
		PFD_SUPPORT_OPENGL |            // OpenGL support
		PFD_DOUBLEBUFFER,               // double buffering support
		PFD_TYPE_RGBA,                  // RGBA color mode
		32,                           // 32 bit color mode
		0, 0, 0, 0, 0, 0,               // ignore color bits, non-palettized mode
		0,                              // no alpha buffer
		0,                              // ignore shift bit
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // ignore accumulation bits
		16,                             // 16 bit z-buffer size
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main drawing plane
		0,                              // reserved
		0, 0, 0 };                      // layer masks ignored

		GLuint  pixelFormat;

		// choose best matching pixel format
		if (!(pixelFormat = ChoosePixelFormat(m_hDC, &pfd)))
		{
			//MessageBox(NULL, "Can't find an appropriate pixel format", "Error", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

		// set pixel format to device context
		if(!SetPixelFormat(m_hDC, pixelFormat,&pfd))
		{
			//MessageBox(NULL, "Unable to set pixel format", "Error", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

		// create the OpenGL rendering context
		if (!(m_hrc = wglCreateContext(m_hDC)))
		{
			//MessageBox(NULL, "Unable to create OpenGL rendering context", "Error",MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

		// now make the rendering context the active one
		if(!wglMakeCurrent(m_hDC, m_hrc))
		{
			//MessageBox(NULL,"Unable to activate OpenGL rendering context", "ERROR", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}
}


// CplaneloaderView diagnostics

#ifdef _DEBUG

CplaneloaderDoc* CplaneloaderView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CplaneloaderDoc)));
	return (CplaneloaderDoc*)m_pDocument;
}
#endif //_DEBUG



void CplaneloaderView::OnRotateX()
{
	m_bRatateXEnable = !m_bRatateXEnable;
}

void CplaneloaderView::OnRotateY()
{
	m_bRatateYEnable = !m_bRatateYEnable;
}

void CplaneloaderView::OnRotateZ()
{
	m_bRatateZEnable = !m_bRatateZEnable;
}

void CplaneloaderView::OnUpdateRotateX(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bRatateXEnable);
}

void CplaneloaderView::OnUpdateRotateY(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bRatateYEnable);
}

void CplaneloaderView::OnUpdateRotateZ(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bRatateZEnable);
}

void CplaneloaderView::OnRotateFlyinrect()
{
	m_bFlyWithRect = !m_bFlyWithRect;
	if (m_bFlyWithRect)
	{
		SetTimer(TIMER_FLY_IN_RECT, 200,NULL);
	}
}
	
void CplaneloaderView::OnUpdateRotateFlyinrect(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bFlyWithRect);
}
