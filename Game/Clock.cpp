#include "stdafx.h"
#include "Clock.h"
#include "MemoryDC.h"
#include "resource.h"
#include <cmath>

CClock::CClock()
{
	m_time = 0;
}

CClock::~CClock()
{
}

void CClock::Render(CMemoryDC &dc, CPoint position)
{
	CBitmap bmp;
	CDC dcMemory;

	bmp.LoadBitmap(m_time ? IDB_CLOCK : IDB_HAND);
	dcMemory.CreateCompatibleDC(dc);
	dcMemory.SelectObject(&bmp);
	dc->TransparentBlt(position.x, position.y, m_width, m_height, &dcMemory, 0, 0, m_width, m_height, RGB(0, 0, 0));

	if (m_time)
	{
		CPen pen(PS_SOLID, 3, RGB(255, 255, 255));

		dc->SelectObject(&pen);
		dc->MoveTo(position + CPoint(22, 24));

		double x = (double)position.x + 22.0 + 6.0 * sin((double)m_tick * M_PI / (double)m_time * 2.0);
		double y = (double)position.y + 24.0 - 6.0 * cos((double)m_tick * M_PI / (double)m_time * 2.0);

		dc->LineTo((int)x, (int)y);
	}
}

void CClock::Tick()
{
	++m_tick;
}

void CClock::SetTime(int time)
{
	m_time = time;
	m_tick = 0;
}

int CClock::GetHeight()
{
	return m_height;
}

int CClock::GetWidth()
{
	return m_width;
}