#pragma once

#include "stdafx.h"
#include "afxdialogex.h"
#include "Game.h"

class CAboutDlg : public CDialogEx
{
public:
	enum {IDD = IDD_ABOUTBOX};

	CAboutDlg();

protected:
	virtual void DoDataExchange(CDataExchange *);

	DECLARE_MESSAGE_MAP()
};