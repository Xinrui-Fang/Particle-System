
// MOUSE_controlled.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMOUSE_controlledApp:
// �йش����ʵ�֣������ MOUSE_controlled.cpp
//

class CMOUSE_controlledApp : public CWinApp
{
public:
	CMOUSE_controlledApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMOUSE_controlledApp theApp;