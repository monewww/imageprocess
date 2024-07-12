// imageProcessing.cpp: 定义应用程序的类行为。
//
#include "framework.h"
#include "imageProcessing.h"
#include "MainFrm.h"
#include "imageProcessingDoc.h"
#include "imageProcessingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CMyInputXYDlg, CDialogEx)
IMPLEMENT_DYNAMIC(CMyInputColorDlg, CDialogEx)
IMPLEMENT_DYNAMIC(CMyInterpolationDlg, CDialogEx)
IMPLEMENT_DYNAMIC(CMyGetGuassSmoothDlg, CDialogEx)
IMPLEMENT_DYNAMIC(CMyGetMedianFilterDlg, CDialogEx)
IMPLEMENT_DYNAMIC(CMyGetBilateralFilterDlg, CDialogEx)
IMPLEMENT_DYNAMIC(CMyGetSharpGradDlg, CDialogEx)
IMPLEMENT_DYNAMIC(CMyGetCannyEdgeDlg, CDialogEx)
IMPLEMENT_DYNAMIC(CMyInputFileDlg, CDialogEx)



BEGIN_MESSAGE_MAP(CimageProcessingApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CimageProcessingApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()

CimageProcessingApp::CimageProcessingApp() noexcept
{
	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("imageProcessing.AppID.NoVersion"));

	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

CimageProcessingApp theApp;

BOOL CimageProcessingApp::InitInstance()
{
	CWinApp::InitInstance();
	EnableTaskbarInteraction(FALSE);
	// 使用 RichEdit 控件需要 AfxInitRichEdit2()
	// AfxInitRichEdit2();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)
	// 注册应用程序的文档模板。  文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CimageProcessingDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CimageProcessingView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// 调度在命令行中指定的命令。  如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};



CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CimageProcessingApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


CMyInputXYDlg::CMyInputXYDlg(CWnd* pParent)
	: CDialogEx(IDD_INPUTXY, pParent)
	, m_x(-1)
	, m_y(-1)
	,text_x(_T("x"))
	,text_y(_T("y"))
{

}

void CMyInputXYDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X, m_x);
	DDX_Text(pDX, IDC_EDIT_Y, m_y);
	DDX_Text(pDX, IDC_text_x, text_x);
	DDX_Text(pDX, IDC_text_Y, text_y);
}

BEGIN_MESSAGE_MAP(CMyInputXYDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMyInputXYDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_CANCEL, &CDialogEx::OnCancel)
END_MESSAGE_MAP()

void CMyInputXYDlg::OnBnClickedOk()
{
	CString str;
	GetDlgItemText(IDC_EDIT_X,str);
	m_x = _ttoi(str);
	GetDlgItemText(IDC_EDIT_Y,str);
	m_y = _ttoi(str);
	CDialogEx::OnOK();
}

void CMyInputXYDlg::OnCancel()
{
	m_x = -1;
	m_y = -1;
	CDialogEx::OnCancel();
}



void CAboutDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

CMyInputXYDlg::~CMyInputXYDlg() {

}

CMyInputColorDlg::CMyInputColorDlg(CWnd* pParent)
	: CDialogEx(IDD_INPUTXYANDCOLOR, pParent)
	, m_x(0)
	, m_y(0)
	, m_r(255)
	, m_g(255)
	, m_b(255)
	, m_a(255)
	, text_x(_T("x"))
	, text_y(_T("y"))
	, text_r(_T("r"))
	, text_g(_T("g"))
	, text_b(_T("b"))
	, text_a(_T("a"))
{

}

CMyInputColorDlg::~CMyInputColorDlg() {

}

void CMyInputColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X, m_x);
	DDX_Text(pDX, IDC_EDIT_Y, m_y);
	DDX_Text(pDX, IDC_EDIT_R, m_r);
	DDX_Text(pDX, IDC_EDIT_G, m_g);
	DDX_Text(pDX, IDC_EDIT_B, m_b);
	DDX_Text(pDX, IDC_EDIT_A, m_a);
	DDX_Text(pDX, IDC_text_x, text_x);
	DDX_Text(pDX, IDC_text_Y, text_y);
	DDX_Text(pDX, IDC_text_R, text_r);
	DDX_Text(pDX, IDC_text_G, text_g);
	DDX_Text(pDX, IDC_text_B, text_b);
	DDX_Text(pDX, IDC_text_A, text_a);
}

BEGIN_MESSAGE_MAP(CMyInputColorDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMyInputColorDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_CANCEL, &CDialogEx::OnCancel)
END_MESSAGE_MAP()

void CMyInputColorDlg::OnBnClickedOk()
{
	CString str;
	GetDlgItemText(IDC_EDIT_X, str);
	m_x = _ttoi(str);
	GetDlgItemText(IDC_EDIT_Y, str);
	m_y = _ttoi(str);
	GetDlgItemText(IDC_EDIT_R, str);
	m_r = _ttoi(str);
	GetDlgItemText(IDC_EDIT_G, str);
	m_g = _ttoi(str);
	GetDlgItemText(IDC_EDIT_B, str);
	m_b = _ttoi(str);
	GetDlgItemText(IDC_EDIT_A, str);
	m_a = _ttoi(str);
	CDialogEx::OnOK();
}

void CMyInputColorDlg::OnCancel()
{
	m_x = 0;
	m_y = 0;
	m_r = 255;
	m_g = 255;
	m_b = 255;
	m_a = 255;
	CDialogEx::OnCancel();
}

void CMyInterpolationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X, m_x);
	DDX_Text(pDX, IDC_EDIT_Y, m_y);
	DDX_Text(pDX, IDC_EDIT_METHOD, m_method);
	DDX_Text(pDX, IDC_text_x, text_x);
	DDX_Text(pDX, IDC_text_Y, text_y);
	DDX_Text(pDX, IDC_text_METHOD, text_method);

}

BEGIN_MESSAGE_MAP(CMyInterpolationDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMyInterpolationDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_CANCEL, &CDialogEx::OnCancel)
END_MESSAGE_MAP()

void CMyInterpolationDlg::OnBnClickedOk()
{
	CString str;
	GetDlgItemText(IDC_EDIT_X, str);
	m_x = _ttoi(str);
	GetDlgItemText(IDC_EDIT_Y, str);
	m_y = _ttoi(str);
	GetDlgItemText(IDC_EDIT_METHOD, str);
	m_method = _ttoi(str);
	CDialogEx::OnOK();

}

void CMyInterpolationDlg::OnCancel()
{
	m_x = 100;
	m_y = 100;
	m_method = 0;
	CDialogEx::OnCancel();
}

CMyInterpolationDlg::CMyInterpolationDlg(CWnd* pParent)
	: CDialogEx(IDD_Interpolation, pParent)
	, m_x(100)
	, m_y(100)
	, m_method(0)
	, text_x(_T("new x"))
	, text_y(_T("new y"))
	, text_method(_T("method"))
{
}

CMyInterpolationDlg::~CMyInterpolationDlg()
{
}

CMyGetGuassSmoothDlg::CMyGetGuassSmoothDlg(CWnd* pParent)
	: CDialogEx(IDD_GETGAUSSSMOOTH, pParent)
	, m_Guass_sigma(1)
	, text_Guass_sigma(_T("Guass sigma"))
{
}

CMyGetGuassSmoothDlg::~CMyGetGuassSmoothDlg()
{
}

void CMyGetGuassSmoothDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GUASSSIGMA, m_Guass_sigma);
	DDX_Text(pDX, IDC_text_GUASSSIGMA, text_Guass_sigma);
}

BEGIN_MESSAGE_MAP(CMyGetGuassSmoothDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMyGetGuassSmoothDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_CANCEL, &CDialogEx::OnCancel)
	END_MESSAGE_MAP()

