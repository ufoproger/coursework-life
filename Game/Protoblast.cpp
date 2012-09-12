#include "stdafx.h"
#include "Protoblast.h"
#include "ProtoblastEffect.h"

CProtoblast::CProtoblast()	// Конструктор по-умолчанию
{
	m_frame = CSize(24, 24);	// Размеры клетки
	m_area = m_frame - CSize(2, 2);	// Размеры внутренней области клетки
	m_effect = NULL;	// Текущий вид

	SetStatus(STATUS_DISABLE);	// Клетка неактивна
}

CProtoblast::~CProtoblast()	// Деструктор
{
	if (m_effect)	// Если клетку рисовали
		delete m_effect;	// То больше рисовать её не стоит
}

void CProtoblast::Render(CMemoryDC &dc, CPoint position)	// Метод отрисовки клетки
{
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));	// Карандаш рисования
	CPen *pLastPen = dc.SelectObject(&pen);	// Устанавливаем карандаш

	dc.Rectangle(position.x, position.y, position.x + m_frame.cx, position.y + m_frame.cy);	// Рисуем прямоугольник
	dc.SelectObject(pLastPen);	// Возвращаем прошлый караншад
	m_effect->Render(dc, position + CPoint(1, 1));	// Отрисовываем внутренню область
}

void CProtoblast::Tick()	// Метод обновления состояния
{
	m_effect->Tick();	// Передаём управление объекту рисования
}

void CProtoblast::Click(bool isLeftButton)	// Метод клика по клетке кнопкой мыши
{
	if (isLeftButton)	// Если это была левая кнопка мыши
		SetStatus(STATUS_ACTIVE);	// То клетка оживает
	else	// Иначе
		SetStatus(STATUS_DISABLE);	// Умираем :(
}

void CProtoblast::Hover()	// Метод выедления клетки
{
	SetStatus(STATUS_HOVER);	// Выделяем клетку
}


void CProtoblast::Unhover()	// Метод унвыделения клетки
{
	SetStatus(m_effect->GetLastStatus());	// Клетка принимает предпоследнее состояние
}

void CProtoblast::SetStatus(EStatus status)	// Метод установки состояния клетки
{
	if (status == STATUS_NONE)	// Если нечего устанавливать
		return;	// То ничего делать не надо

	EStatus currStatus = m_effect ? m_effect->GetStatus() : STATUS_NONE;	// Получаем текущее состояние клетки

	if (status == currStatus)	// если текущее и новое состояния совпадают
		return;	// Больше ничего делать не надо

	if (status == STATUS_HOVER && currStatus != STATUS_DISABLE)	// Если хотим выделить клетку и она не является неживой
		return;	// То так дело не пойдет

	CProtoblastEffect *temp = m_effect;	// Текущий эффект рисования

	switch (status)	// В зависимости от состояния
	{
	case STATUS_ACTIVE:	// Оживляем клетку
		m_effect = new CProtoblastEffectActive(m_area, temp);	// Эффект живой клетки
		break;

	case STATUS_HOVER:	// Выделяем
		m_effect = new CProtoblastEffectHover(m_area, temp);	// Выделяем
		break;

	default:	// Иначе наверное она неживая
		m_effect = new CProtoblastEffectDisable(m_area, temp);	// Мертвая клетка
	}

	delete temp;	// Удаляем старый эффект рисования
}

int CProtoblast::GetWidth()	// Получаем ширину клетки
{
	return m_frame.cx;	// Ширина
}

int CProtoblast::GetHeight()	// Получаем высоту клетки
{
	return m_frame.cy;	// Высота
}

CSize CProtoblast::GetSize()	// Получаем размер клетки
{
	return m_frame;	// Размер!
}

bool CProtoblast::IsLive()	// Является ли клетка живой?
{
	return (m_effect->GetStatus() == STATUS_ACTIVE);	// Если живая, то клетка живая 146%
}