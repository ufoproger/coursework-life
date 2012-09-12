#pragma once

#include "ChildView.h"

/* Класс основого фрейма, наследованный от класса основного окна */
class CMainFrame : public CFrameWnd
{
public:
	CMainFrame();	// Конструктор по-умолчанию
	~CMainFrame();	// Деструктор

	BOOL PreCreateWindow(CREATESTRUCT &);	// Метод подготовки к созданию окна
	BOOL OnCmdMsg(UINT, int, void *, AFX_CMDHANDLERINFO *);	// Метод получения сообщений

private:
	CStatusBar m_wndStatusBar;	// Статус-бар
	CChildView m_wndView;		// Вид

	afx_msg void OnTimer(UINT);				// Метод перехвата тика таймера
	afx_msg int OnCreate(LPCREATESTRUCT);	// Метод перехвата сообщения создания окна
	afx_msg void OnSetFocus(CWnd *);		// Метод перехвата получения фокуса окна

	DECLARE_MESSAGE_MAP()			// Макрос конца класса для объявления перехватываемых сообщений
	DECLARE_DYNAMIC(CMainFrame)		// Макрос возможности получения информации о классе во время выполнения программы
};