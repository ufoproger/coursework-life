#include "stdafx.h"
#include "Clock.h"
#include "MemoryDC.h"
#include "resource.h"
#include <cmath>

CClock::CClock()	// Конструктор по-умолчанию
{
	m_time = 0;	// Ручной режим вызова эволюции
}

CClock::~CClock()	// Деструктор
{
}

void CClock::Render(CMemoryDC &dc, CPoint position)	// Метод вывода на экран индикатора
{
	CBitmap bmp;	// Изображение индикатора
	CDC dcMemory;	// Контекст рисования в памяти

	bmp.LoadBitmap(m_time ? IDB_CLOCK : IDB_HAND);	// В зависимости от режима вызова эволюций загружается либо циферблат, либо пингвин
	dcMemory.CreateCompatibleDC(dc);	// Создание совместимого контекста рисования с имеющимся
	dcMemory.SelectObject(&bmp);	// Установка изображения-индикатора
	dc->TransparentBlt(position.x, position.y, m_width, m_height, &dcMemory, 0, 0, m_width, m_height, RGB(0, 0, 0)); // Вывод в общий контекст рисования изображения с учётом прозрачного цвета

	if (m_time)	// Если режим вызова эволюций автоматический
	{
		CPen pen(PS_SOLID, 3, RGB(255, 255, 255));	// Карандаш для рисования стрелки часов

		dc->SelectObject(&pen);	// Выбираем карандаш
		dc->MoveTo(position + CPoint(22, 24));	// Переходим в центр циферблата

		double x = (double)position.x + 22.0 + 6.0 * sin((double)m_tick * M_PI / (double)m_time * 2.0);	// Получаем положение конца стрелки по оси абсцисс
		double y = (double)position.y + 24.0 - 6.0 * cos((double)m_tick * M_PI / (double)m_time * 2.0);	// Получаем положение конца стрелки по оси ординат

		dc->LineTo((int)x, (int)y);	// Рисуем стрелку из центра циферблата
	}
}

void CClock::Tick()	// Метод обновления состояния
{
	++m_tick;	// Увеличиваем счетчик количества тиков с последней эволюции
}

void CClock::SetTime(int time)	// Метод установки интервала между вызовами эволюции
{
	m_time = time;	// Запоминаем интервал
	m_tick = 0;		// Эволюции пока быть не должно
}

int CClock::GetHeight()	// Метод получения высоты изображения-индикатора
{
	return m_height;	// Возвращаем высоту
}

int CClock::GetWidth()	// Метод получения ширины изображения-индикатора
{
	return m_width;		// Возвращаем ширину
}