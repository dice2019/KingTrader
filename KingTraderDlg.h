
// KingTraderDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "DBAccess.h"
#include "afxdtctl.h"
#include "afxcmn.h"

#define BUFFER_LEN 0x1000
#define SPEEDSHOW_INTERVAL_SECONDS 2

#define WM_MSG_KTDLG	WM_USER+1000

#define WM_MSG_WPARAM_STRMSG			1
#define WM_MSG_WPARAM_COUNT_DONE		2
#define WM_MSG_WPARAM_COUNT_ERROR   	3
#define WM_MSG_WPARAM_COUNT_ALL			4

#define WM_MSG_WPARAM_SPEED				5
#define WM_MSG_WPARAM_RESTOFTIME		6 //RestOfTime
#define WM_MSG_WPARAM_PROGRESS			7

#define WM_MSG_WPARAM_BTNENABLE			8

typedef struct _CDBAccessMarket {
	LPWSTR market;
	int type;
	int onesize;
	int dailycount;
	CDBAccess *dbAccess;
}CDBAccessMarket;

class CKingTraderDlg;

typedef struct _ThreadWorkInfo {
	CString code;
	int count;
	int marketIndex;

	CKingTraderDlg* kTraderDlg;
}ThreadWorkInfo;

typedef struct _MysqlWorkInfo {
	bool isDailyJob;
	bool isReplace;

	int kxCount;
	
	CKingTraderDlg* kTraderDlg;
}MysqlWorkInfo;

typedef UINT(__stdcall *WorkThreadFunc)(LPVOID);

// CKingTraderDlg 对话框
class CKingTraderDlg : public CDialogEx
{
// 构造
public:
	CKingTraderDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_KINGTRADER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	HANDLE m_hThreadHandle;
	UINT m_uThreadID;

	void LoadDll();
	void UnloadDll();
	bool InitialMarket();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg LRESULT OnInfoMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	void enableButtonGroup(bool enable);

	ThreadWorkInfo *CreateThreadWorkInfo();
	bool CreateWorkThread(WorkThreadFunc pFunc, LPVOID lParam);
	void TerminateWorkThread();

	bool SaveKline(LPWSTR market, LPWSTR code, int type, KLine &kline, LPWSTR method);
	static UINT ReadLastNItemThread(LPVOID lParam);
	static UINT GetStkListThread(LPVOID lParam);
	static UINT Save2MysqlThread(LPVOID lParam);

public:
	HMODULE DBApiHMODULE;
	CDBAccess_DBInitial DBInitial;
	CDBAccess_DBCleanUp DBCleanUp;
	WCHAR szPath[MAX_PATH]; //存放选择的目录路径 

	bool initDone;
	static bool DateTime2Number(CTime &cTime, DWORD &high, DWORD &low);
	static bool Number2DateTime(DWORD &high, DWORD &low, CTime &cTime);

	CEdit m_iopath;
	afx_msg void OnBnClickedBtnLoaddll();
	afx_msg void OnBnClickedBtnPathshow();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedBtnDt2num();
	CDateTimeCtrl m_dtDate;
	CDateTimeCtrl m_dtTime;
	CEdit m_dtNum;
	afx_msg void OnBnClickedBtnNum2dt();
	CEdit m_edtHigh;
	CEdit m_edtLow;
	afx_msg void OnBnClickedBtnNow();
	CEdit m_edtKlineMesg;
	CProgressCtrl m_pgSave;

	CComboBox m_marketBox;
	CComboBox m_periodBox;
	CEdit m_codeEdit;
	CEdit m_countEdit;
	afx_msg void OnBnClickedStklist();
	afx_msg void OnBnClickedSavemysql();
	CButton m_replaceCheck;
	CButton m_btnSave2Mysql;
	CButton m_readBtn;
	CButton m_stklistBtn;
	CButton m_iopathshowBtn;
	afx_msg void OnBnClickedExit();
	CEdit m_edtDoneCount;
	CEdit m_edtErrorCount;
	afx_msg void OnBnClickedRead();
	CEdit m_edtSpeed;
	CButton m_btnDailySave;
	afx_msg void OnBnClickedSavedaily();
	CEdit m_edtRestOfTime;
	CEdit m_edtTotalCount;
};
