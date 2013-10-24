// NTLMCrackDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NTLMCrack.h"
#include "NTLMCrackDlg.h"
#include "des.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNTLMCrackDlg 对话框




CNTLMCrackDlg::CNTLMCrackDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNTLMCrackDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNTLMCrackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EdtType2, m_EdtType2);
	DDX_Control(pDX, IDC_EdtType3, m_EdtType3);
	DDX_Control(pDX, IDC_EdtUserName, m_EdtUserName);
	DDX_Control(pDX, IDC_EdtWorkstation, m_EdtWorkstation);
	DDX_Control(pDX, IDC_EdtChallenge, m_EdtChallenge);
	DDX_Control(pDX, IDC_EdtPwd, m_EdtEnteredPwd);
	DDX_Control(pDX, IDC_EdtAlphabet, m_EdtAlphabet);
	DDX_Control(pDX, IDC_EdtAlphabet2, m_EdtMinLen);
	DDX_Control(pDX, IDC_EdtAlphabet3, m_EdtMaxLen);
	DDX_Control(pDX, IDC_EdtAlphabet4, m_EdtCurrentPwd);
	DDX_Control(pDX, IDC_EDIT1, m_EdtDisplayedYY);
	DDX_Control(pDX, IDC_EDITPwd2, m_EdtDisplayedPwd2);
	DDX_Control(pDX, IDC_EdtDisplayedX2, m_EdtDisplayedX2);
	DDX_Control(pDX, IDC_EDITPwd1, m_EdtDisplayedPwd1);
	DDX_Control(pDX, IDC_EdtCurrent, m_EdtDisplayedCurrent);
	DDX_Control(pDX, IDC_EdtSpeed, m_EdtDisplayedSpeed);
}

