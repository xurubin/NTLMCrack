// NTLMCrack.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CNTLMCrackApp:
// �йش����ʵ�֣������ NTLMCrack.cpp
//

class CNTLMCrackApp : public CWinApp
{
public:
	CNTLMCrackApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CNTLMCrackApp theApp;