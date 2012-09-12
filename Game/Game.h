#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "stdafx.h"
#include "resource.h"
#include "Protoblast.h"
#include "Clock.h"
#include <vector>

/* Класс игровой логики, наследованный от класса приложения виндовса */
class CGameApp : public CWinApp
{
public:
	CGameApp();	// Конструктор
	~CGameApp();	// Деструктор

	BOOL InitInstance();	// Метод инициализации окна
	int ExitInstance();	// Метод выхода

	CSize GetFieldSize();	// Метод получения размера игрового поля
	void LeftButtonDoubleClick(UINT, CPoint);	// Метод от двойного клика левой кнопки мыши
	void LeftButtonDown(UINT, CPoint);	// Метод от нажатия левой кнопки мыши
	void RightButtonUp(UINT, CPoint);	// Метод от отпускания правой кнопки мыши
	void CreateNewGame();	// Метод создания новой игры
	void RenderField(CMemoryDC &, CRect);	// Метод рисования игрового поля
	void NcMouseMove();	// Метод от курсора, вышеднего за пределы игрового поля
	void MouseMove(UINT, CPoint);	// Метод от курсора, бегающего по игровому полю
	void KeyDown(UINT, UINT, UINT);	// Метод от нажатия клавиши
	void Tick();	// Метод тика
	int GetProtoblastCount();	// Метод получения количества живых клеток
	int GetFPS();	// Метод получения частоты кадров в секунду

	afx_msg void OnAppAbout();	// Метод нажатия на пункт "О программе" в меню сверху
	afx_msg void OnOpenGame();	// Метод нажатия на пункт "Открыть игру" в меню сверху
	afx_msg void OnSaveGame();	// Метод нажатия на пункт "Сохранить игру как" в меню сверху
	afx_msg void OnNewGame();	// Метод нажатия на пункт "Новая игра" в меню сверху
	afx_msg void OnEvolve(UINT);	// Метод нажатия на один из пунктов установки скорости эволюции, где-то в меню сверху

	DECLARE_MESSAGE_MAP() // Макрос конца класса для объявления перехватываемых сообщений

private:
	static const int m_fieldHeight = 69;	// Высота игрового поля
	static const int m_fieldWidth = 56;	// Ширина игрового поля
	static const int m_fps = 25;	// Частота кадров в секунду

	std::vector < std::vector < CProtoblast * > > m_field;	// Двумерный массив клеток на поле
	CPoint m_oldMousePosition;	// Старая позиция указателя мыши
	CPoint m_hoverCell;	// Выделенная ячейка
	CPoint m_offset;	// Сдвиг поля для навигации
	CClock m_clock;	// Индикатор режима эволюций
	int m_evolveTime;	// Время интервала между эволюциями
	int m_tickCount;	// Время с момента последней эволюции
	bool m_pause;	// А не на паузе ли игра?

	bool MousePositionToXY(CPoint, CPoint &);	// Метод реобразования координат курсора в координаты клетки на поле
	void FreeField();	// Метод даления игрового поля
	void Evolve();	// Метод эволюции-революции
};

extern CGameApp theApp; 	// Объявляем объект класса везде!
