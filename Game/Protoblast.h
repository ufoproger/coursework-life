#pragma once

#include "stdafx.h"
#include "ProtoblastEffect.h"

/* Класс клетки игрового поля */
class CProtoblast
{
public:
	CProtoblast();	// Конструктор по-умолчанию
	~CProtoblast();	// Деструктор

	CSize GetSize();	// Метод получения размера клетки
	void Render(CMemoryDC &, CPoint);	// Метод отрисовки клетки
	void Tick();	// Метод обновления состояния
	void Click(bool isLeftButton = true);	// Метод клика по клетке кнопкой мыши
	void Hover();	// Метод выделения клетки
	void Unhover();	// Метод унвыделения клетки
	bool IsLive();	// Метод определения является ли клетка живой?
	int GetWidth();	// Метод получения ширины клетки
	int GetHeight();	// Метод получения высоты клетки


private:
	CProtoblastEffect *m_effect;	// Рисование клетки
	CSize m_frame;	// Размер клетки
	CSize m_area;	// Размер внутренней области
	
	void SetStatus(EStatus);	// Метод установки состояния клетки
};