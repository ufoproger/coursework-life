#include "stdafx.h"
#include "Game.h"
#include "MainFrm.h"
#include "Protoblast.h"
#include "AboutDialog.h"
#include <fstream>

BEGIN_MESSAGE_MAP(CGameApp, CWinApp)
	ON_COMMAND_RANGE(ID_EVOLVE_1_SECOND, ID_EVOLVE_HAND, &CGameApp::OnEvolve)
	ON_COMMAND(ID_APP_ABOUT, &CGameApp::OnAppAbout)
	ON_COMMAND(ID_NEW_GAME, &CGameApp::OnNewGame)
	ON_COMMAND(ID_OPEN_GAME, &CGameApp::OnOpenGame)
	ON_COMMAND(ID_SAVE_GAME, &CGameApp::OnSaveGame)
END_MESSAGE_MAP()

CGameApp::CGameApp()
{
	CreateNewGame();

	m_pause = false;
	m_hoverCell = CPoint(-1, -1);
	m_offset = CPoint(0, 0);
}

CGameApp::~CGameApp()
{
	FreeField();
}

CGameApp theApp;

void CGameApp::OnEvolve(UINT id)
{
	CheckMenuRadioItem(GetMenu(*m_pMainWnd), ID_EVOLVE_1_SECOND, ID_EVOLVE_HAND, id, MF_BYCOMMAND);

	switch (id)
	{
	case ID_EVOLVE_1_SECOND:
		m_evolveTime = 1;
		break;

	case ID_EVOLVE_3_SECONDS:
		m_evolveTime = 3;
		break;

	case ID_EVOLVE_10_SECONDS:
		m_evolveTime = 10;
		break;

	case ID_EVOLVE_42_MINUTES:
		m_evolveTime = 42 * 60;
		break;

	default:
		m_evolveTime = 0;
	}

	m_evolveTime *= m_fps;
	m_tickCount = 0;
	m_clock.SetTime(m_evolveTime);
}

BOOL CGameApp::InitInstance()
{
	INITCOMMONCONTROLSEX initCtrls;

	initCtrls.dwSize = sizeof(initCtrls);
	initCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&initCtrls);
	CWinApp::InitInstance();
	EnableTaskbarInteraction(FALSE);
	SetRegistryKey(_T("Mihail Snetkov"));

	CMainFrame* pFrame = new CMainFrame;

	if (!pFrame)
		return FALSE;
	
	m_pMainWnd = pFrame;
	pFrame->LoadFrame(IDR_MAINFRAME);
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	OnEvolve(ID_EVOLVE_3_SECONDS);

	return TRUE;
}

void CGameApp::CreateNewGame()
{
	FreeField();

	for (size_t i = 0; i < m_fieldHeight; ++i)
	{
		m_field.push_back(std::vector < CProtoblast * >());

		for (size_t j = 0; j < m_fieldWidth; j++)
			m_field[i].push_back(new CProtoblast());
	}
}

void CGameApp::FreeField()
{
	for (size_t i = 0; i < m_field.size(); ++i)
	{
		for (size_t j = 0; j < m_field[i].size(); ++j)
			delete m_field[i][j];

		m_field[i].clear();
	}

	m_field.clear();
}

void CGameApp::LeftButtonDoubleClick(UINT nFlags, CPoint point)
{
	CPoint cell;

	if (MousePositionToXY(point, cell))
		m_field[cell.x][cell.y]->Click();
}

void CGameApp::LeftButtonDown(UINT nFlags, CPoint point)
{
	m_oldMousePosition = CPoint(-1, -1);
}

void CGameApp::RightButtonUp(UINT nFlags, CPoint point)
{
	CPoint cell;

	if (MousePositionToXY(point, cell))
		m_field[cell.x][cell.y]->Click(false);

	MouseMove(0, point);
}

void CGameApp::NcMouseMove()
{
	if (m_hoverCell == CPoint(-1, -1))
		return;

	m_field[m_hoverCell.x][m_hoverCell.y]->Unhover();
	m_hoverCell = CPoint(-1, -1);
}