BEGIN_MESSAGE_MAP(CNTLMCrackDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ProcessMessages, &CNTLMCrackDlg::OnBnClickedProcessmessages)
	ON_BN_CLICKED(IDC_BUTTON1, &CNTLMCrackDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BtnFindYY, &CNTLMCrackDlg::OnBnClickedBtnfindyy)
	ON_BN_CLICKED(IDC_BtnFindPwd2, &CNTLMCrackDlg::OnBnClickedBtnfindpwd2)
	ON_BN_CLICKED(IDC_BUTTON3, &CNTLMCrackDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BtnFindPwd1, &CNTLMCrackDlg::OnBnClickedBtnfindpwd1)
	ON_BN_CLICKED(IDC_BUTTON4, &CNTLMCrackDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CNTLMCrackDlg 消息处理程序

BOOL CNTLMCrackDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_EdtEnteredPwd.SetLimitText(14);
	// TODO: 在此添加额外的初始化代码
	m_EdtType2.SetWindowTextA("TlRMTVNTUAACAAAADgAOADgAAAAFgoGiUtCewfgxY2YAAAAAAAAAAHAAcABGAAAABQCTCAAAAA9EAFUATABXAEkAQwBIAAIADgBEAFUATABXAEkAQwBIAAEACgBMAEUATQBPAE4ABAAcAGQAdQBsAHcAaQBjAGgALgBvAHIAZwAuAHUAawADACgAbABlAG0AbwBuAC4AZAB1AGwAdwBpAGMAaAAuAG8AcgBnAC4AdQBrAAAAAAA=");
	//Xur
	m_EdtType3.SetWindowTextA("TlRMTVNTUAADAAAAGAAYAHwAAAAYABgAlAAAAA4ADgBIAAAADAAMAFYAAAAaABoAYgAAAAAAAACsAAAABYKAogUBKAoAAAAPRABVAEwAVwBJAEMASABhAHYAcwBoAHUAYQBCAEwARQBXAEgATQAtAEwAQQBQAFQATwBQALYewY8mUNcwSfKLlb3jPdqrsBBP8ksLA8iqLWZbpHsMdSvrkIuRvdcVGCVh9BI1Rw==");

	m_EdtAlphabet.SetWindowTextA("EARIOTNSLCUDPMHGBFYWKVXZJQ0123456789");
	m_EdtMinLen.SetWindowTextA("0");
	m_EdtMaxLen.SetWindowTextA("7");
	
	SetTimer(1,1000,0);
	memset(&pcb,0,sizeof(pcb));
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNTLMCrackDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CNTLMCrackDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CNTLMCrackDlg::OnBnClickedProcessmessages()
{
	char txt[1024];
	m_EdtType2.GetWindowTextA(txt,sizeof(txt));
	if (!ParseType2Msg(txt,&Msg_2)) {
		AfxMessageBox("Parse Type2 Message Error");
		return;
	}
	m_EdtType3.GetWindowTextA(txt,sizeof(txt));
	if (!ParseType3Msg(txt,&Msg_3)) {
		AfxMessageBox("Parse Type3 Message Error");
		return;
	}
	UpdateDisplayedInformation();
	memset(&pcb,0,sizeof(pcb));
}

void CNTLMCrackDlg::UpdateDisplayedInformation(void)
{
	CString str;
	
	str = "";
	str.AppendFormat("%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x(%s)",
						Msg_2.challenge[0],Msg_2.challenge[1],
						Msg_2.challenge[2],Msg_2.challenge[3],
						Msg_2.challenge[4],Msg_2.challenge[5],
						Msg_2.challenge[6],Msg_2.challenge[7],
						Msg_2.challenge
						);
	m_EdtChallenge.SetWindowText(str);

	str = (wchar_t*)Msg_3.username;
	m_EdtUserName.SetWindowText(str);
	str = (wchar_t*)Msg_3.workstation;
	m_EdtWorkstation.SetWindowText(str);
	
}

void CNTLMCrackDlg::OnBnClickedButton1()
{
	static const char LM_MAGIC[] = "KGS!@#$%";
	char txtBuf[15];
	CString str;
	memset(txtBuf,0,sizeof(txtBuf));
	m_EdtEnteredPwd.GetWindowTextA(str);
	str.MakeUpper();
	strcpy_s(txtBuf,str.GetLength()+1,str);
	
	/////////Calculate Hash/////////////////////////////
	gl_des_ctx des_ctx;
	char hash[21];
	memset(hash,0,sizeof(hash));
	gl_des_setkey7(&des_ctx,(unsigned char*)txtBuf);
	gl_des_ecb_encrypt(&des_ctx,LM_MAGIC,hash);
	gl_des_setkey7(&des_ctx,(unsigned char*)&txtBuf[7]);
	gl_des_ecb_encrypt(&des_ctx,LM_MAGIC,&hash[8]);

	///////Calculate Response/////////////////////////
	char response[24];
	gl_des_setkey7(&des_ctx,(unsigned char*)&hash[0]);
	gl_des_ecb_encrypt(&des_ctx,(const char *)Msg_2.challenge,&response[0]);
	gl_des_setkey7(&des_ctx,(unsigned char*)&hash[7]);
	gl_des_ecb_encrypt(&des_ctx,(const char *)Msg_2.challenge,&response[8]);
	gl_des_setkey7(&des_ctx,(unsigned char*)&hash[14]);
	gl_des_ecb_encrypt(&des_ctx,(const char *)Msg_2.challenge,&response[16]);

	/////////Verify Response///////////////////////
	if (!memcmp(response,Msg_3.lmresponse,sizeof(response)))
		AfxMessageBox("Correct Password",MB_ICONINFORMATION);
	else 
		AfxMessageBox("Incorrect Password",MB_ICONSTOP);
}

void CNTLMCrackDlg::OnBnClickedBtnfindyy()
{
	int yy;
	gl_des_ctx des_ctx;
	unsigned char key[7],r[8];

	memset(key,0,sizeof(key));
	for(yy=0;yy<0xffff;yy++) {
		int j;
		j=0;
		key[0] = yy&0xFF;
		key[1] = yy>>8;
		gl_des_setkey7(&des_ctx,key);
		gl_des_ecb_encrypt(&des_ctx,(const char*)Msg_2.challenge,(char*)r);
		if(memcmp(r,Msg_3.lmresponse+16,8)==0) {
			CString str;
			str.AppendFormat("%.2x%.2x",key[0],key[1]);
			m_EdtDisplayedYY.SetWindowTextA(str);
			pcb.YY2[0] = key[0];
			pcb.YY2[1] = key[1];
			//break;
		}
	}
}

void CNTLMCrackDlg::OnBnClickedBtnfindpwd2()
{
	if ((pcb.YY2[0]==0)&&(pcb.YY2[1]==0)) {
		AfxMessageBox("Y'Y' must be found first");
		return;
	}

	char txt[256];
	m_EdtMinLen.GetWindowTextA(txt,256);
	pcb.MinLen = atol(txt);
	m_EdtMaxLen.GetWindowTextA(txt,256);
	pcb.MaxLen = atol(txt);
	m_EdtCurrentPwd.GetWindowTextA(txt,256);
	strcpy(pcb.CurrentPwd,txt);
	m_EdtAlphabet.GetWindowTextA(txt,256);
	strcpy(pcb.Alphabet,txt);
	memcpy(pcb.challenge,Msg_2.challenge,sizeof(Msg_2.challenge));
	memcpy(pcb.response2,Msg_3.lmresponse+8,8);

	CreateThread(NULL,0,BruteForcePwd2,&pcb,0,NULL);
}

void CNTLMCrackDlg::OnBnClickedButton3()
{
	m_EdtDisplayedPwd2.SetWindowTextA(pcb.CurrentPwd);
	CString str;
	str.AppendFormat("%.2x",pcb.X2);
	m_EdtDisplayedX2.SetWindowTextA(str);
}

void CNTLMCrackDlg::OnBnClickedBtnfindpwd1()
{
	if ((pcb.YY2[0]==0)&&(pcb.YY2[1]==0)) {
		AfxMessageBox("Y'Y' must be found first");
		return;
	}
	if (pcb.X2==0) {
		AfxMessageBox("X' must be found first");
		return;
	}

	char txt[256];
	m_EdtMinLen.GetWindowTextA(txt,256);
	pcb.MinLen = atol(txt);
	m_EdtMaxLen.GetWindowTextA(txt,256);
	pcb.MaxLen = atol(txt);
	m_EdtCurrentPwd.GetWindowTextA(txt,256);
	strcpy(pcb.CurrentPwd,txt);
	m_EdtAlphabet.GetWindowTextA(txt,256);
	strcpy(pcb.Alphabet,txt);
	memcpy(pcb.challenge,Msg_2.challenge,sizeof(Msg_2.challenge));
	memcpy(pcb.response2,Msg_3.lmresponse+8,8);
	memcpy(pcb.response1,Msg_3.lmresponse,8);

	CreateThread(NULL,0,BruteForcePwd1,&pcb,0,NULL);
}

void CNTLMCrackDlg::OnBnClickedButton4()
{
	m_EdtDisplayedPwd1.SetWindowTextA(pcb.CurrentPwd);
}
void CNTLMCrackDlg::OnTimer(UINT_PTR nIDEvent)
{
	static int last;
	m_EdtDisplayedCurrent.SetWindowTextA(pcb.CurrentPwd);
	CString str;
	str.AppendFormat("%d ops/sec",pcb.progress-last);
	m_EdtDisplayedSpeed.SetWindowTextA(str);
	last = pcb.progress;
	//theApp.InitApplication();
}

