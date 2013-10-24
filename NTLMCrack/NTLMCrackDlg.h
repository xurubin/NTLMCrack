// NTLMCrackDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "bruteforce.h"

// CNTLMCrackDlg 对话框
class CNTLMCrackDlg : public CDialog
{
// 构造
public:
	CNTLMCrackDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_NTLMCRACK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CEdit m_EdtType2;
	CEdit m_EdtType3;
	afx_msg void OnBnClickedProcessmessages();
	void UpdateDisplayedInformation(void);
	Type2Msg Msg_2;
	Type3Msg Msg_3;
	PCB pcb;
	CEdit m_EdtUserName;
	CEdit m_EdtWorkstation;
	CEdit m_EdtChallenge;
	CEdit m_EdtEnteredPwd;
public:
	afx_msg void OnBnClickedButton1();
public:
	CEdit m_EdtAlphabet;
public:
	CEdit m_EdtMinLen;
public:
	CEdit m_EdtMaxLen;
public:
	CEdit m_EdtCurrentPwd;
public:
	CStatic m_ImgDescription;
public:
	CEdit m_EdtDisplayedYY;
public:
	afx_msg void OnBnClickedBtnfindyy();
public:
	CEdit m_EdtDisplayedPwd2;
public:
	afx_msg void OnBnClickedBtnfindpwd2();
public:
	afx_msg void OnBnClickedButton3();
public:
	CEdit m_EdtDisplayedX2;
public:
	CEdit m_EdtDisplayedPwd1;
public:
	afx_msg void OnBnClickedBtnfindpwd1();
public:
	afx_msg void OnBnClickedButton4();
public:
	CEdit m_EdtDisplayedCurrent;
public:
	CEdit m_EdtDisplayedSpeed;
};