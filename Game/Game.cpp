#include "stdafx.h"
#include "Game.h"
#include "MainFrm.h"
#include "Protoblast.h"
#include "AboutDialog.h"
#include <fstream>

BEGIN_MESSAGE_MAP(CGameApp, CWinApp)	// Макрос начала объявления перехватываемых сообщений
	ON_COMMAND_RANGE(ID_EVOLVE_1_SECOND, ID_EVOLVE_HAND, &CGameApp::OnEvolve)	// Сообщения выбора скорости эволюции
	ON_COMMAND(ID_APP_ABOUT, &CGameApp::OnAppAbout)	// Сообщение о заинтересованности в просмотре пункта "О программе"
	ON_COMMAND(ID_NEW_GAME, &CGameApp::OnNewGame)	// Кто-то хочет начать новую игру - вот и сообщение
	ON_COMMAND(ID_OPEN_GAME, &CGameApp::OnOpenGame)	// Сообщение пункта "Открыть игру"
	ON_COMMAND(ID_SAVE_GAME, &CGameApp::OnSaveGame)	// Сообщение пункта "Сохранить игру как"
END_MESSAGE_MAP()	// Макрос конца объявления перехватываемых сообщений

CGameApp::CGameApp()	// Конструктор
{
	CreateNewGame();	// Создаём новую игру

	m_pause = false;	// Снимаем игру с паузы
	m_hoverCell = CPoint(-1, -1);	// Никакая ячейка не выделена
	m_offset = CPoint(0, 0);	// Поле никуда не сдвинуто
}

CGameApp::~CGameApp()	// Деструктор
{
	FreeField();	// Удаляем игровое поле из памяти
}

CGameApp theApp;	// Объект игрового класса

void CGameApp::OnEvolve(UINT id)	// Метод нажатия на один из пунктов установки скорости эволюции, где-то в меню сверху
{
	CheckMenuRadioItem(GetMenu(*m_pMainWnd), ID_EVOLVE_1_SECOND, ID_EVOLVE_HAND, id, MF_BYCOMMAND);	// Задаем выделенный пункт в диапазоне пункт в меню сверху

	switch (id)	// В зависимости от желаемого
	{
	case ID_EVOLVE_1_SECOND:	// Раз в секунду
		m_evolveTime = 1;	// Пока секунда
		break;

	case ID_EVOLVE_3_SECONDS:	// Раз в три секунды
		m_evolveTime = 3;	// Три секунды
		break;

	case ID_EVOLVE_10_SECONDS:	// Раз в десять секунд
		m_evolveTime = 10;	// Десять секунд
		break;

	case ID_EVOLVE_42_MINUTES:	// Раз в сорок две минуты
		m_evolveTime = 42 * 60;	// Сорок два умножить на шестьдесят секунд
		break;

	default:
		m_evolveTime = 0;	// Ручной режим эволюции
	}

	m_evolveTime *= m_fps;	// Умножаем на количество кадров в секунду
	m_tickCount = 0;	// С момента последней эволюции еще ни одного момента не прошло
	m_clock.SetTime(m_evolveTime);	// Задаём индикатору как часто ему крутиться
}

BOOL CGameApp::InitInstance()	// Метод инициализации окна
{
	INITCOMMONCONTROLSEX initCtrls;	// Настройка контролов

	initCtrls.dwSize = sizeof(initCtrls);	// Размер
	initCtrls.dwICC = ICC_WIN95_CLASSES;	// 	Стиль
	InitCommonControlsEx(&initCtrls);	// Задаём
	CWinApp::InitInstance();	// Родителю даём возможность тоже проинициализироваться
	EnableTaskbarInteraction(FALSE);	// Настройки поведения в панели задач
	SetRegistryKey(_T("Mihail Snetkov"));

	CMainFrame* pFrame = new CMainFrame;	// Создаём окно

	if (!pFrame)	// Если не создали окно
		return FALSE;	// То играть не во что
	
	m_pMainWnd = pFrame;	// Иначе это у нас главное окно
	pFrame->LoadFrame(IDR_MAINFRAME);	// Загружаем верхнее меню
	pFrame->ShowWindow(SW_SHOW);	// Режим отображения
	pFrame->UpdateWindow();	// Обновляем окно

	OnEvolve(ID_EVOLVE_3_SECONDS);	// Задаём частоту эволюций

	return TRUE;	// Все отлично, можно играть!
}

