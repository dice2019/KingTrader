
// KingTraderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KingTrader.h"
#include "KingTraderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CKingTraderDlg 对话框
static CDatabase dataBase;
static CDBAccessMarket gDBAccessMarket[] = {
	{ L"SH", KPERIOD_DAY, KLINE_SIZE, 5, NULL },
	{ L"SZ", KPERIOD_DAY, KLINE_SIZE, 5, NULL },

	{ L"SH", KPERIOD_MIN5, KLINE_SIZE, 100, NULL },
	{ L"SZ", KPERIOD_MIN5, KLINE_SIZE, 100, NULL },

	{ L"SH", KPERIOD_MIN1, KLINE_SIZE, 500, NULL },
	{ L"SZ", KPERIOD_MIN1, KLINE_SIZE, 500, NULL },
};

static CDBAccess_DelData DelData;
static CDBAccess_DataItemsCount DataItemsCount;
static CDBAccess_GetStgDbVersion GetStgDbVersion;
static CDBAccess_GetStgIndexCount GetStgIndexCount;

static CDBAccess_GetDBInstance3 GetDBInstance3;
static CDBAccess_GetDBInstance4 GetDBInstance4;
static CDBAccess_CheckOpen CheckOpen;

static CDBAccess_ReadDataByTime ReadDataByTime;
static CDBAccess_ReadLastDataItem ReadLastDataItem;
static CDBAccess_ReadLastNItem ReadLastNItem;
static CDBAccess_ReadFirstLastNItem ReadFirstLastNItem;

static CDBAccess_ReadDataByStkCode ReadDataByStkCode;
static CDBAccess_ReadVerAndSeq ReadVerAndSeq;

static CDBAccess_ReadDataCount ReadDataCount;

static CDBAccess_FindStgIndex FindStgIndex;

static CDBAccess_GetStkList GetStkList;

CKingTraderDlg::CKingTraderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKingTraderDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKingTraderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_IOPATH, m_iopath);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dtDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_dtTime);
	DDX_Control(pDX, IDC_EDIT_HIGH, m_edtHigh);
	DDX_Control(pDX, IDC_EDIT_LOW, m_edtLow);
	DDX_Control(pDX, IDC_EDIT_KLINE, m_edtKlineMesg);
	DDX_Control(pDX, IDC_PROGRESS1, m_pgSave);
	DDX_Control(pDX, IDC_COMBO_MARKET, m_marketBox);
	DDX_Control(pDX, IDC_COMBO_PERIOD, m_periodBox);
	DDX_Control(pDX, IDC_EDIT_CODE, m_codeEdit);
	DDX_Control(pDX, IDC_EDIT_COUNT, m_countEdit);
	DDX_Control(pDX, IDC_CHK_REPLACE, m_replaceCheck);
	DDX_Control(pDX, IDSAVEMYSQL, m_btnSave2Mysql);
	DDX_Control(pDX, IDREAD, m_readBtn);
	DDX_Control(pDX, IDSTKLIST, m_stklistBtn);
	DDX_Control(pDX, IDC_BTN_PATHSHOW, m_iopathshowBtn);
	DDX_Control(pDX, IDC_EDIT_TOTALCOUNT, m_edtTotalCount);
	DDX_Control(pDX, IDC_EDIT_DONECOUNT, m_edtDoneCount);
	DDX_Control(pDX, IDC_EDIT_ERRORCOUNT, m_edtErrorCount);
	DDX_Control(pDX, IDC_EDIT_SPEED, m_edtSpeed);
	DDX_Control(pDX, IDSAVEDAILY, m_btnDailySave);
	DDX_Control(pDX, IDC_EDIT_RESTOFTIME, m_edtRestOfTime);
}

BEGIN_MESSAGE_MAP(CKingTraderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_PATHSHOW, &CKingTraderDlg::OnBnClickedBtnPathshow)
	ON_BN_CLICKED(IDC_BTN_DT2NUM, &CKingTraderDlg::OnBnClickedBtnDt2num)
	ON_BN_CLICKED(IDC_BTN_NUM2DT, &CKingTraderDlg::OnBnClickedBtnNum2dt)
	ON_BN_CLICKED(IDC_BTN_NOW, &CKingTraderDlg::OnBnClickedBtnNow)
	ON_BN_CLICKED(IDSTKLIST, &CKingTraderDlg::OnBnClickedStklist)
	ON_BN_CLICKED(IDSAVEMYSQL, &CKingTraderDlg::OnBnClickedSavemysql)
	ON_BN_CLICKED(IDEXIT, &CKingTraderDlg::OnBnClickedExit)
	ON_BN_CLICKED(IDREAD, &CKingTraderDlg::OnBnClickedRead)
	ON_BN_CLICKED(IDSAVEDAILY, &CKingTraderDlg::OnBnClickedSavedaily)
END_MESSAGE_MAP()

void CKingTraderDlg::UnloadDll()
{
	if (DBCleanUp) {
		DBCleanUp();
	}

	if (DBApiHMODULE) {
		FreeLibrary(DBApiHMODULE);
		DBApiHMODULE = NULL;
	}

	DBInitial = NULL;
	DBCleanUp = NULL;
	GetStgDbVersion = NULL;
	GetStgIndexCount = NULL;
	DataItemsCount = NULL;
	DelData = NULL;
	GetDBInstance4 = NULL;
	GetDBInstance3 = NULL;

	CheckOpen = NULL;
	ReadDataByTime = NULL;
	ReadLastDataItem = NULL;
	ReadLastNItem = NULL;
	ReadFirstLastNItem = NULL;
	ReadDataByStkCode = NULL;

	ReadVerAndSeq = NULL;
	ReadDataCount = NULL;
	FindStgIndex = NULL;
	GetStkList = NULL;

	for (int idx = 0; idx < sizeof(gDBAccessMarket) / sizeof(CDBAccessMarket); idx++) {
		gDBAccessMarket[idx].dbAccess = NULL;
	}
}

