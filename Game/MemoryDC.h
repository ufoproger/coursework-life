#pragma once

class CMemoryDC : public CDC 
{
public:
	CMemoryDC(CDC *, const CRect *pRect = NULL);
	~CMemoryDC();
	CMemoryDC* operator->();
	operator CMemoryDC*();

private:       
	CBitmap m_bitmap;
	CBitmap *m_oldBitmap;
	CDC *m_pDC;
	CRect m_rect;
	BOOL m_bMemDC;
};