void CMyGetGuassSmoothDlg::OnBnClickedOk()
{
	CString str;
	GetDlgItemText(IDC_EDIT_GUASSSIGMA, str);
	m_Guass_sigma = _ttof(str);
	CDialogEx::OnOK();
}

void CMyGetGuassSmoothDlg::OnCancel()
{
	m_Guass_sigma = 1;
	CDialogEx::OnCancel();
}

CMyGetMedianFilterDlg::CMyGetMedianFilterDlg(CWnd* pParent)
	: CDialogEx(IDD_MEDIAN_FILTERING, pParent)
	, m_Median_size(3)
	, text_Median_size(_T("Median size"))
{
}

CMyGetMedianFilterDlg::~CMyGetMedianFilterDlg()
{
}

void CMyGetMedianFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MEDIAN, m_Median_size);
	DDX_Text(pDX, IDC_text_MEDIAN, text_Median_size);
}

BEGIN_MESSAGE_MAP(CMyGetMedianFilterDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMyGetMedianFilterDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_CANCEL, &CDialogEx::OnCancel)

	END_MESSAGE_MAP()

void CMyGetMedianFilterDlg::OnBnClickedOk()
{
	CString str;
	GetDlgItemText(IDC_EDIT_MEDIAN, str);
	m_Median_size = _ttoi(str);
	CDialogEx::OnOK();
}

void CMyGetMedianFilterDlg::OnCancel()
{
	m_Median_size = 3;
	CDialogEx::OnCancel();
}

CMyGetBilateralFilterDlg::CMyGetBilateralFilterDlg(CWnd* pParent)
	: CDialogEx(IDD_BILATERAL_FILTERING, pParent)
	, m_Bilateral_size(3)
	, m_Bilateral_sigma_d(1)
	, m_Bilateral_sigma_r(1)
	, text_Bilateral_size(_T("Bilateral size"))
	, text_Bilateral_sigma_d(_T("Bilateral sigma_d"))
	, text_Bilateral_sigma_r(_T("Bilateral sigma_r"))
{
}

CMyGetBilateralFilterDlg::~CMyGetBilateralFilterDlg()
{
}

void CMyGetBilateralFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BILATERAL_SIZE, m_Bilateral_size);
	DDX_Text(pDX, IDC_EDIT_BILATERAL_SIGMA_D, m_Bilateral_sigma_d);
	DDX_Text(pDX, IDC_EDIT_BILATERAL_SIGMA_R, m_Bilateral_sigma_r);
	DDX_Text(pDX, IDC_text_BILATERAL_SIZE, text_Bilateral_size);
	DDX_Text(pDX, IDC_text_BILATERAL_SIGMA_D, text_Bilateral_sigma_d);
	DDX_Text(pDX, IDC_text_BILATERAL_SIGMA_R, text_Bilateral_sigma_r);
}

BEGIN_MESSAGE_MAP(CMyGetBilateralFilterDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMyGetBilateralFilterDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_CANCEL, &CDialogEx::OnCancel)
END_MESSAGE_MAP()

void CMyGetBilateralFilterDlg::OnBnClickedOk()
{
	CString str;
	GetDlgItemText(IDC_EDIT_BILATERAL_SIZE, str);
	m_Bilateral_size = _ttoi(str);
	GetDlgItemText(IDC_EDIT_BILATERAL_SIGMA_D, str);
	m_Bilateral_sigma_d = _ttof(str);
	GetDlgItemText(IDC_EDIT_BILATERAL_SIGMA_R, str);
	m_Bilateral_sigma_r = _ttof(str);
	CDialogEx::OnOK();
}

void CMyGetBilateralFilterDlg::OnCancel()
{
	m_Bilateral_size = 3;
	m_Bilateral_sigma_d = 1;
	m_Bilateral_sigma_r = 1;
	CDialogEx::OnCancel();
}

CMyGetSharpGradDlg::CMyGetSharpGradDlg(CWnd* pParent)
	: CDialogEx(IDD_SHARPENGRAD, pParent)
	, m_sharpness(1)
	, text_sharpness(_T("sharpness"))
{
}