void CKingTraderDlg::LoadDll()
{
	if (DBApiHMODULE) {
		return;
	}

	DBApiHMODULE = LoadLibrary(L"StorageAccess_2_0.dll");

	DBInitial = (CDBAccess_DBInitial)GetProcAddress(DBApiHMODULE, "?DBInitial@CDBAccess@@SA_NPA_W@Z");
	if (DBInitial == NULL) {
		AfxMessageBox(L"加载DBInitial错误!");
		return;
	}

	DBCleanUp = (CDBAccess_DBCleanUp)GetProcAddress(DBApiHMODULE, "?DBCleanUp@CDBAccess@@SAXXZ");
	if (DBCleanUp == NULL) {
		AfxMessageBox(L"加载DBCleanUp错误!");
		return;
	}

	GetStgDbVersion = (CDBAccess_GetStgDbVersion)GetProcAddress(DBApiHMODULE, "?GetStgDbVersion@CDBAccess@@QAEHXZ");
	if (GetStgDbVersion == NULL) {
		AfxMessageBox(L"加载GetStgDbVersion错误!");
		return;
	}

	GetStgIndexCount = (CDBAccess_GetStgIndexCount)GetProcAddress(DBApiHMODULE, "?GetStgIndexCount@CDBAccess@@QAEHXZ");
	if (GetStgDbVersion == NULL) {
		AfxMessageBox(L"加载GetStgIndexCount错误!");
		return;
	}

	DataItemsCount = (CDBAccess_DataItemsCount)GetProcAddress(DBApiHMODULE, "?DataItemsCount@CDBAccess@@QAEHXZ");
	if (DataItemsCount == NULL) {
		AfxMessageBox(L"加载DataItemsCount错误!");
		return;
	}

	DelData = (CDBAccess_DelData)GetProcAddress(DBApiHMODULE, "?DelData@CDBAccess@@QAEXPBD@Z");
	if (DelData == NULL) {
		AfxMessageBox(L"加载DelData错误!");
		return;
	}

	GetDBInstance4 = (CDBAccess_GetDBInstance4)GetProcAddress(DBApiHMODULE, "?GetDBInstance@CDBAccess@@SAPAV1@PA_WHHHPAV?$basic_string@_WU?$char_traits@_W@std@@V?$allocator@_W@2@@std@@@Z");
	if (GetDBInstance4 == NULL) {
		AfxMessageBox(L"加载GetDBInstance4错误!");
		return;
	}

	GetDBInstance3 = (CDBAccess_GetDBInstance3)GetProcAddress(DBApiHMODULE, "?GetDBInstance@CDBAccess@@SAPAV1@PA_WHHPAV?$basic_string@_WU?$char_traits@_W@std@@V?$allocator@_W@2@@std@@@Z");
	if (GetDBInstance3 == NULL) {
		AfxMessageBox(L"加载GetDBInstance3错误!");
		return;
	}

	CheckOpen = (CDBAccess_CheckOpen)GetProcAddress(DBApiHMODULE, "?CheckOpen@CDBAccess@@QAE_NPAV?$basic_string@_WU?$char_traits@_W@std@@V?$allocator@_W@2@@std@@@Z");
	if (CheckOpen == NULL) {
		AfxMessageBox(L"加载CheckOpen错误!");
		return;
	}

	ReadDataByTime = (CDBAccess_ReadDataByTime)GetProcAddress(DBApiHMODULE, "?ReadDataByTime@CDBAccess@@QAE_NPBDAA_J1AAI1AAGPAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z");
	if (ReadDataByTime == NULL) {
		AfxMessageBox(L"加载ReadDataByTime错误!");
		return;
	}

	ReadLastDataItem = (CDBAccess_ReadLastDataItem)GetProcAddress(DBApiHMODULE, "?ReadLastDataItem@CDBAccess@@QAEXPBDPAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z");
	if (ReadLastDataItem == NULL) {
		AfxMessageBox(L"加载ReadLastDataItem错误!");
		return;
	}

	ReadLastNItem = (CDBAccess_ReadLastNItem)GetProcAddress(DBApiHMODULE, "?ReadLastNItem@CDBAccess@@QAE_NPBDHAAIAA_JAAGPAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@_J@Z");
	if (ReadLastNItem == NULL) {
		AfxMessageBox(L"加载ReadLastNItem错误!");
		return;
	}


	ReadFirstLastNItem = (CDBAccess_ReadFirstLastNItem)GetProcAddress(DBApiHMODULE, "?ReadFirstLastNItem@CDBAccess@@IAE_NPBDHHAAIAA_JAAGPAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@_J@Z");
	if (ReadFirstLastNItem == NULL) {
		AfxMessageBox(L"加载ReadFirstLastNItem错误!");
		return;
	}

	ReadDataByStkCode = (CDBAccess_ReadDataByStkCode)GetProcAddress(DBApiHMODULE, "?ReadDataByStkCode@CDBAccess@@QAE_NPBDAAIAA_JAAGPAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z");
	if (ReadDataByStkCode == NULL) {
		AfxMessageBox(L"加载ReadDataByStkCode错误!");
		return;
	}

	ReadVerAndSeq = (CDBAccess_ReadVerAndSeq)GetProcAddress(DBApiHMODULE, "?ReadVerAndSeq@CDBAccess@@QAEXPBDPAIPA_J@Z");
	if (ReadVerAndSeq == NULL) {
		AfxMessageBox(L"加载ReadVerAndSeq错误!");
		return;
	}

	ReadDataCount = (CDBAccess_ReadDataCount)GetProcAddress(DBApiHMODULE, "?ReadDataCount@CDBAccess@@QAEHPBD@Z");
	if (ReadDataCount == NULL) {
		AfxMessageBox(L"加载ReadDataCount错误!");
		return;
	}

	FindStgIndex = (CDBAccess_FindStgIndex)GetProcAddress(DBApiHMODULE, "?FindStgIndex@CDBAccess@@IAEHAAU__kmam_STG_INDEX@@ABU__kama_SECURITY_IDENTIFY@@@Z");
	if (FindStgIndex == NULL) {
		AfxMessageBox(L"加载FindStgIndex错误!");
		return;
	}

	GetStkList = (CDBAccess_GetStkList)GetProcAddress(DBApiHMODULE, "?GetStkList@CDBAccess@@QAEHPADH@Z");
	if (GetStkList == NULL) {
		AfxMessageBox(L"加载GetStkList错误!");
		return;
	}
}

