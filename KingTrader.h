
// KingTrader.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "res\resource.h"		// ������


// CKingTraderApp: 
// �йش����ʵ�֣������ KingTrader.cpp
//

class CKingTraderApp : public CWinApp
{
public:
	CKingTraderApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CKingTraderApp theApp;