void CGameApp::MouseMove(UINT nFlags, CPoint point)
{
	if (m_pause)
		return;

	if (nFlags == MK_LBUTTON)
	{
		if (m_oldMousePosition != CPoint(-1, -1))
			m_offset += point - m_oldMousePosition;

		m_oldMousePosition = point;
	}

	CPoint cell;

	if (MousePositionToXY(point, cell))
	{
		if (m_hoverCell != cell && m_hoverCell != CPoint(-1, -1))
			m_field[m_hoverCell.x][m_hoverCell.y]->Unhover();

		m_hoverCell = cell;
		m_field[cell.x][cell.y]->Hover();
	}
	else
		if (m_hoverCell != CPoint(-1, -1))
		{
			m_field[m_hoverCell.x][m_hoverCell.y]->Unhover();
			m_hoverCell = CPoint(-1, -1);
		}
}

void CGameApp::Evolve()
{
	int movX[8] = {-1, -1, -1, 0, 1, 1, 1, 0};
	int movY[8] = {-1, 0, 1, 1, 1, 0, -1, -1};

	std::vector < std::vector < bool > > tempField(m_fieldHeight, std::vector < bool >(m_fieldWidth, false));

	for (int i = 0; i < m_fieldHeight; ++i)
		for (int j = 0; j < m_fieldWidth; ++j)
		{
			tempField[i][j] = m_field[i][j]->IsLive();

			int count = 0;

			for (int k = 0; k < 8; ++k)
			{
				int x = i + movX[k];
				int y = j + movY[k];

				if (x < 0 || y < 0 || x >= m_fieldHeight || y >= m_fieldWidth)
					continue;

				if (m_field[x][y]->IsLive())
					++count;
			}

			if (m_field[i][j]->IsLive())
			{
				if (count < 2 || count > 3)
					tempField[i][j] = false;
			}
			else
			{
				if (count == 3)
					tempField[i][j] = true;
			}
		}


	for (int i = 0; i < m_fieldHeight; ++i)
		for (int j = 0; j < m_fieldWidth; ++j)
			if (tempField[i][j] ^ m_field[i][j]->IsLive())
			{
				if (tempField[i][j])
					m_field[i][j]->Click();
				else
					m_field[i][j]->Click(false);
			}
}

void CGameApp::Tick()
{
	if (m_pause)
		return;

	++m_tickCount;

	for (int i = 0; i < m_fieldHeight; ++i)
		for (int j = 0; j < m_fieldWidth; ++j)
			m_field[i][j]->Tick();

	m_clock.Tick();

	if (m_evolveTime && m_tickCount % m_evolveTime == 0)
		Evolve();
}

void CGameApp::RenderField(CMemoryDC &dc, CRect screen)
{
	CSize protoblastSize(m_field[0][0]->GetSize());

	for (int i = 0; i < m_fieldHeight; ++i)
		for (int j = 0; j < m_fieldWidth; ++j)
			m_field[i][j]->Render(dc, CPoint(j * protoblastSize.cx, i * protoblastSize.cy) + m_offset);

	
	int deltaLeft = m_offset.x;
	int deltaRight = screen.Width() - m_offset.x - protoblastSize.cx;
	int deltaTop = m_offset.y;
	int deltaBottom = screen.Height() - m_offset.y - protoblastSize.cy;
	CBitmap bmp;
	CDC dcMemory;

	bmp.LoadBitmap(IDB_GRASS);
	dcMemory.CreateCompatibleDC(dc);
	dcMemory.SelectObject(&bmp);

	if (deltaTop > 0)
		for (int i = 1; i < deltaTop / protoblastSize.cy + 2; ++i)
			for (int j = -1; j < screen.Width() / protoblastSize.cx + 2; ++j)
			{
				int x = (m_offset.x % protoblastSize.cx) + protoblastSize.cx * j;
				int y = m_offset.y - protoblastSize.cy * i;

				dc->BitBlt(x, y, protoblastSize.cx, protoblastSize.cy, &dcMemory, 0, 0, SRCCOPY);
			}

	if (deltaBottom > 0)
		for (int i = 0; i < deltaBottom / protoblastSize.cy; ++i)
			for (int j = -1; j < screen.Width() / protoblastSize.cx + 2; ++j)
			{
				int x = (m_offset.x % protoblastSize.cx) + protoblastSize.cx * j;
				int y = m_offset.y + protoblastSize.cy * (m_fieldHeight + i);

				dc->BitBlt(x, y, protoblastSize.cx, protoblastSize.cy, &dcMemory, 0, 0, SRCCOPY);
			}

	if (deltaLeft > 0)
		for (int i = 0; i < m_fieldHeight; ++i)
			for (int j = 1; j < deltaLeft / protoblastSize.cx + 2; ++j)
			{
				int x = m_offset.x - protoblastSize.cx * j;
				int y = m_offset.y + protoblastSize.cy * i;

				dc->BitBlt(x, y, protoblastSize.cx, protoblastSize.cy, &dcMemory, 0, 0, SRCCOPY);
			}

	if (deltaRight > 0)
		for (int i = 0; i < m_fieldHeight; ++i)
			for (int j = 0; j < deltaRight / protoblastSize.cx; ++j)
			{
				int x = m_offset.x + (m_fieldWidth + j) * protoblastSize.cx;
				int y = m_offset.y + protoblastSize.cy * i;

				dc->BitBlt(x, y, protoblastSize.cx, protoblastSize.cy, &dcMemory, 0, 0, SRCCOPY);
			}

	if (screen.Height() > 60 && screen.Width() > 60 && !m_pause)
		m_clock.Render(dc, CPoint(screen.Width(), screen.Height()) - CPoint(10, 10) - CPoint(m_clock.GetWidth(), m_clock.GetHeight()));
}