// CKingTraderDlg 消息处理程序

BOOL CKingTraderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ZeroMemory(szPath, sizeof(szPath));

	DBCleanUp = NULL;
	DBApiHMODULE = NULL;
	DBInitial = NULL;
	GetDBInstance3 = NULL;
	GetDBInstance4 = NULL;

	CheckOpen = NULL;
	ReadDataByTime = NULL;
	ReadLastDataItem = NULL;
	ReadLastNItem = NULL;

	ReadFirstLastNItem = NULL;
	ReadDataByStkCode = NULL;
	ReadVerAndSeq = NULL;

	ReadDataCount = NULL;

	wsprintf(szPath, L"%s", L"E:\\KT\\io");
	m_iopath.SetWindowText(szPath);

	initDone = InitialMarket();
	if (initDone != true) {
		AfxMessageBox(L"无效的目录，请重新选择");
		wsprintf(szPath, L"%s", L"C:\\");
	}
	else {
		AfxMessageBox(L"加载io目录完毕!");
	}

	CTime now = CTime::GetCurrentTime();
	CTime m_cTime = CTime(now.GetYear(), now.GetMonth(), now.GetDay(), 9, 30, 0);
	m_dtDate.SetTime(&m_cTime);
	m_dtTime.SetTime(&m_cTime);

	m_marketBox.ResetContent();
	m_marketBox.InsertString(0, L"上海");
	m_marketBox.InsertString(1, L"深圳");
	m_marketBox.SetCurSel(0);

	m_codeEdit.SetWindowText(L"000001");
	m_countEdit.SetWindowText(L"600");
	
	m_edtTotalCount.SetWindowText(L"0");
	m_edtDoneCount.SetWindowText(L"0");
	m_edtErrorCount.SetWindowText(L"0");

	m_edtSpeed.SetWindowText(L"0");
	m_edtRestOfTime.SetWindowText(L"00:00:00");

	m_periodBox.ResetContent();
	m_periodBox.InsertString(0, L"分笔");
	m_periodBox.InsertString(1, L"1分钟");
	m_periodBox.InsertString(2, L"5分钟");
	m_periodBox.InsertString(3, L"日线");
	m_periodBox.SetCurSel(3);

	CLog::Instance().SetLogFilePath(L"KingTrader.log", true);
	CLog::Instance().SetLogLevel(LOG_INFO);

	m_pgSave.SetRange(0, 100);
	m_pgSave.SetPos(0);

	enableButtonGroup(true);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CKingTraderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CKingTraderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
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
		CDialogEx::OnPaint();
	}
}

void CKingTraderDlg::enableButtonGroup(bool enable)
{
	m_btnSave2Mysql.EnableWindow(enable);
	m_replaceCheck.EnableWindow(enable);
	m_codeEdit.EnableWindow(enable);
	m_countEdit.EnableWindow(enable);
	m_periodBox.EnableWindow(enable);
	m_marketBox.EnableWindow(enable);
	m_stklistBtn.EnableWindow(enable);
	m_iopathshowBtn.EnableWindow(enable);
	m_readBtn.EnableWindow(enable);
	m_iopath.EnableWindow(enable);
	m_btnDailySave.EnableWindow(enable);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CKingTraderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CKingTraderDlg::OnBnClickedBtnPathshow()
{
	CString str;

	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = L"请选择需要输出的目录：";
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);

	if (lp && SHGetPathFromIDList(lp, szPath))
	{
		initDone = InitialMarket();
		if (initDone != true) {
			AfxMessageBox(L"无效的目录，请重新选择");
			wsprintf(szPath, L"%s", L"C:\\");
		}

		m_iopath.SetWindowText(szPath);
	}
	else {
		AfxMessageBox(L"无效的目录，请重新选择");
	}
}

void CKingTraderDlg::OnBnClickedExit()
{
	if (m_hThreadHandle != NULL)
	{
		TerminateWorkThread();
	}
	else {
		dataBase.Close();
		CDialogEx::OnCancel();
	}
}

bool CKingTraderDlg::InitialMarket()
{
	CDBAccessBuf buffer;
	ZeroMemory(&buffer, sizeof(CDBAccessBuf));

	//UnloadDll();
	LoadDll();

	bool ret = DBInitial(szPath);
	if (ret) {
		for (int idx = 0; idx < sizeof(gDBAccessMarket) / sizeof(CDBAccessMarket); idx++) {
			gDBAccessMarket[idx].dbAccess = GetDBInstance4(gDBAccessMarket[idx].market,
				gDBAccessMarket[idx].type, gDBAccessMarket[idx].onesize, 0x7B, &buffer);
		}
	}

	return ret;
}

#define NUMTH ((ULONGLONG)1000000 * (ULONGLONG)86400)

bool CKingTraderDlg::DateTime2Number(CTime &cTime, DWORD &high, DWORD &low)
{
	DWORD d2num = 0;
	ULONGLONG date = 0;
	ULONGLONG time = 0;

	DWORD var1 = (14 - cTime.GetMonth()) / 12;
	DWORD var2 = cTime.GetYear() + 4800 - var1;
	DWORD var3 = cTime.GetMonth() + 12 * var1 - 3;
	d2num = (var2 >> 2) + 365 * var2 + (153 * var3 + 2) / 5
		+ cTime.GetDay() - var2 / 100 + var2 / 400 - 32045;

	ULONGLONG var5 = NUMTH;
	date = var5 * d2num;

	time = cTime.GetHour() * (ULONGLONG)3600 + cTime.GetMinute() * (ULONGLONG)60 + cTime.GetSecond();
	time *= (ULONGLONG)1000000;
	date += time;

	high = date >> 32;
	low = date & 0xFFFFFFFF;
	return true;
}

