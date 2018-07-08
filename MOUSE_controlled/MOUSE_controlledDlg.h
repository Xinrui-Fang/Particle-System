
// MOUSE_controlledDlg.h : ͷ�ļ�
//

#pragma once

#include "afxdialogex.h"
#include "afxwin.h"

// CMOUSE_controlledDlg �Ի���
class CMOUSE_controlledDlg : public CDialogEx
{
// ����
public:
	CMOUSE_controlledDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MOUSE_CONTROLLED_DIALOG };

	BOOL SetWindowPixelFormat(HDC hDC);                    // �趨���ظ�ʽ
    BOOL CreateViewGLContext(HDC hDC);                    // view GL Context
	void RenderScene();
    void RenderScene1();      
	void RenderScene_fire();// ���Ƴ���
	void RenderScene_huoyan();
	void RenderScene_star();
    HDC hrenderDC;                                                            // DC
    HGLRC hrenderRC;                                                        // RC
    int PixelFormat;                                                        // ���ظ�ʽ

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT nIDEvent);                // ����OnTimer����
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnStnClickedRender();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);   // ����OnMouseWheel����
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);        // ����OnMouseMove����

	void initparticles();
	void initparticles_fire();
	void initparticles_huoyan();
	void initparticles_star();

	void init();
	void init_fire();
	void init_huoyan();
	void init_star();

	afx_msg void OnClose();
//	afx_msg void OnBnClickedXuehua();
//	CButton m_snow;
	afx_msg void OnBnClickedSnow();
	afx_msg void OnBnClickedFire();
//	afx_msg void OnEnChangeNum();
	int m_num;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedHuoyan();
	afx_msg void OnBnClickedStar();
	int m_color;
	afx_msg void OnBnClickedButton2();
//	int m_life;
	float m_life;
	afx_msg void OnBnClickedButton3();
};