void CGameApp::CreateNewGame()	// Метод создания новой игры
{
	FreeField();	// Вдруг до этого уже играли и есть что удалить

	for (size_t i = 0; i < m_fieldHeight; ++i)	// Пробегая по высоте поля
	{
		m_field.push_back(std::vector < CProtoblast * >());	// Создаем массив строк поля

		for (size_t j = 0; j < m_fieldWidth; j++)	// И пробегая по ширине поля
			m_field[i].push_back(new CProtoblast());	// Создаём клетку
	}
}

void CGameApp::FreeField()	// Метод удаления поля
{
	for (size_t i = 0; i < m_field.size(); ++i)	// Пробегая по количеству строк в поле
	{
		for (size_t j = 0; j < m_field[i].size(); ++j)	// Промегаем по всей строке
			delete m_field[i][j];	// И удаляем каждую клетку

		m_field[i].clear();	// Удаляем строку поля
	}

	m_field.clear();	// Удаляем массив строк
}

void CGameApp::LeftButtonDoubleClick(UINT nFlags, CPoint point)	// Метод от двойного клика левой кнопки мыши
{
	CPoint cell;	// Представим, что в эту координату на поле кликнули

	if (MousePositionToXY(point, cell))	// Если попали кликом по полю
		m_field[cell.x][cell.y]->Click();	// То оживляем клетку
}

void CGameApp::LeftButtonDown(UINT nFlags, CPoint point)	// Метод от нажимания левой кнопки мыши
{
	m_oldMousePosition = CPoint(-1, -1);	// Сбрасываем последнее положение кнопки мыши
}

void CGameApp::RightButtonUp(UINT nFlags, CPoint point)	// Метод от отпускания правой кнопки мыши
{
	CPoint cell;	// Будто сюда кликнули

	if (MousePositionToXY(point, cell))	// Если попали в поле
		m_field[cell.x][cell.y]->Click(false);	// Убиваем клетку

	MouseMove(0, point);	// Скорее всего убитую клетку надо выделить
}

void CGameApp::NcMouseMove()	// Метод от курсора, вышеднего за пределы игрового поля
{
	if (m_hoverCell == CPoint(-1, -1))	// Если никакая клетка не была выделена
		return;	// То, собственно, делать нечего

	m_field[m_hoverCell.x][m_hoverCell.y]->Unhover();	// Иначе унвыделяем клетку
	m_hoverCell = CPoint(-1, -1);	// И теперь точно выделять нечего
}

void CGameApp::MouseMove(UINT nFlags, CPoint point)	// Метод от передвижения курсора по игровому полю
{
	if (m_pause)	// Если игра на паузе
		return;	// То делать нечего

	if (nFlags == MK_LBUTTON)	// Если двигаем с зажатой левой кнопкой мыши
	{
		if (m_oldMousePosition != CPoint(-1, -1))	// Если мышь чуть-чуть передвинулась, потянув за собой поле
			m_offset += point - m_oldMousePosition;	// То тянем поле за мышкой

		m_oldMousePosition = point;	// А теперь это актуальное положение мыши
	}

	CPoint cell;	// Вдруг задели клетку на поле

	if (MousePositionToXY(point, cell))	// Если попали по клетке, пока водили курсором
	{
		if (m_hoverCell != cell && m_hoverCell != CPoint(-1, -1))	// Если клетка не выделена, а есть другая выделенная клетка
			m_field[m_hoverCell.x][m_hoverCell.y]->Unhover();	// То другую веделенную клетку унвыделим

		m_hoverCell = cell;	// А это выделим
		m_field[cell.x][cell.y]->Hover();	// Выделим, я сказал. :)
	}
	else	// Иначе если по клетке не попали
		if (m_hoverCell != CPoint(-1, -1))	// Если есть выделенные клетки
		{
			m_field[m_hoverCell.x][m_hoverCell.y]->Unhover();	// Унвыделим выделенную
			m_hoverCell = CPoint(-1, -1);	// И пометим, что ничего не выделено
		}
}