bool CKingTraderDlg::Number2DateTime(DWORD &high, DWORD &low, CTime &cTime)
{
	bool ret = true;
	ULONGLONG date = 0;
	ULONGLONG time = 0;

	date = high;
	date <<= 32;
	date += low;
	time = date % NUMTH;
	DWORD var0 = (DWORD)(date / NUMTH);

	DWORD var1 = var0 + 32044;
	DWORD var2 = (4 * var1 + 3) / 0x23AB1u;
	DWORD var3 = var1 - ((146097 * var2) >> 2);
	DWORD var4 = (4 * var3 + 3) / 0x5B5u;
	DWORD var5 = var3 - ((1461 * var4) >> 2);
	DWORD var6 = (5 * var5 + 2) / 0x99u;
	DWORD day = var5 - (153 * var6 + 2) / 5u + 1;
	DWORD month = var6 + 3 - 12 * (var6 / 0xAu);
	DWORD year = var4 + 100 * var2 + var6 / 0xAu - 4800;

	ULONGLONG seconds = time / (ULONGLONG)1000000;
	ULONGLONG hour = seconds / (ULONGLONG)3600;
	seconds = seconds % (ULONGLONG)3600;
	ULONGLONG minute = seconds / (ULONGLONG)60;
	ULONGLONG second = seconds % (ULONGLONG)60;

	CString strTime;
	if (year < 1800 || year > 2300) {
		strTime.Format(L"[%08X %08X] year %d error", high, low, year);
		ret = false;
	}
	else if (month < 1 || month > 12) {
		strTime.Format(L"[%08X %08X] month %d error", high, low, month);
		ret = false;
	}
	else if (day < 1 || day > 31) {
		strTime.Format(L"[%08X %08X] day %d error", high, low, day);
		ret = false;
	}
	else if (hour > 23 || minute > 59 || second > 59) {
		strTime.Format(L"[%08X %08X] time %d:%d:%d error", high, low, hour, minute, second);
		ret = false;
	}

	if (ret == false) {
		//m_edtKlineInfo.SetWindowText(strTime);
		CLog::Instance().Log(LOG_ERROR, L"%s", strTime);
	}
	else {
		cTime = CTime(year, month, day, (int)hour, (int)minute, (int)second);
	}

	return ret;
}

#define SQLCMD_BUFLEN 600
#define SQLCMD_REPLACE L"replace"
#define SQLCMD_INSERTIGNORE L"insert ignore"
bool CKingTraderDlg::SaveKline(LPWSTR market, LPWSTR code, int type, KLine &kline, LPWSTR method)
{
	bool ret = true;
	CTime cTime;
	CString strTime;
	CString strKline;
	TCHAR sqlcmd[SQLCMD_BUFLEN];

	if (!dataBase.IsOpen())//判断数据库是否已经被打开
	{
		try {
			BOOL bflag = dataBase.Open(L"miniMysql");
			if (!bflag)
			{
				m_edtKlineMesg.SetWindowText(L"打开数据库错误!");
				return false;
			}
		}
		catch (CDBException* e)
		{
			AfxMessageBox(e->m_strError);
			e->Delete();
			return false;
		}
	}

	if (Number2DateTime(kline.time_high, kline.time_low, cTime)) {
		LPWSTR kdur = L"none";
		if (type == KPERIOD_MIN1) {
			kdur = L"min1";
			cTime += CTimeSpan(0, 0, 1, 0);
		}
		else if (type == KPERIOD_MIN5) {
			kdur = L"min5";
			cTime += CTimeSpan(0, 0, 5, 0);
		}
		else if (type == KPERIOD_DAY) {
			kdur = L"day";
		}
		else {
			CLog::Instance().Log(LOG_INFO, L"Unkown type 0x%02X.", type);
			m_edtKlineMesg.SetWindowText(L"未知周期!");
			return false;
		}

		strTime = cTime.Format(L"%Y-%m-%d %H:%M:%S");
		strKline.Format(L"%s%s %s %s %.02f, %.02f, %.02f, %.02f, %.02f, %.02f", market, code, kdur, strTime,
			kline.open, kline.high, kline.low, kline.close, kline.vol, kline.amount);
		CLog::Instance().Log(LOG_DEBUG, L"%s", strKline);
		m_edtKlineMesg.SetWindowText(strKline);

		if (cTime > CTime::GetCurrentTime()) {
			CLog::Instance().Log(LOG_ERROR, L"timestamp faster than now (%s)", strTime);
			return false;
		}

		//KingTrader存在13:00时间点的数据
#if 0
		if (cTime.GetHour() == 13 && cTime.GetMinute() == 00) {
			CLog::Instance().Log(LOG_ERROR, L"invalid trade timestamp (%s)", strTime);
			return false;
		}
#endif

		//国债期货就可能出现成交为0的时间点
#if 0
		if (kline.open == kline.high && kline.open == kline.low
			&& kline.open == kline.close && kline.vol == 0 && kline.amount == 0) {
			CLog::Instance().Log(LOG_ERROR, L"invalid kline price (%s)", strKline);
			return false;
		}
#endif

		if (kline.open <= 0 || kline.high <= 0 || kline.low <= 0 || kline.close <= 0) {
			CLog::Instance().Log(LOG_ERROR, L"invalid kline price (%s)", strKline);
			return false;
		}

		if (kline.vol == 0 || kline.amount == 0) {
			//CLog::Instance().Log(LOG_ERROR, L"%s", strKline);
			CLog::Instance().Log(LOG_INFO, L"zero vol or amount (%s)", strKline);
			//continue;
		}

		CString stid;
		stid.Format(L"%s%s", market, code);
		ZeroMemory(&sqlcmd, SQLCMD_BUFLEN);
		swprintf_s(sqlcmd, SQLCMD_BUFLEN,
			L"%s into Tk%s set stid=\"%s\", date=\"%s\", open=%.02f, high=%.02f, low=%.02f, close=%.02f, vol=%.02f, amount=%.02f, source=\"%s\";",
			method, kdur, stid, strTime, kline.open, kline.high, kline.low, kline.close, kline.vol, kline.amount, L"KTReader");
		CLog::Instance().Log(LOG_DEBUG, L"Sql[%s]", sqlcmd);

		try
		{
			dataBase.ExecuteSQL(sqlcmd);
		}
		catch (...)
		{
			strKline.Format(L"保存数据错误 %s", sqlcmd);
			m_edtKlineMesg.SetWindowText(strKline);
			ret = false;
		}
	}
	else {
		strKline.Format(L"数据解析错误!%d");
		m_edtKlineMesg.SetWindowText(strKline);
		ret = false;
	}

	return ret;
}

