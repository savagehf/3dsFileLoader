
#pragma once

#include "trilist.h"

class CplaneloaderView : public CView
{
protected: 
	CplaneloaderView();
	virtual ~CplaneloaderView();
	DECLARE_DYNCREATE(CplaneloaderView)
	CplaneloaderDoc* GetDocument() const;

public:
	virtual void OnDraw(CDC* pDC); 
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	BOOL InitOpenGL();
	BOOL LoadPlaneModel();
	void InitLight();
	void Render();
	void DrawAxes();
	void DrawPlane();
	void DrawPlaneWithRect();
	void DrawFlyRoute();

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int  OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRotateX();
	afx_msg void OnRotateY();
	afx_msg void OnRotateZ();
	afx_msg void OnUpdateRotateX(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRotateY(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRotateZ(CCmdUI *pCmdUI);
	afx_msg void OnRotateFlyinrect();
	afx_msg void OnUpdateRotateFlyinrect(CCmdUI *pCmdUI);
	DECLARE_MESSAGE_MAP()

protected:
	HDC	  m_hDC;
	HGLRC m_hrc;
	CTriList m_planeModel;
	BOOL m_bPlaneLoaded;
	float m_fAngleX;
	float m_fAngleY;
	float m_fAngleZ;
	BOOL  m_bRatateXEnable;
	BOOL  m_bRatateYEnable;
	BOOL  m_bRatateZEnable;

	//绕矩形路线飞行模拟
	BOOL  m_bFlyWithRect;
	float m_fFlyStep;
	int   m_nDirection;
	BOOL  m_bDrawDir0;
	BOOL  m_bDrawDir1;
	BOOL  m_bDrawDir2;
	BOOL  m_bDrawDir3;

};

#ifndef _DEBUG  // debug version in planeloaderView.cpp
inline CplaneloaderDoc* CplaneloaderView::GetDocument() const
   { return reinterpret_cast<CplaneloaderDoc*>(m_pDocument); }
#endif

