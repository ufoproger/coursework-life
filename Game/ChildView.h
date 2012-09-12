#pragma once

#include "Protoblast.h"

class CChildView : public CWnd
{
public:
	CChildView();
	virtual ~CChildView();

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg void OnLButtonDblClk(UINT, CPoint);
	afx_msg void OnNcMouseMove( UINT nHitTest, CPoint point);
	afx_msg void OnLButtonDown(UINT, CPoint);
	afx_msg void OnRButtonUp(UINT, CPoint);
	afx_msg void OnMouseMove(UINT, CPoint);
	afx_msg void OnKeyDown(UINT, UINT, UINT);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC *);

	DECLARE_MESSAGE_MAP()
};

