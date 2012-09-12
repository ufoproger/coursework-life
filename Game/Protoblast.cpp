#include "stdafx.h"
#include "Protoblast.h"
#include "ProtoblastEffect.h"

CProtoblast::CProtoblast()
{
	m_frame = CSize(24, 24);
	m_area = m_frame - CSize(2, 2);
	m_effect = NULL;

	SetStatus(STATUS_DISABLE);
}

CProtoblast::~CProtoblast()
{
	if (m_effect)
		delete m_effect;
}

void CProtoblast::Render(CMemoryDC &dc, CPoint position)
{
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen *pLastPen = dc.SelectObject(&pen);

	dc.Rectangle(position.x, position.y, position.x + m_frame.cx, position.y + m_frame.cy);
	dc.SelectObject(pLastPen);
	m_effect->Render(dc, position + CPoint(1, 1));
}

void CProtoblast::Tick()
{
	m_effect->Tick();
}

void CProtoblast::Click(bool isLeftButton)
{
	if (isLeftButton)
		SetStatus(STATUS_ACTIVE);
	else
		SetStatus(STATUS_DISABLE);
}

void CProtoblast::Hover()
{
	SetStatus(STATUS_HOVER);
}


void CProtoblast::Unhover()
{
	SetStatus(m_effect->GetLastStatus());
}

void CProtoblast::SetStatus(EStatus status)
{
	if (status == STATUS_NONE)
		return;

	EStatus currStatus = m_effect ? m_effect->GetStatus() : STATUS_NONE;

	if (status == currStatus)
		return;

	if (status == STATUS_HOVER && currStatus != STATUS_DISABLE)
		return;

	CProtoblastEffect *temp = m_effect;

	switch (status)
	{
	case STATUS_ACTIVE:
		m_effect = new CProtoblastEffectActive(m_area, temp);
		break;

	case STATUS_HOVER:
		m_effect = new CProtoblastEffectHover(m_area, temp);
		break;

	default:
		m_effect = new CProtoblastEffectDisable(m_area, temp);
	}

	delete temp;
}

int CProtoblast::GetWidth()
{
	return m_frame.cx;
}

int CProtoblast::GetHeight()
{
	return m_frame.cy;
}

CSize CProtoblast::GetSize()
{
	return m_frame;
}

bool CProtoblast::IsLive()
{
	return (m_effect->GetStatus() == STATUS_ACTIVE);
}