void CKingTraderDlg::OnBnClickedBtnDt2num()
{
	CTime cTime;
	m_dtDate.GetTime(cTime);
	m_dtTime.GetTime(cTime);


	DWORD high = 0;
	DWORD low = 0;
	DateTime2Number(cTime, high, low);

	CString str;
	str.Format(L"%08X", high);
	m_edtHigh.SetWindowText(str);

	str.Format(L"%08X", low);
	m_edtLow.SetWindowText(str);
	UpdateData(FALSE);
}

void CKingTraderDlg::OnBnClickedBtnNum2dt()
{
	CString str;

	UpdateData(TRUE);
	m_edtHigh.GetWindowText(str);
	//DWORD high = _tcstoul(str, NULL, 16);
	DWORD high = wcstoul(str, NULL, 16);

	m_edtLow.GetWindowText(str);
	DWORD low = wcstoul(str, NULL, 16);


	CTime cTime = CTime::GetCurrentTime();
	Number2DateTime(high, low, cTime);
	m_dtDate.SetTime(&cTime);
	m_dtTime.SetTime(&cTime);
	UpdateData(FALSE);
}

void CKingTraderDlg::OnBnClickedBtnNow()
{
	CTime cTime = CTime::GetCurrentTime();
	DWORD high = 0;
	DWORD low = 0;
	DateTime2Number(cTime, high, low);

	CString str;
	str.Format(L"%08X", high);
	m_edtHigh.SetWindowText(str);

	str.Format(L"%08X", low);
	m_edtLow.SetWindowText(str);

	m_dtDate.SetTime(&cTime);
	m_dtTime.SetTime(&cTime);

	UpdateData(FALSE);
}

//将TCHAR转为char   
//*tchar是TCHAR类型指针，*_char是char类型指针   
int TcharToChar(const TCHAR * tchar, char * _char)
{
	int iLength = 0;
	//获取字节长度   
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
	//将tchar值赋给_char    
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
	return iLength;
}

int CharToTchar(const char * _char, TCHAR * tchar)
{
	int iLength = 0;
	iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength);
	return iLength;
}

bool CKingTraderDlg::CreateWorkThread(WorkThreadFunc pFunc, LPVOID lParam)
{
	m_hThreadHandle = (HANDLE)::_beginthreadex(NULL, 0, pFunc, lParam, 0, &m_uThreadID);
	if (m_hThreadHandle) {
		return true;
	}
	else {
		return false;
	}
}

#define WAIT_WORKTHREAD_TIMEOUT (3*1000)
void CKingTraderDlg::TerminateWorkThread(void) {
	//WaitForSingleObject(m_hThreadHandle, INFINITE);
	DWORD dwRetCode = WaitForSingleObject(m_hThreadHandle, WAIT_WORKTHREAD_TIMEOUT);
	if (dwRetCode == WAIT_TIMEOUT) {
		DWORD dwExitCode = 0;
		CLog::Instance().Log(LOG_ERROR, L"work thread exit timeout, terminate !");
		::TerminateThread(m_hThreadHandle, dwExitCode);
	}

	//设置变量
	if (m_hThreadHandle != NULL)
	{
		CloseHandle(m_hThreadHandle);
		m_hThreadHandle = NULL;
		m_uThreadID = 0;
	}

	enableButtonGroup(true);
}

ThreadWorkInfo *CKingTraderDlg::CreateThreadWorkInfo()
{
	if (!initDone) {
		AfxMessageBox(L"请先设置正确的io目录!");
		return NULL;
	}

	m_pgSave.SetPos(0);

	CString market = L"SH";
	if (m_marketBox.GetCurSel() == 1) {
		market = "SZ";
	}

	int type = m_periodBox.GetCurSel();
	if (type == 0) {
		type = KPERIOD_TICK; //tick
	}
	else if (type == 1) {
		type = KPERIOD_MIN1; //min1
	}
	else if (type == 2) {
		type = KPERIOD_MIN5; //min5
	}
	else {
		type = KPERIOD_DAY; //day
	}

	ThreadWorkInfo *threaWorkInfo = new ThreadWorkInfo;
	threaWorkInfo->marketIndex = -1;
	for (int idx = 0; idx < sizeof(gDBAccessMarket) / sizeof(CDBAccessMarket); idx++) {
		if (wcscmp(market, gDBAccessMarket[idx].market) == 0 && gDBAccessMarket[idx].type == type) {
			threaWorkInfo->marketIndex = idx;
			break;
		}
	}

	if (threaWorkInfo->marketIndex < 0) {
		AfxMessageBox(L"没有合适的访问句柄!");
		return NULL;
	}

	m_codeEdit.GetWindowText(threaWorkInfo->code);

	CString strCount;
	m_countEdit.GetWindowText(strCount);
	threaWorkInfo->count = _wtoi(strCount);

	threaWorkInfo->kTraderDlg = this;

	return threaWorkInfo;
}

void CKingTraderDlg::OnBnClickedStklist()
{
	ThreadWorkInfo *threaWorkInfo = CreateThreadWorkInfo();
	if (threaWorkInfo) {
		//begin upload
		CreateWorkThread((WorkThreadFunc)CKingTraderDlg::GetStkListThread, (LPVOID)threaWorkInfo);
	}

	UpdateData(FALSE);
}

void CKingTraderDlg::OnBnClickedRead()
{
	ThreadWorkInfo *threaWorkInfo = CreateThreadWorkInfo();
	if (threaWorkInfo) {
		//begin upload
		CreateWorkThread((WorkThreadFunc)CKingTraderDlg::ReadLastNItemThread, (LPVOID)threaWorkInfo);
	}

	UpdateData(FALSE);
}

