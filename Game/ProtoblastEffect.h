#pragma once

#include "stdafx.h"
#include "MemoryDC.h"

enum EStatus {STATUS_NONE, STATUS_DISABLE, STATUS_ACTIVE, STATUS_HOVER};	// Перечисление состояний клетки

/* Абстрактный класс эффекта рисования клетки */
class CProtoblastEffect
{
public:
	CProtoblastEffect(CSize area);	// Конструктор
	virtual ~CProtoblastEffect();	// Виртуальный деструктор

	COLORREF GetCurrColor();	// Метод получения текущего цвета клетки

	virtual void Tick() = 0;	// Метод тика
	virtual void Render(CMemoryDC &, CPoint);	// Метод отрисовки
	virtual EStatus GetStatus() = 0;	// Метод получения статуса
	virtual EStatus GetLastStatus();	// Метод получения предыдущего статуса

protected:
	COLORREF m_currColor;	// Текущий цвет
	CSize m_area;			// Размер

	void ChangeColor(COLORREF, int, int, int);	// Метод изменения текущего цвета в сторону заданного цвета пошагово для каждой компоненты
	byte MakeChangeColorStep (int, int, int);	// Метод изменения цветовой компоненты до заданной с заданным шагом
};

/* Класс эффекта мертвой клетки, наследованный от класса эффекта рисования клетки */
class CProtoblastEffectDisable : public CProtoblastEffect
{
public:
	CProtoblastEffectDisable(CSize, CProtoblastEffect *);	// Конструктор
	~CProtoblastEffectDisable();	// Деструктор

	void Tick();			// Метод тика
	EStatus GetStatus();	// Метод получения статуса

protected:
	static const COLORREF m_endColor = RGB(255, 255, 255);	// Конечный цвет (белый)
};

/* Класс эффекта живой клетки, наследованный от класса эффекта рисования клетки */
class CProtoblastEffectActive : public CProtoblastEffect
{
public:
	CProtoblastEffectActive(CSize, CProtoblastEffect *);	// Конструктор
	~CProtoblastEffectActive();	// Деструктор

	void Tick();			// Метод тика
	EStatus GetStatus();	// Метод получения статуса

protected:
	static const COLORREF m_endColor = RGB(87, 141, 249);	// Конечный цвет (голубовастый ближе к синему)
};

/* Класс эффекта выделенной клетки, наследованный от класса эффекта рисования клетки */
class CProtoblastEffectHover : public CProtoblastEffect
{
public:
	CProtoblastEffectHover(CSize, CProtoblastEffect *last);	// Конструктор
	~CProtoblastEffectHover();	// Деструктор
	
	void Tick();	// Метод тика
	void Render(CMemoryDC &, CPoint);	// Метод рисования
	EStatus GetStatus();		// Метод получения статус
	EStatus GetLastStatus();	// Метод получения последнего статуса

protected:
	COLORREF m_colors[4];	// Промежуточные цвета
	int m_color;			// Индекс текущего выбранного промежуточного цвета
	int m_tickCount;		// Счетчик с начала анимации
	EStatus m_lastStatus;	// Предпоследний статус
};