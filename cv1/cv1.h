
// cv1.h : cv1 ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// Ccv1App:
// �� Ŭ������ ������ ���ؼ��� cv1.cpp�� �����Ͻʽÿ�.
//

class Ccv1App : public CWinApp
{
public:
	Ccv1App();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Ccv1App theApp;
