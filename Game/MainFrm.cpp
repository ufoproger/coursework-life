#include "stdafx.h"
#include "Game.h"
#include "MainFrm.h"

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)	// Макрос возможности получения информации о классе во время выполнения программы

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)	// Макрос начала объявления перехватываемых сообщения
	ON_WM_CREATE()	// Перехват сообщения создания окна
	ON_WM_SETFOCUS()	// Перехват сообщения получения фокуса окна
	ON_WM_TIMER()	// Перехват сообщения тика таймера
END_MESSAGE_MAP()	// Макрос конца объявления перехвата сообщений

static UINT indicators[] ={ID_SEPARATOR};	// Массив элементов статус-бара

CMainFrame::CMainFrame()	// Конструктор по-умолчанию
{
}

CMainFrame::~CMainFrame()	// Деструктор
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)	// Метод перехвата сообщения создания окна
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)	// Если не удалось создать окно
		return -1;	// Всё плохо

	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))	// Если не получилось создать вид
		return -1;	// Всё плохо

	if (!m_wndStatusBar.Create(this))	// Если не удалось создать статус-бар
		return -1;	// Всё плохо

	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));	// Установка индикаторов статус-бара
	SetTimer(1, 1000 / theApp.GetFPS(), NULL);	// Установка таймера

	return 0;	// Всё хорошо
}

void CMainFrame::OnTimer(UINT nIDEvent)	// Метод перехвата сообщения тика таймера
{
	theApp.Tick();	// Передаём управление объекту игровой логики
	m_wndView.RedrawWindow();	// Запрос на перерисовку вида

	CString text;	// Текст в статус-бар

	text.Format(_T("Количество живых клеток на поле: %d"), theApp.GetProtoblastCount());	// Форматируем информацию
	m_wndStatusBar.SetPaneText(0, text);	// Добавляем в статус-бар
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT &cs)	// Метод подготовки к созданию окна
{
	if(!CFrameWnd::PreCreateWindow(cs))	// Если родительскому классу подготовиться к созданию окна
		return FALSE;	// Выходим

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;	// Впуклая рамка окна
	cs.lpszClass = AfxRegisterWndClass(0);	// Класс

	return TRUE;	// Всё хорошо, можно создавать окно
}

void CMainFrame::OnSetFocus(CWnd *pOldWnd)	// Метод перехвата получения фокуса окна
{
	m_wndView.SetFocus();	// Передаём управление виду
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void *pExtra, AFX_CMDHANDLERINFO *pHandlerInfo)	// Метод получения сообщений
{
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))	// Если вид забрал сообщение
		return TRUE;	// То хватит их пересылать

	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);	// Иначе пусть родительский класс с сообщениями разбирается
}