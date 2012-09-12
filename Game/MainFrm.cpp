#include "stdafx.h"
#include "Game.h"
#include "MainFrm.h"

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
END_MESSAGE_MAP()

static UINT indicators[] ={ID_SEPARATOR};

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
		return -1;

	if (!m_wndStatusBar.Create(this))
		return -1;

	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));
	SetTimer(1, 1000 / theApp.GetFPS(), NULL);

	return 0;
}

void CMainFrame::OnTimer(UINT nIDEvent)
{
	theApp.Tick();
	m_wndView.RedrawWindow();

	CString text;

	text.Format(_T("Количество живых клеток на поле: %d"), theApp.GetProtoblastCount());
	m_wndStatusBar.SetPaneText(0, text);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

void CMainFrame::OnSetFocus(CWnd *pOldWnd)
{
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void *pExtra, AFX_CMDHANDLERINFO *pHandlerInfo)
{
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}