void CKingTraderDlg::OnBnClickedSavemysql()
{
	if (!initDone) {
		AfxMessageBox(L"请先设置正确的io目录!");
		return;
	}

	m_pgSave.SetPos(0);
	MysqlWorkInfo *mysqlWorkInfo = new MysqlWorkInfo;
	mysqlWorkInfo->kTraderDlg = this;

	CString strCount;
	m_countEdit.GetWindowText(strCount);
	mysqlWorkInfo->kxCount = _wtoi(strCount);

	mysqlWorkInfo->isDailyJob = false;
	mysqlWorkInfo->isReplace = false;
	if (m_replaceCheck.GetCheck() == 1) {
		mysqlWorkInfo->isReplace = true;
	}

	CreateWorkThread((WorkThreadFunc)CKingTraderDlg::Save2MysqlThread, (LPVOID)mysqlWorkInfo);
	UpdateData(FALSE);
}

void CKingTraderDlg::OnBnClickedSavedaily()
{
	if (!initDone) {
		AfxMessageBox(L"请先设置正确的io目录!");
		return;
	}

	m_pgSave.SetPos(0);
	MysqlWorkInfo *mysqlWorkInfo = new MysqlWorkInfo;
	mysqlWorkInfo->kTraderDlg = this;

	mysqlWorkInfo->isDailyJob = true;
	mysqlWorkInfo->kxCount = 0;
	mysqlWorkInfo->isReplace = true;
	CreateWorkThread((WorkThreadFunc)CKingTraderDlg::Save2MysqlThread, (LPVOID)mysqlWorkInfo);
	UpdateData(FALSE);
}

UINT CKingTraderDlg::GetStkListThread(LPVOID lParam)
{
	bool ret = false;
	int count = 0;

	CDBAccessBuf buffer;
	ZeroMemory(&buffer, sizeof(CDBAccessBuf));

	ThreadWorkInfo *threaWorkInfo = (ThreadWorkInfo *)lParam;
	threaWorkInfo->kTraderDlg->enableButtonGroup(false);

	ret = CheckOpen(gDBAccessMarket[threaWorkInfo->marketIndex].dbAccess, &buffer);

	//count = GetStgDbVersion(gDBAccessMarket[threaWorkInfo->marketIndex].dbAccess);
	//count = GetStgIndexCount(gDBAccessMarket[threaWorkInfo->marketIndex].dbAccess);

	stock *pstockList = new stock[MAX_STOCK_NUMBER];
	count = GetStkList(gDBAccessMarket[threaWorkInfo->marketIndex].dbAccess, pstockList, sizeof(stock) * MAX_STOCK_NUMBER);


	CString strInfo = L"";
	strInfo.Format(L"获取股票数量:%d", count);
	threaWorkInfo->kTraderDlg->m_edtKlineMesg.SetWindowText(strInfo);

	delete[] pstockList;
	threaWorkInfo->kTraderDlg->enableButtonGroup(true);
	return 0L;
}

