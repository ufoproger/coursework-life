#include "stdafx.h"
#include "ProtoblastEffect.h"
#include <algorithm>

CProtoblastEffect::CProtoblastEffect(CSize area) : 
	m_area(area)
{
}

CProtoblastEffect::~CProtoblastEffect()
{
}

void CProtoblastEffect::ChangeColor(COLORREF endColor, int stepR, int stepG, int stepB)
{
	if (m_currColor == endColor)
		return;

	int r = MakeChangeColorStep(GetRValue(m_currColor), GetRValue(endColor), stepR);
	int g = MakeChangeColorStep(GetGValue(m_currColor), GetGValue(endColor), stepG);
	int b = MakeChangeColorStep(GetBValue(m_currColor), GetBValue(endColor), stepB);

	m_currColor = RGB(r, g, b);
}

byte CProtoblastEffect::MakeChangeColorStep(int chCurr, int chEnd, int step)
{
	if (step < 0)
		step *= -1;

	if (chCurr == chEnd)
		return chCurr;

	if (chCurr > chEnd)
		chCurr = max(chCurr - step, chEnd);
	else
		chCurr = min(chCurr + step, chEnd);

	if (chCurr < 0)
		chCurr = 0;

	if (chCurr > 255)
		chCurr = 255;

	return chCurr;
}

void CProtoblastEffect::Render(CMemoryDC &dc, CPoint position)
{
	dc.FillSolidRect(position.x, position.y, m_area.cx, m_area.cy, m_currColor);
}

EStatus CProtoblastEffect::GetLastStatus()
{
	return STATUS_NONE;
}

CProtoblastEffectDisable::CProtoblastEffectDisable(CSize area, CProtoblastEffect *last) :
	CProtoblastEffect(area)
{
	m_currColor = last ? last->GetCurrColor() : m_endColor;
}

CProtoblastEffectDisable::~CProtoblastEffectDisable()
{
}

void CProtoblastEffectDisable::Tick()
{
	ChangeColor(m_endColor, 20, 20, 20);
}

EStatus CProtoblastEffectDisable::GetStatus()
{
	return STATUS_DISABLE;
}

CProtoblastEffectActive::CProtoblastEffectActive(CSize area, CProtoblastEffect *last) :
	CProtoblastEffect(area)
{
	m_currColor = last ? last->GetCurrColor() : m_endColor;
}

CProtoblastEffectActive::~CProtoblastEffectActive()
{
}

EStatus CProtoblastEffectActive::GetStatus()
{
	return STATUS_ACTIVE;
}

void CProtoblastEffectActive::Tick()
{
	ChangeColor(m_endColor, 20, 20, 20);
}

CProtoblastEffectHover::CProtoblastEffectHover(CSize area, CProtoblastEffect *last) :
	CProtoblastEffect(area)
{
	m_lastStatus = last->GetStatus();
	m_color = 0;
	m_colors[0] = RGB(255, 255, 153);
	m_colors[1] = RGB(255, 255, 102);
	m_colors[2] = RGB(255, 255, 51);
	m_colors[3] = m_colors[1];
	m_currColor = last->GetCurrColor();
}

CProtoblastEffectHover::~CProtoblastEffectHover()
{
}

void CProtoblastEffectHover::Tick()
{
	++m_tickCount;

	if (m_tickCount % 10 == 0)
	{
		m_tickCount = 0;
		m_color = (m_color + 1) % 4; 
	}
}

void CProtoblastEffectHover::Render(CMemoryDC &dc, CPoint position)
{
	dc.FillSolidRect(position.x, position.y, m_area.cx, m_area.cy, m_colors[m_color]);
}

EStatus CProtoblastEffectHover::GetStatus()
{
	return STATUS_HOVER;
}

EStatus CProtoblastEffectHover::GetLastStatus()
{
	return m_lastStatus;
}