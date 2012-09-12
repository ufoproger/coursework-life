#pragma once

#include "stdafx.h"
#include "MemoryDC.h"

enum EStatus {STATUS_NONE, STATUS_DISABLE, STATUS_ACTIVE, STATUS_HOVER};

class CProtoblastEffect
{
public:
	CProtoblastEffect(CSize area);
	virtual ~CProtoblastEffect();

	COLORREF GetCurrColor() { return m_currColor; }

	virtual void Tick() = 0;
	virtual void Render(CMemoryDC &, CPoint);
	virtual EStatus GetStatus() = 0;
	virtual EStatus GetLastStatus();

protected:
	COLORREF m_currColor;
	CSize m_area;

	void ChangeColor(COLORREF, int, int, int);
	byte MakeChangeColorStep (int, int, int);
};

class CProtoblastEffectDisable : public CProtoblastEffect
{
public:
	CProtoblastEffectDisable(CSize, CProtoblastEffect *);
	~CProtoblastEffectDisable();

	void Tick();
	EStatus GetStatus();

protected:
	static const COLORREF m_endColor = RGB(255, 255, 255);
};

class CProtoblastEffectActive : public CProtoblastEffect
{
public:
	CProtoblastEffectActive(CSize, CProtoblastEffect *);
	~CProtoblastEffectActive();

	void Tick();
	EStatus GetStatus();

protected:
	static const COLORREF m_endColor = RGB(87, 141, 249);
};

class CProtoblastEffectHover : public CProtoblastEffect
{
public:
	CProtoblastEffectHover(CSize, CProtoblastEffect *last = NULL);
	~CProtoblastEffectHover();
	
	void Tick();
	void Render(CMemoryDC &, CPoint);
	EStatus GetStatus();
	EStatus GetLastStatus();

protected:
	COLORREF m_colors[4];
	int m_color;
	int m_tickCount;
	EStatus m_lastStatus;
};