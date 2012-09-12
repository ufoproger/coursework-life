#pragma once

#define _USE_MATH_DEFINES

#include "stdafx.h"
#include "MemoryDC.h"

class CClock
{
public:
	CClock();
	~CClock();

	void SetTime(int);
	void Render(CMemoryDC &, CPoint);
	void Tick();
	int GetHeight();
	int GetWidth();

protected:
	static const int m_height = 49;
	static const int m_width = 49;

	int m_tick;
	int m_time;
};