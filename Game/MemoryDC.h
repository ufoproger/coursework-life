#pragma once

/* Класс для реализация двойной буферизации, наследуется от класса от контекста рисования */
class CMemoryDC : public CDC 
{
public:
	CMemoryDC(CDC *, const CRect *pRect = NULL);	// Конструктор
	~CMemoryDC();	// Конструктор по-умолчанию
	CMemoryDC* operator->();	// Оператор получение доступа к методам класса
	operator CMemoryDC*();	// Оператор разыменования

private:       
	CBitmap m_bitmap;	// Текущий экран
	CBitmap *m_oldBitmap;	// Старый экран
	CDC *m_pDC;	// Контекст рисования первоначальный
	CRect m_rect;	// Область рисования
	BOOL m_bMemDC;	// Рисуем ли сейчас в памяти?
};