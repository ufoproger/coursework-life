#include "stdafx.h"
#include "MemoryDC.h"

CMemoryDC::CMemoryDC(CDC *pDC, const CRect *pRect) : 	// Конструктор
	CDC()	// Вызываем конструктор родительского класса
{
	m_pDC = pDC;	// Копируем указатель на контекст рисования
	m_oldBitmap = NULL;	// Старое изображение области рисования
	m_bMemDC = !pDC->IsPrinting();	// Был ли контекст использован для рисования

	if (pRect == NULL)	// Если область рисования не указана
		pDC->GetClipBox(&m_rect);	// То будем рисовать везде
	else
		m_rect = *pRect;	// Иначе только там, где надо

	if (m_bMemDC)	// Если уже рисовали на экране
	{
		CreateCompatibleDC(pDC);	// Создаем совместимый контекст рисования
		pDC->LPtoDP(&m_rect);	// Получаем размеры
		m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());	// Создаем копию экрана
		m_oldBitmap = SelectObject(&m_bitmap);	// Ставим старую копию и запоминаем то, что было
		SetMapMode(pDC->GetMapMode());	// Настройки рисования переносим
		SetWindowExt(pDC->GetWindowExt());	// Переносим настройки рисования
		SetViewportExt(pDC->GetViewportExt());	// Настройки переносим рисования
		pDC->DPtoLP(&m_rect);	// Получаем размер
		SetWindowOrg(m_rect.left, m_rect.top);	// Сдвиг окна
	}
	else	// Если на экране не рисовали
	{
		m_bPrinting = pDC->m_bPrinting;	// Рисовали ли раньше?
		m_hDC = pDC->m_hDC;	// Копируем контекст рисоавния
		m_hAttribDC = pDC->m_hAttribDC;	// Копируем аттрибуты рисования
	}

	FillSolidRect(m_rect, pDC->GetBkColor());	// Заливаем всё фоновым цветом
}

CMemoryDC::~CMemoryDC()	// Деструктор
{          
	if (m_bMemDC)	// Если рисовали до этого что-то
	{
		m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), this, m_rect.left, m_rect.top, SRCCOPY);	// Выводим в контекст рисования то, что нарисовали в памяти
		SelectObject(m_oldBitmap);	// Отдаём обратно старое изображение
	}
	else	// Если не рисовали до этого что-нибудь
		m_hDC = m_hAttribDC = NULL;	// Нечего выводить
}

CMemoryDC* CMemoryDC::operator->() 	// Вызов методов
{
	return this;
}       

CMemoryDC::operator CMemoryDC*()	// Оператор разыменование указателя
{
	return this;
}