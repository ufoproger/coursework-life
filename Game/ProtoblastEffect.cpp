#include "stdafx.h"
#include "ProtoblastEffect.h"
#include <algorithm>

CProtoblastEffect::CProtoblastEffect(CSize area) : 	// Конструктор
	m_area(area)	// Вызываем родительский конструктор
{
}

CProtoblastEffect::~CProtoblastEffect()	// Деструктор
{
}

COLORREF CProtoblastEffect::GetCurrColor()	// Метод получения текущего цвета
{
	return m_currColor;	// Текущий цвет
}

void CProtoblastEffect::ChangeColor(COLORREF endColor, int stepR, int stepG, int stepB)	// Метод изменения текущего цвета в сторону заданного цвета пошагово для каждой компоненты
{
	if (m_currColor == endColor)	// Если достигли цели
		return;	// То делать нечего

	int r = MakeChangeColorStep(GetRValue(m_currColor), GetRValue(endColor), stepR);	// Иначе для каждой компоненты приближаем цель
	int g = MakeChangeColorStep(GetGValue(m_currColor), GetGValue(endColor), stepG);	// Приближаем цель для каждой компоненты
	int b = MakeChangeColorStep(GetBValue(m_currColor), GetBValue(endColor), stepB);	// Приближаем...

	m_currColor = RGB(r, g, b);	// Получаем текущий цвет
}

byte CProtoblastEffect::MakeChangeColorStep(int chCurr, int chEnd, int step)	// Метод изменения цветовой компоненты до заданной с заданным шагом
{
	if (step < 0)	// Если шаг отрицательный
		step *= -1;	// То делаем его положительным

	if (chCurr == chEnd)	// Если компоненты похожи
		return chCurr;	// То эффект достигнут

	if (chCurr > chEnd)	// Если надо назад уменьшить компоненту
		chCurr = max(chCurr - step, chEnd);	// То так и делаем
	else
		chCurr = min(chCurr + step, chEnd);	// Иначе надо немного вперед

	return chCurr;	// Что получилось, то и возвращаем
}

void CProtoblastEffect::Render(CMemoryDC &dc, CPoint position)	// Метод отрисовки
{
	dc.FillSolidRect(position.x, position.y, m_area.cx, m_area.cy, m_currColor);	// Заливаем внутреннюю область текущим цветом
}

EStatus CProtoblastEffect::GetLastStatus()	// Получаем последний статус
{
	return STATUS_NONE;	// Последнее здесь не нужно
}

CProtoblastEffectDisable::CProtoblastEffectDisable(CSize area, CProtoblastEffect *last) :	// Конструктор
	CProtoblastEffect(area)	// Конструктор родительского класса
{
	m_currColor = last ? last->GetCurrColor() : m_endColor;	// Если у прошлого эффекта есть текущий цвет, то он и для нас тоже текущий
}

CProtoblastEffectDisable::~CProtoblastEffectDisable()	// Деструктор
{
}

void CProtoblastEffectDisable::Tick()	// Метод тика
{
	ChangeColor(m_endColor, 20, 20, 20);	// Приближаем текущий цвет к конечнему 20-мильными шагами
}

EStatus CProtoblastEffectDisable::GetStatus()	// Метод получения статуса
{
	return STATUS_DISABLE;	// Статус мертвой клетки
}

CProtoblastEffectActive::CProtoblastEffectActive(CSize area, CProtoblastEffect *last) :	// Конструктор
	CProtoblastEffect(area)	// Конструктор родительского класса
{
	m_currColor = last ? last->GetCurrColor() : m_endColor;	// Если была прошлая клетка, то забираем её текущий цвет себе
}

CProtoblastEffectActive::~CProtoblastEffectActive()	// Деструктор
{
}

EStatus CProtoblastEffectActive::GetStatus()	// Метод полученрия статуса
{
	return STATUS_ACTIVE;	// Статус живой клетки
}

void CProtoblastEffectActive::Tick()	// Метод тика
{
	ChangeColor(m_endColor, 20, 20, 20);	// Приближаем текущий цвет к цели
}

CProtoblastEffectHover::CProtoblastEffectHover(CSize area, CProtoblastEffect *last) :	// Конструктор
	CProtoblastEffect(area)	// Конструктор родительского класса
{
	m_lastStatus = last->GetStatus();	// Получаем статус прошлого эффекта
	m_color = 0;	// Индекс текущего цвета
	m_colors[0] = RGB(255, 255, 153);	// Ярко-желтый
	m_colors[1] = RGB(255, 255, 102);	// Желтый
	m_colors[2] = RGB(255, 255, 51);	// Не ярко-желтый
	m_colors[3] = m_colors[1];	// Желтый
	m_currColor = last->GetCurrColor();	// Получаем текущий цвет, чтобы потом его вернуть следующему эффекту
}

CProtoblastEffectHover::~CProtoblastEffectHover()	// Деструктор
{
}

void CProtoblastEffectHover::Tick()	// Метод тика
{
	++m_tickCount;	// Тикаем

	if (m_tickCount % 10 == 0)	// Если пора менять цвет
	{
		m_tickCount = 0;	// То тикаем заново
		m_color = (m_color + 1) % 4; 	// Меняем цвет на следующий
	}
}

void CProtoblastEffectHover::Render(CMemoryDC &dc, CPoint position)	// Метод рисования
{
	dc.FillSolidRect(position.x, position.y, m_area.cx, m_area.cy, m_colors[m_color]);	// Рисуем внутреннюю область цвета, на который указывает индекс цвета
}

EStatus CProtoblastEffectHover::GetStatus()	// Метод получения статуса
{
	return STATUS_HOVER;	// Статус выделенной клетки
}

EStatus CProtoblastEffectHover::GetLastStatus()	// Метод получения статуса прошлого эффекта
{
	return m_lastStatus;	// Прошлый статус
}