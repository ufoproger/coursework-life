#pragma once

#include "stdafx.h"
#include "afxdialogex.h"
#include "Game.h"

/* Класс диалога "О программе", наследуемый от класса диалогов с поддеркой передачи данных */
class CAboutDlg : public CDialogEx
{
public:
	enum {IDD = IDD_ABOUTBOX};	// Идентификатор диалога

	CAboutDlg();	// Конструктор по-умолчанию

private:
	DECLARE_MESSAGE_MAP() // Макрос конца класса для объявления перехватываемых сообщений
};