int CGameApp::GetProtoblastCount()
{
	int count = 0;

	for (int i = 0; i < m_fieldHeight; ++i)
		for (int j = 0; j < m_fieldWidth; ++j)
			if (m_field[i][j]->IsLive())
				++count;

	return count;
}

CSize CGameApp::GetFieldSize()
{
	return CSize(m_fieldWidth, m_fieldHeight);
}

int CGameApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}

int CGameApp::GetFPS()
{
	return m_fps;
}

bool CGameApp::MousePositionToXY(CPoint position, CPoint &cell)
{
	position -= m_offset;

	if (position.x < 0 || position.y < 0)
		return false;

	int y = position.x / m_field[0][0]->GetWidth();
	int x = position.y / m_field[0][0]->GetHeight();

	if (x >= m_fieldHeight || y >= m_fieldWidth || x < 0 || y < 0)
		return false;

	cell = CPoint(x, y);
	
	return true;
}

void CGameApp::OnAppAbout()
{
	m_pause = true;

	CAboutDlg aboutDlg;

	aboutDlg.DoModal();
	m_pause = false;
}

void CGameApp::OnOpenGame()
{
	m_pause = true;

	CFileDialog dialog(TRUE, _T("*.life"), _T(""), 0, _T("Файлы игры (*.life)|*.life|"));

	if (dialog.DoModal() == IDOK)
	{
		CreateNewGame();

		std::ifstream fin(dialog.GetPathName());
		int count;

		fin >> count;

		for (int i = 0; i < count; ++i)
		{
			int x, y;

			fin >> x >> y;

			if (x > 0 && y > 0 && x < m_fieldHeight && y < m_fieldWidth)
				m_field[x][y]->Click();
		}

		MessageBox(NULL, _T("Игра загружена успешно!"), _T(""), MB_ICONINFORMATION);
		OnEvolve(ID_EVOLVE_HAND);
	}

	m_pause = false;
}

void CGameApp::OnSaveGame()
{
	if (GetProtoblastCount() == 0)
	{
		MessageBox(NULL, _T("Нечего сохранять!"), _T(""), MB_ICONERROR);

		return;
	}

	m_pause = true;

	CFileDialog dialog(FALSE, _T("*.life"), _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Файлы игры (*.life)|*.life|"));
	
	if (dialog.DoModal() == IDOK)
	{
		std::ofstream fout(dialog.GetPathName());

		fout << GetProtoblastCount() << std::endl;
		fout << std::endl;

		for (int i = 0; i < m_fieldHeight; ++i)
			for (int j = 0; j < m_fieldWidth; ++j)
				if (m_field[i][j]->IsLive())
					fout << i << " " << j << std::endl;

		MessageBox(NULL, _T("Игра сохранена успешно!"), _T(""), MB_ICONINFORMATION);
	}

	m_pause = false;
}

void CGameApp::KeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (!m_pause && nChar == 32 && m_evolveTime == 0)
		Evolve();
}

void CGameApp::OnNewGame()
{
	if (GetProtoblastCount() == 0)
		return;
	
	m_pause = true;

	if (MessageBox(NULL, _T("Начать новую игру?"), _T("Новая игра"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		CreateNewGame();

	m_pause = false;
}