CMyGetSharpGradDlg::~CMyGetSharpGradDlg()
{
}

void CMyGetSharpGradDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SHARPNESS, m_sharpness);
	DDX_Text(pDX, IDC_text_SHARPNESS, text_sharpness);
}

BEGIN_MESSAGE_MAP(CMyGetSharpGradDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMyGetSharpGradDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_CANCEL, &CDialogEx::OnCancel)

	END_MESSAGE_MAP()

void CMyGetSharpGradDlg::OnBnClickedOk()
{
	CString str;
	GetDlgItemText(IDC_EDIT_SHARPNESS, str);
	m_sharpness = _ttoi(str);
	CDialogEx::OnOK();
}

void CMyGetSharpGradDlg::OnCancel()
{
	m_sharpness = -1;
	CDialogEx::OnCancel();
}

CMyGetCannyEdgeDlg::CMyGetCannyEdgeDlg(CWnd* pParent)
	: CDialogEx(IDD_CANNYEDGE, pParent)
	, m_guass_sigma(1)
	, m_lowThreshold(50)
	, m_highThreshold(100)
	, text_lowThreshold(_T("lowThreshold"))
	, text_highThreshold(_T("highThreshold"))
	, text_guass_sigma(_T("guass sigma"))
{
}

CMyGetCannyEdgeDlg::~CMyGetCannyEdgeDlg()
{
}

void CMyGetCannyEdgeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LOWTHRESHOLD, m_lowThreshold);
	DDX_Text(pDX, IDC_EDIT_HIGHTHRESHOLD, m_highThreshold);
	DDX_Text(pDX, IDC_text_LOWTHRESHOLD, text_lowThreshold);
	DDX_Text(pDX, IDC_text_HIGHTHRESHOLD, text_highThreshold);
	DDX_Text(pDX, IDC_EDIT_GUASSSIGMA, m_guass_sigma);
	DDX_Text(pDX, IDC_text_GUASSSIGMA, text_guass_sigma);
}

BEGIN_MESSAGE_MAP(CMyGetCannyEdgeDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMyGetCannyEdgeDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_CANCEL, &CDialogEx::OnCancel)
	END_MESSAGE_MAP()

void CMyGetCannyEdgeDlg::OnBnClickedOk()
{
	CString str;
	GetDlgItemText(IDC_EDIT_LOWTHRESHOLD, str);
	m_lowThreshold = _ttoi(str);
	GetDlgItemText(IDC_EDIT_HIGHTHRESHOLD, str);
	m_highThreshold = _ttoi(str);
	GetDlgItemText(IDC_EDIT_GUASSSIGMA, str);
	m_guass_sigma = _ttoi(str);
	CDialogEx::OnOK();
}

void CMyGetCannyEdgeDlg::OnCancel()
{
	m_lowThreshold = 50;
	m_highThreshold = 100;
	m_guass_sigma = -1;
	CDialogEx::OnCancel();
}

CMyInputFileDlg::CMyInputFileDlg(CWnd* pParent)
	: CDialogEx(IDD_INPUTFILENAME, pParent)
	, m_fileName(_T("1.bmp"))
	, text_fileName(_T("file name"))
{
}

CMyInputFileDlg::~CMyInputFileDlg()
{
}

void CMyInputFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_fileName);
	DDX_Text(pDX, IDC_text_FILENAME, text_fileName);
}

BEGIN_MESSAGE_MAP(CMyInputFileDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMyInputFileDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_CANCEL, &CDialogEx::OnCancel)

	END_MESSAGE_MAP()

void CMyInputFileDlg::OnBnClickedOk()
{
	CString str;
	GetDlgItemText(IDC_EDIT_FILENAME, str);
	m_fileName = str;
	CDialogEx::OnOK();
}

void CMyInputFileDlg::OnCancel()
{
	m_fileName = _T("1.bmp");
	CDialogEx::OnCancel();
}

