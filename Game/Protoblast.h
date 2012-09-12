#pragma once

#include "stdafx.h"
#include "ProtoblastEffect.h"

class CProtoblast
{
public:
	CProtoblast();
	~CProtoblast();

	CSize GetSize();	
	void Render(CMemoryDC &, CPoint);
	void Tick();
	void Click(bool isLeftButton = true);
	void Hover();
	void Unhover();
	void Free() { if (m_effect) delete m_effect; }
	bool IsLive();
	int GetWidth();
	int GetHeight();


protected:
	CProtoblastEffect *m_effect;
	CSize m_frame;
	CSize m_area;
	
	void SetStatus(EStatus status);
};