#include "stdafx.h"
#include "Game.h"
#include "ChildView.h"

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_NCMOUSEMOVE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

void CChildView::OnNcMouseMove(UINT hitTest, CPoint point)
{
	theApp.NcMouseMove();
}

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;
	
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return TRUE;
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	theApp.KeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	theApp.LeftButtonDoubleClick(nFlags, point);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	theApp.MouseMove(nFlags, point);
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	theApp.LeftButtonDown(nFlags, point);
}

void CChildView::OnRButtonUp(UINT nFlags, CPoint point)
{
	theApp.RightButtonUp(nFlags, point);
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this);
	CMemoryDC pDC(&dc);
	CRect rect;

	GetClientRect(&rect);
	theApp.RenderField(pDC, rect);
}
