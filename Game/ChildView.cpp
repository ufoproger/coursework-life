#include "stdafx.h"
#include "Game.h"
#include "ChildView.h"

CChildView::CChildView()	// Конструктор по-умолчанию
{
}

CChildView::~CChildView()	// Деструктор
{
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)	// Начало объявления перехватываемых сообщений
	ON_WM_PAINT()			// Сообщение рисования вида
	ON_WM_ERASEBKGND()		// Сообщение необходимости очистки фона перед рисованием
	ON_WM_LBUTTONDBLCLK()	// Сообщение двойного клика левой кнопки мыши
	ON_WM_MOUSEMOVE()		// Сообщение передвижения мыши в области вида
	ON_WM_LBUTTONDOWN()	// Сообщение нажатия левой кнопки мыши
	ON_WM_RBUTTONUP()	// Сообщение отпускания правой кнопки мыши
	ON_WM_NCMOUSEMOVE()	// Сообщения передвижения мыши вне области вида
	ON_WM_KEYDOWN()	// Сообщение нажатия на клавишу
END_MESSAGE_MAP()	// Конец объявления перехватываемых сообщений

void CChildView::OnNcMouseMove(UINT hitTest, CPoint point)	// Метод перехвата передвижения мыши вне области вида
{
	theApp.NcMouseMove();	// Передача управление объекту игровой логики
}

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 	// Метод подготовки к созданию окна
{
	if (!CWnd::PreCreateWindow(cs))	// Если у родительсткого класса не получилось подготовить окно к созданию
		return FALSE;	// То создавать нечего
	
	cs.dwExStyle |= WS_EX_CLIENTEDGE;	// Окно имеет рамку с притопленными краями
	cs.style &= ~WS_BORDER;	// Окно имеет бордюр
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL); // Хотим обычное человеческое окно

	return TRUE;	// Всё удалось, можно создавать окно
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)	// Метод определения необходимости очистки фона перед рисованием вида
{
	return FALSE;	// Очищать фон не трубуется
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)	// Метод нажатия на клавишу
{
	theApp.KeyDown(nChar, nRepCnt, nFlags);	// Передаём бразды правления объекту игровой логики
}

void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)	// Метод перехвата двойного клика левой кнопки мыши
{
	theApp.LeftButtonDoubleClick(nFlags, point);	// Передаём управление объекту игровой логики
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)	// Метод перехвата движения мыши в области вида
{
	theApp.MouseMove(nFlags, point);	// Передаём управление объекту игровой логики
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)	// Метод перехвата нажатия левой кнопки мыши
{
	theApp.LeftButtonDown(nFlags, point);	// Передаём управление объекту игровой логики
}

void CChildView::OnRButtonUp(UINT nFlags, CPoint point)	// Метод перехвата отпускания правой кнопки мыши
{
	theApp.RightButtonUp(nFlags, point);	// Передаём управление объекту игровой логики
}

void CChildView::OnPaint() 	// Метод рисования в виде
{
	CPaintDC dc(this);	// Получаем текущий контекст рисования
	CMemoryDC pDC(&dc);	// Хотим двойную буфферизацию, чтобы не мерцало
	CRect rect;	// Прямоугольник

	GetClientRect(&rect);	// Получаем размеры вида
	theApp.RenderField(pDC, rect);	// Передаём управление объекту игровой логики для отрисовки всего и вся
}