void CGameApp::Evolve()	// Метод эволюции-революции
{
	int movX[8] = {-1, -1, -1, 0, 1, 1, 1, 0};	// Соседи по оси X
	int movY[8] = {-1, 0, 1, 1, 1, 0, -1, -1};	// Соседи по оси Y

	std::vector < std::vector < bool > > tempField(m_fieldHeight, std::vector < bool >(m_fieldWidth, false));	// Создаём пустое поле

	for (int i = 0; i < m_fieldHeight; ++i)	// Пробегаем по всем строкам поля
		for (int j = 0; j < m_fieldWidth; ++j)	// Ну и по каждой строке в частности
		{
			tempField[i][j] = m_field[i][j]->IsLive();	// Помечаем, если клетка живая

			int count = 0;	// Количество соседей

			for (int k = 0; k < 8; ++k)	// Пробегаем по соседям
			{
				int x = i + movX[k];	// Координаты соседа по оси X
				int y = j + movY[k];	// Координаты соседа по оси Y

				if (x < 0 || y < 0 || x >= m_fieldHeight || y >= m_fieldWidth)	// Если это сосед из параллельного мира
					continue;	// То с ним лучше не связываться

				if (m_field[x][y]->IsLive())	// Если сосед живой
					++count;	// То он добавляется в копилку живых соседей
			}

			if (m_field[i][j]->IsLive())	// Если текущая клетка живая
			{
				if (count < 2 || count > 3)	// И количество соседей не в норме
					tempField[i][j] = false;	// То пора умереть
			}
			else	// Иначе текущая клетка не живая
			{
				if (count == 3)	// Но количество соседей позволяет
					tempField[i][j] = true;	// Клетке ожить
			}
		}


	for (int i = 0; i < m_fieldHeight; ++i)	// Пробегаем по всем строкам 
		for (int j = 0; j < m_fieldWidth; ++j)	// В каждой строке
			if (tempField[i][j] ^ m_field[i][j]->IsLive())	// Если у нас есть разница в статусе
			{
				if (tempField[i][j])	// То если клетке суждено ожить
					m_field[i][j]->Click();	// Она оживёт
				else	// Иначе
					m_field[i][j]->Click(false);	// Она умрём
			}
}

void CGameApp::Tick()	// Метод тика
{
	if (m_pause)	// Если пауза
		return;	// То ничего делать не надо

	++m_tickCount;	// Увеличиваем счетчик с последней эволюции

	for (int i = 0; i < m_fieldHeight; ++i)	// Пробегаем по всем строкам
		for (int j = 0; j < m_fieldWidth; ++j)	// И по каждой строке в частности
			m_field[i][j]->Tick();	// Передаём тик в клетки

	m_clock.Tick();	// Часы тоже тикают

	if (m_evolveTime && m_tickCount % m_evolveTime == 0)	// Если пришло время для эволюции
		Evolve();	// То эволюции быть
}

