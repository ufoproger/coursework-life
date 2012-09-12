#pragma once

#define _USE_MATH_DEFINES

#include "stdafx.h"
#include "MemoryDC.h"

/* Класс индикации режима вызова эволюции клеток */
class CClock
{
public:
	CClock();	// Конструктор по-умолчанию
	~CClock();	// Деструктор

	void SetTime(int);	// Метод установки интервала между вызовами эволюции клеток
	void Render(CMemoryDC &, CPoint);	// Метод вывода на экран индикатора
	void Tick();	// Метод обновления состояния
	int GetHeight();	// Метод получения высоты изображения-индикатора
	int GetWidth();		// Метод получения ширины изображения-индикатора

protected:
	static const int m_height = 49;	// Высота изображения-индикатора
	static const int m_width = 49;	// Ширина изображения-индикатора

	int m_tick;	// Счётчик количества тиков до с момента последней эволюции
	int m_time; // Интервал между вызовами эволюции клеток
};