UINT CKingTraderDlg::ReadLastNItemThread(LPVOID lParam)
{
	KLine *pKbuf = NULL;
	bool ret = false;
	int idx = 0;
	DWORD a0, a1, a2, a3;
	a0 = a1 = a2 = a3 = 0;
	unsigned int count = 0;
	unsigned  int ui1 = 0;
	__int64 ll = 0;
	unsigned short us1 = 0;

	kmam_STG_INDEX stgidx;
	kama_SECURITY_IDENTIFY secidx;

	CDBAccessBuf buffer;
	ZeroMemory(&buffer, sizeof(CDBAccessBuf));
	ZeroMemory(&stgidx, sizeof(kmam_STG_INDEX));
	ZeroMemory(&secidx, sizeof(kama_SECURITY_IDENTIFY));

	ThreadWorkInfo *threaWorkInfo = (ThreadWorkInfo *)lParam;
	threaWorkInfo->kTraderDlg->enableButtonGroup(false);

	ret = CheckOpen(gDBAccessMarket[threaWorkInfo->marketIndex].dbAccess, &buffer);

	char acode[10];
	TcharToChar(threaWorkInfo->code.GetBuffer(), acode); //"300044"
	acode[9] = '\0';

	if (threaWorkInfo->count <= 0) {
		count = ReadDataCount(gDBAccessMarket[threaWorkInfo->marketIndex].dbAccess, acode);
	}
	else {
		count = threaWorkInfo->count;
	}

	//memcpy_s(&secidx, 0x18u, acode, 7);
	//ret = FindStgIndex(pDBAccessMarke->dbAccess, &stgidx, &secidx);
	//ret = ReadDataByStkCode(pDBAccessMarke->dbAccess, acode, &buffer.reserve[0], &buffer.reserve[1], &buffer.reserve[10], &buffer.reserve[30]);
	//ret = ReadLastDataItem(pDBAccessMarke->dbAccess, acode, (DWORD *)&buffer);

	if (count < 1) {
		threaWorkInfo->kTraderDlg->m_edtKlineMesg.SetWindowText(L"没有数据可以读取!");
		threaWorkInfo->kTraderDlg->enableButtonGroup(true);
		return 2L;
	}

	pKbuf = new KLine[count + 1];
	buffer.KBuffer = pKbuf;
	buffer.a1 = 00;
	buffer.a2 = 00;
	buffer.a3 = 00;
	buffer.Count = 0x00;
	buffer.Length = count * sizeof(KLine);

	ret = ReadLastNItem(gDBAccessMarket[threaWorkInfo->marketIndex].dbAccess, acode, count, &ui1, &ll, &us1, (DWORD *)&buffer, 0);
	count = buffer.Count / sizeof(KLine);
	CLog::Instance().Log(LOG_INFO, L"ReadLastNItem market:%s code:%s, type:%d, count:%d", gDBAccessMarket[threaWorkInfo->marketIndex].market, threaWorkInfo->code,
		gDBAccessMarket[threaWorkInfo->marketIndex].type, count);
	if (us1 != sizeof(KLine)) {
		threaWorkInfo->kTraderDlg->m_edtKlineMesg.SetWindowText(L"KLine长度错误!");
		delete[] pKbuf;
		threaWorkInfo->kTraderDlg->enableButtonGroup(true);
		return 4L;
	}

	int error = 0;
	int progress = 0;
	int totalKxCount;
	int doneKxCount = 0;
	int lastKxCount = 0;
	int lastProgress = 0;
	CTime startTask, lastTask;

	CTime cTime = CTime::GetCurrentTime();
	CString strCount = L"0";
	CString strKline = L"";

	totalKxCount = buffer.Count / sizeof(KLine);
	strKline.Format(L"%d", totalKxCount);
	threaWorkInfo->kTraderDlg->m_edtTotalCount.SetWindowText(strKline);

	for (idx = 0; idx < totalKxCount; idx++) {
		if (Number2DateTime(pKbuf[idx].time_high, pKbuf[idx].time_low, cTime)) {
			strKline.Format(L"[%s] %.02f, %.02f, %.02f, %.02f, %.02f, %.02f", cTime.Format(_T("%Y-%m-%d %H:%M:%S")),
				pKbuf[idx].open, pKbuf[idx].high, pKbuf[idx].low, pKbuf[idx].close, pKbuf[idx].vol, pKbuf[idx].amount);
			CLog::Instance().Log(LOG_DEBUG, L"%s", strKline);
			if (!threaWorkInfo->kTraderDlg->SaveKline(gDBAccessMarket[threaWorkInfo->marketIndex].market,
				threaWorkInfo->code.GetBuffer(), gDBAccessMarket[threaWorkInfo->marketIndex].type, pKbuf[idx], SQLCMD_REPLACE)) {
				error++;
				strCount.Format(L"%d", error);
				threaWorkInfo->kTraderDlg->m_edtErrorCount.SetWindowText(strCount);
			}
		}
		else {
			strKline.Format(L"数据解析错误!%d", idx);
			threaWorkInfo->kTraderDlg->m_edtKlineMesg.SetWindowText(strKline);
			error++;
		}

		doneKxCount = idx + 1;

		strCount.Format(L"%d", doneKxCount);
		threaWorkInfo->kTraderDlg->m_edtDoneCount.SetWindowText(strCount);

		progress = 100 * doneKxCount / totalKxCount;
		threaWorkInfo->kTraderDlg->m_pgSave.SetPos(progress);

		CTime tmNow = CTime::GetCurrentTime();
		int diffSec = (int)(tmNow.GetTime() - lastTask.GetTime());
		if (diffSec > SPEEDSHOW_INTERVAL_SECONDS && doneKxCount < totalKxCount && (doneKxCount - lastKxCount) > 60) {
			int taskSpeed = (doneKxCount - lastKxCount) / diffSec;
			CString strSpeed = L"0";
			strSpeed.Format(L"%d", taskSpeed);
			threaWorkInfo->kTraderDlg->m_edtSpeed.SetWindowText(strSpeed);

			if (taskSpeed > 1) {
				int restoftime = (int)(totalKxCount - doneKxCount) / taskSpeed;
				int rot_hour = restoftime / 3600;
				int rot_minute = (restoftime % 3600) / 60;
				int rot_second = restoftime % 60;
				CString strRestOfTime = L"00:00:00";
				strRestOfTime.Format(L"%02d:%02d:%02d", rot_hour, rot_minute, rot_second);
				threaWorkInfo->kTraderDlg->m_edtRestOfTime.SetWindowText(strRestOfTime);
			}

			lastTask = tmNow;
			lastKxCount = doneKxCount;
		}
	}

	threaWorkInfo->kTraderDlg->m_pgSave.SetPos(100);
	CLog::Instance().Log(LOG_INFO, L"Read market:%s code:%s, type:%d, count:%d(error %d).", gDBAccessMarket[threaWorkInfo->marketIndex].market,
		threaWorkInfo->code, gDBAccessMarket[threaWorkInfo->marketIndex].type, idx, error);

	strKline.Format(L"总共保存 成功:%d, 错误:%d", idx - error, error);
	threaWorkInfo->kTraderDlg->m_edtKlineMesg.SetWindowText(strKline);

	if (pKbuf) {
		delete[] pKbuf;
	}

	buffer.KBuffer = NULL;
	threaWorkInfo->kTraderDlg->m_edtRestOfTime.SetWindowText(L"00:00:00");
	//ReadVerAndSeq(gDBAccessMarket[threaWorkInfo->marketIndex].dbAccess, acode, &ui1, &ll);
	threaWorkInfo->kTraderDlg->enableButtonGroup(true);
	return 0L;
}