void CGameApp::RenderField(CMemoryDC &dc, CRect screen)	// Метод отрисовки игрового поля
{
	CSize protoblastSize(m_field[0][0]->GetSize());	// Получаем размеры клетки

	for (int i = 0; i < m_fieldHeight; ++i)	// Пробегаем по всем строкам
		for (int j = 0; j < m_fieldWidth; ++j)	// И в каждой строке
			m_field[i][j]->Render(dc, CPoint(j * protoblastSize.cx, i * protoblastSize.cy) + m_offset);	// Отрисовываем клетки

	
	int deltaLeft = m_offset.x;	// Свободное пространство слева
	int deltaRight = screen.Width() - m_offset.x - protoblastSize.cx;	// Справа
	int deltaTop = m_offset.y;	// Сверху
	int deltaBottom = screen.Height() - m_offset.y - protoblastSize.cy;	// И снизу
	CBitmap bmp;	// Рисунок травы (тайтл)
	CDC dcMemory;	// Контекст для рисования в памяти

	bmp.LoadBitmap(IDB_GRASS);	// Загружаем травку
	dcMemory.CreateCompatibleDC(dc);	// Создаем совместимый контекста
	dcMemory.SelectObject(&bmp);	// Выбираем рисунок

	if (deltaTop > 0)	// Если сверху не хватает зелени, то заполняем
		for (int i = 1; i < deltaTop / protoblastSize.cy + 2; ++i)
			for (int j = -1; j < screen.Width() / protoblastSize.cx + 2; ++j)
			{
				int x = (m_offset.x % protoblastSize.cx) + protoblastSize.cx * j;	// Координаты травы по оси X
				int y = m_offset.y - protoblastSize.cy * i;	// Координаты травы по оси Y

				dc->BitBlt(x, y, protoblastSize.cx, protoblastSize.cy, &dcMemory, 0, 0, SRCCOPY);	// Рисуем траву
			}

	if (deltaBottom > 0)	// Если снизу не хватает зелени, то опять её дорисовываем
		for (int i = 0; i < deltaBottom / protoblastSize.cy; ++i)
			for (int j = -1; j < screen.Width() / protoblastSize.cx + 2; ++j)
			{
				int x = (m_offset.x % protoblastSize.cx) + protoblastSize.cx * j;
				int y = m_offset.y + protoblastSize.cy * (m_fieldHeight + i);

				dc->BitBlt(x, y, protoblastSize.cx, protoblastSize.cy, &dcMemory, 0, 0, SRCCOPY);
			}

	if (deltaLeft > 0)	// Если слева пусто, то тоже траву рисуем
		for (int i = 0; i < m_fieldHeight; ++i)
			for (int j = 1; j < deltaLeft / protoblastSize.cx + 2; ++j)
			{
				int x = m_offset.x - protoblastSize.cx * j;
				int y = m_offset.y + protoblastSize.cy * i;

				dc->BitBlt(x, y, protoblastSize.cx, protoblastSize.cy, &dcMemory, 0, 0, SRCCOPY);
			}

	if (deltaRight > 0)	// Справа пусто? Нарисуем траву!
		for (int i = 0; i < m_fieldHeight; ++i)
			for (int j = 0; j < deltaRight / protoblastSize.cx; ++j)
			{
				int x = m_offset.x + (m_fieldWidth + j) * protoblastSize.cx;
				int y = m_offset.y + protoblastSize.cy * i;

				dc->BitBlt(x, y, protoblastSize.cx, protoblastSize.cy, &dcMemory, 0, 0, SRCCOPY);
			}

	if (screen.Height() > 60 && screen.Width() > 60 && !m_pause)	// Если часы-индикатор влезают в окно
		m_clock.Render(dc, CPoint(screen.Width(), screen.Height()) - CPoint(10, 10) - CPoint(m_clock.GetWidth(), m_clock.GetHeight()));	// То и их рисуем
}

int CGameApp::GetProtoblastCount()	// Метод получения количества живых клеток
{
	int count = 0;	// Только экстрасенс на текущий момент сколько живых клеток на поле

	for (int i = 0; i < m_fieldHeight; ++i)	// А мы пробегаем по всем строкам
		for (int j = 0; j < m_fieldWidth; ++j)	// В каждой строке по всем клеткам
			if (m_field[i][j]->IsLive())	// И если клетка жива
				++count;	// То увеличиваем счётчик

	return count;	// Возвращаем результат
}

CSize CGameApp::GetFieldSize()	// Метод получения размеров поля
{
	return CSize(m_fieldWidth, m_fieldHeight);	// Формируем размер поля
}

int CGameApp::ExitInstance()	// Метод выхода
{
	return CWinApp::ExitInstance();	// Родительский класс тоже хочет выйти
}

int CGameApp::GetFPS()	// Метод получения количества кадров в секунду
{
	return m_fps;	// Количество кадров в секунду
}

bool CGameApp::MousePositionToXY(CPoint position, CPoint &cell)	// Метод получения координат клетки по координатам курсора мыши
{
	position -= m_offset;	// Убираем сдвиг

	if (position.x < 0 || position.y < 0)	// Если мы не в поле
		return false;	// То сразу нет

	int y = position.x / m_field[0][0]->GetWidth();	// Иначе координаты по оси Y
	int x = position.y / m_field[0][0]->GetHeight();	// Потом по оси X

	if (x >= m_fieldHeight || y >= m_fieldWidth)	// Если поле кончилось
		return false;	// То опять нет

	cell = CPoint(x, y);	// Иначе искомые координаты клетки поля под курсором
	
	return true;	// Все хорошо, данные годные
}

