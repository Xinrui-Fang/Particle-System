
// MOUSE_controlledDlg.h : 头文件
//

#pragma once

#include "afxdialogex.h"
#include "afxwin.h"

// CMOUSE_controlledDlg 对话框
class CMOUSE_controlledDlg : public CDialogEx
{
// 构造
public:
	CMOUSE_controlledDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MOUSE_CONTROLLED_DIALOG };

	BOOL SetWindowPixelFormat(HDC hDC);                    // 设定像素格式
    BOOL CreateViewGLContext(HDC hDC);                    // view GL Context
	void RenderScene();
    void RenderScene1();      
	void RenderScene_fire();// 绘制场景
	void RenderScene_huoyan();
	void RenderScene_star();
    HDC hrenderDC;                                                            // DC
    HGLRC hrenderRC;                                                        // RC
    int PixelFormat;                                                        // 像素格式

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT nIDEvent);                // 重载OnTimer函数
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnStnClickedRender();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);   // 重载OnMouseWheel函数
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);        // 重载OnMouseMove函数

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
