#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "stdafx.h"
#include "resource.h"
#include "Protoblast.h"
#include "Clock.h"
#include <vector>

class CGameApp : public CWinApp
{
public:
	CGameApp();
	~CGameApp();

	virtual BOOL InitInstance();
	virtual int ExitInstance();

	CSize GetFieldSize();
	void LeftButtonDoubleClick(UINT, CPoint);
	void LeftButtonDown(UINT, CPoint);
	void RightButtonUp(UINT, CPoint);
	void CreateNewGame();
	void RenderField(CMemoryDC &, CRect);
	void NcMouseMove();
	void MouseMove(UINT, CPoint);
	void KeyDown(UINT, UINT, UINT);
	void Tick();
	int GetProtoblastCount();
	int GetFPS();

	afx_msg void OnAppAbout();
	afx_msg void OnOpenGame();
	afx_msg void OnSaveGame();
	afx_msg void OnNewGame();
	afx_msg void OnEvolve(UINT);

	DECLARE_MESSAGE_MAP()

protected:
	static const int m_fieldHeight = 69;
	static const int m_fieldWidth = 56;
	static const int m_fps = 25;

	std::vector < std::vector < CProtoblast * > > m_field;
	CPoint m_oldMousePosition;
	CPoint m_hoverCell;
	CPoint m_offset;
	CClock m_clock;
	int m_evolveTime;
	int m_tickCount;
	bool m_pause;

	bool MousePositionToXY(CPoint, CPoint &);
	void FreeField();
	void Evolve();
};

extern CGameApp theApp;