UINT CKingTraderDlg::Save2MysqlThread(LPVOID lParam)
{
	unsigned  int ui1 = 0;
	__int64 ll = 0;
	unsigned short us1 = 0;
	CString strCount = L"0";
	KLine *pKbuf = NULL;
	CDBAccessBuf buffer;
	ZeroMemory(&buffer, sizeof(CDBAccessBuf));

	MysqlWorkInfo *mysqlWorkInfo = (MysqlWorkInfo *)lParam;
	mysqlWorkInfo->kTraderDlg->enableButtonGroup(false);

	LPWSTR method = SQLCMD_REPLACE;

	int error = 0;
	int stkCount = 0;
	int kxCount = 0;

	int doneKxCount = 0;
	int lastKxCount = 0;
	int totalKxCount = 0;

	int lastProgress = 0;
	CTime startTask, lastTask;
	
	int idx = 0;
	int jdx = 0;
	int kdx = 0;
	CTime cTime;
	CString strTime = L"";
	CString strKline = L"";

	stock *pstockList = new stock[MAX_STOCK_NUMBER];
	startTask = cTime = CTime::GetCurrentTime();

	CLog::Instance().Log(LOG_INFO, L"Save2MysqlThread Calc totalKxCount ...");
	mysqlWorkInfo->kTraderDlg->m_edtKlineMesg.SetWindowText(L"计算K线总数...");

	for (idx = 0; idx < sizeof(gDBAccessMarket) / sizeof(CDBAccessMarket); idx++) {
		bool ret = CheckOpen(gDBAccessMarket[idx].dbAccess, &buffer);
		if (!ret) {
			continue;
		}

		ASSERT(gDBAccessMarket[idx].dbAccess);
		stkCount = GetStkList(gDBAccessMarket[idx].dbAccess, pstockList, sizeof(stock) * MAX_STOCK_NUMBER);

		for (jdx = 0; jdx < stkCount; jdx++) {
			kxCount = ReadDataCount(gDBAccessMarket[idx].dbAccess, pstockList[jdx].code);
			if (mysqlWorkInfo->isDailyJob) {
				kxCount = min(gDBAccessMarket[idx].dailycount, kxCount);
			}
			else {
				if (mysqlWorkInfo->kxCount > 0) {
					kxCount = min(mysqlWorkInfo->kxCount, kxCount);
				}
			}

			totalKxCount += kxCount;
			strKline.Format(L"%d", totalKxCount);
			mysqlWorkInfo->kTraderDlg->m_edtTotalCount.SetWindowText(strKline);
		}

		strKline.Format(L"%d", totalKxCount);
		mysqlWorkInfo->kTraderDlg->m_edtTotalCount.SetWindowText(strKline);
	}

	CLog::Instance().Log(LOG_INFO, L"Save2MysqlThread Start.(totalKxCount %d)", totalKxCount);
	strKline.Format(L"%d", totalKxCount);
	mysqlWorkInfo->kTraderDlg->m_edtTotalCount.SetWindowText(strKline);

	for (idx = 0; idx < sizeof(gDBAccessMarket) / sizeof(CDBAccessMarket); idx++) {
		bool ret = CheckOpen(gDBAccessMarket[idx].dbAccess, &buffer);
		stkCount = GetStkList(gDBAccessMarket[idx].dbAccess, pstockList, sizeof(stock) * MAX_STOCK_NUMBER);

		for (jdx = 0; jdx < stkCount; jdx++) {
			if (pKbuf) {
				delete[] pKbuf;
				pKbuf = NULL;
			}

			if (mysqlWorkInfo->isDailyJob) {
				kxCount = gDBAccessMarket[idx].dailycount;
			}
			else {
				if (mysqlWorkInfo->kxCount > 0) {
					kxCount = mysqlWorkInfo->kxCount;
				}
				else {
					kxCount = ReadDataCount(gDBAccessMarket[idx].dbAccess, pstockList[jdx].code);
				}
			}

			ASSERT(kxCount > 0);
			pKbuf = new KLine[kxCount + 1];
			buffer.KBuffer = pKbuf;
			buffer.a1 = 00;
			buffer.a2 = 00;
			buffer.a3 = 00;
			buffer.Count = 0x00;
			buffer.Length = kxCount * sizeof(KLine);

			WCHAR ucode[10];
			CharToTchar(pstockList[jdx].code, ucode);

			ret = ReadLastNItem(gDBAccessMarket[idx].dbAccess, pstockList[jdx].code, kxCount, &ui1, &ll, &us1, (DWORD *)&buffer, 0);
			kxCount = buffer.Count / sizeof(KLine);
			CLog::Instance().Log(LOG_INFO, L"ReadLastNItem market:%s code:%s, type:%d, count:%d", gDBAccessMarket[idx].market, ucode, gDBAccessMarket[idx].type, kxCount);

			if (us1 != sizeof(KLine)) {
				mysqlWorkInfo->kTraderDlg->m_edtKlineMesg.SetWindowText(L"KLine长度错误!");
				delete[] pKbuf;
				doneKxCount += kxCount;
				continue;
			}

			for (kdx = 0; kdx < kxCount; kdx++) {
				doneKxCount++;

				if (!mysqlWorkInfo->kTraderDlg->SaveKline(gDBAccessMarket[idx].market, ucode, gDBAccessMarket[idx].type, pKbuf[kdx], method)) {
					error++;
					strCount.Format(L"%d", error);
					mysqlWorkInfo->kTraderDlg->m_edtErrorCount.SetWindowText(strCount);
				}

				strCount.Format(L"%d", doneKxCount);
				mysqlWorkInfo->kTraderDlg->m_edtDoneCount.SetWindowText(strCount);

				int progress = 100 * doneKxCount / totalKxCount;
				if (progress - lastProgress >= 1 || progress >= 99) {
					mysqlWorkInfo->kTraderDlg->m_pgSave.SetPos(progress);
					lastProgress = progress;
				}

				CTime tmNow = CTime::GetCurrentTime();
				int diffSec = (int)(tmNow.GetTime() - lastTask.GetTime());
				if (diffSec > SPEEDSHOW_INTERVAL_SECONDS && doneKxCount < totalKxCount && (doneKxCount - lastKxCount) > 60) {
					int taskSpeed = (doneKxCount - lastKxCount) / diffSec;
					CString strSpeed = L"0";
					strSpeed.Format(L"%d", taskSpeed);
					mysqlWorkInfo->kTraderDlg->m_edtSpeed.SetWindowText(strSpeed);

					if (taskSpeed > 1) {
						int restoftime = (int)(totalKxCount - doneKxCount) / taskSpeed;
						int rot_hour = restoftime / 3600;
						int rot_minute = (restoftime % 3600) / 60;
						int rot_second = restoftime % 60;
						CString strRestOfTime = L"00:00:00";
						strRestOfTime.Format(L"%02d:%02d:%02d", rot_hour, rot_minute, rot_second);
						mysqlWorkInfo->kTraderDlg->m_edtRestOfTime.SetWindowText(strRestOfTime);
					}

					lastTask = tmNow;
					lastKxCount = doneKxCount;
				}
			}

			CLog::Instance().Log(LOG_INFO, L"Save market:%s code:%s, type:%d, count:%d(error %d).", gDBAccessMarket[idx].market,
				ucode, gDBAccessMarket[idx].type, kdx, error);
		}

		if (pKbuf) {
			delete[] pKbuf;
			pKbuf = NULL;
		}
	}

	strCount.Format(L"K线总数:%d, 完成:%d，错误:%d", totalKxCount, doneKxCount, error);
	mysqlWorkInfo->kTraderDlg->m_edtKlineMesg.SetWindowText(strCount);
	mysqlWorkInfo->kTraderDlg->m_pgSave.SetPos(100);

	mysqlWorkInfo->kTraderDlg->m_edtRestOfTime.SetWindowText(L"00:00:00");

	CLog::Instance().Log(LOG_INFO, L"Save2MysqlThread Done.");
	mysqlWorkInfo->kTraderDlg->enableButtonGroup(true);
	return 0L;
}