void CGameApp::OnAppAbout()	// Метод вызова меню "О программе"
{
	m_pause = true;	// Ставим на паузу

	CAboutDlg aboutDlg;	// Диалог "О программе"

	aboutDlg.DoModal();	// Открываем диалог, наслаждаемся
	m_pause = false;	// Закрыли диалог, сняли с паузы игру
}

void CGameApp::OnOpenGame()	// Метод вызова меню "Открыть игру"
{
	m_pause = true;	// Ставим на паузу

	CFileDialog dialog(TRUE, _T("*.life"), _T(""), 0, _T("Файлы игры (*.life)|*.life|"));	// Создаём диалог открытия файла

	if (dialog.DoModal() == IDOK)	// Если нажали ОК
	{
		CreateNewGame();	// То создаем новую игру

		std::ifstream fin(dialog.GetPathName());	// Открываем файл с сохраненной игрой
		int count;	// Количество живых клеток в файле

		fin >> count;	// Получаем из файла количество клеток

		for (int i = 0; i < count; ++i)	// И теперь
		{
			int x, y;	// Координаты клетки на поле

			fin >> x >> y;	// Считываем координаты

			if (x > 0 && y > 0 && x < m_fieldHeight && y < m_fieldWidth)	// Если клетка влезаем на поле
				m_field[x][y]->Click();	// Она оживаем
		}

		MessageBox(NULL, _T("Игра загружена успешно!"), _T(""), MB_ICONINFORMATION);	// Радостное известие
		OnEvolve(ID_EVOLVE_HAND);	// Режим эволюции - ручками
	}

	m_pause = false;	// Снимаем с паузы
}

void CGameApp::OnSaveGame()	// Метод вызова меню "Сохранить игру как"
{
	if (GetProtoblastCount() == 0)	// Если живых клеток нет
	{
		MessageBox(NULL, _T("Нечего сохранять!"), _T(""), MB_ICONERROR);	// То плохая новость

		return;	// И можно ничего не сохранять
	}

	m_pause = true;	// Ставим на паузу

	CFileDialog dialog(FALSE, _T("*.life"), _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Файлы игры (*.life)|*.life|"));	// Создаем диалог сохранения
	
	if (dialog.DoModal() == IDOK)	// Если пользователь выбрал куда сохранить
	{
		std::ofstream fout(dialog.GetPathName());	// Открываем этот файл

		fout << GetProtoblastCount() << std::endl;	// Выводим количество живых клеток
		fout << std::endl;

		for (int i = 0; i < m_fieldHeight; ++i)	// А потом пробегаем по всем строкам
			for (int j = 0; j < m_fieldWidth; ++j)	// В каждой строке по всем клеткам
				if (m_field[i][j]->IsLive())	// И если клетка живая
					fout << i << " " << j << std::endl;	// То ей суждено быть записанной в файл

		MessageBox(NULL, _T("Игра сохранена успешно!"), _T(""), MB_ICONINFORMATION);	// Радостное сообщение
	}

	m_pause = false;	// Пауза закончилась
}

void CGameApp::KeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)	// Метод от нажатия клавишы
{
	if (!m_pause && nChar == 32 && m_evolveTime == 0)	// Если нажали пробел и игра не на паузе и эволюция ручная
		Evolve();	// То время для эволюции
}

void CGameApp::OnNewGame()	// Метод вызова меню "Новая игра"
{
	if (GetProtoblastCount() == 0)	// Если и так чисто
		return;	// То делать нечего
	
	m_pause = true;	// Иначе пауза

	if (MessageBox(NULL, _T("Начать новую игру?"), _T("Новая игра"), MB_YESNO | MB_ICONQUESTION) == IDYES)	// Интересный вопрос
		CreateNewGame();	// Если согласны на новую игру, то делаем её

	m_pause = false;	// Пауза закончилась
}