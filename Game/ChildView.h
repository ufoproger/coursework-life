#pragma once

#include "Protoblast.h"

/* Класс дочернего вида, наследуемый от класса окна */
class CChildView : public CWnd
{
public:
	CChildView();	// Конструктор по-умолчанию
	~CChildView();	// Деструктор

private:
	BOOL PreCreateWindow(CREATESTRUCT &);	// Метод подготовки к созданию окна

	afx_msg void OnLButtonDblClk(UINT, CPoint);	// Метод перехвата двойного клика левой кнопки мыши
	afx_msg void OnNcMouseMove(UINT, CPoint);	// Метод перехвата передвижения курсора вне области вида
	afx_msg void OnLButtonDown(UINT, CPoint);	// Метод перехвата нажатия левой кнопки мыши
	afx_msg void OnRButtonUp(UINT, CPoint);		// Метод перехвата отпускания правой кнопки мыши
	afx_msg void OnMouseMove(UINT, CPoint);		// Метод перехвата передвижения курсора в области вида
	afx_msg void OnKeyDown(UINT, UINT, UINT);	// Метод нажатия на клавишу
	afx_msg void OnPaint();						// Метод рисования в виде
	afx_msg BOOL OnEraseBkgnd(CDC *);			// Метод определения необходимости очистки фона

	DECLARE_MESSAGE_MAP()	// Макрос конца класса для объявления перехватываемых сообщений
};

