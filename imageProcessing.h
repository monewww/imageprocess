// imageProcessing.h: imageProcessing 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"       // 主符号

class CimageProcessingApp : public CWinApp
{
public:
	CimageProcessingApp() noexcept;

// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

class CMyInputXYDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyInputXYDlg)	
public:
	CMyInputXYDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyInputXYDlg();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INPUTXY };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	int m_x;
	int m_y;
	afx_msg void OnBnClickedOk();
	afx_msg void OnCancel();
	CString text_x;
	CString text_y;
};

class CMyInputColorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyInputColorDlg)
public:
	CMyInputColorDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyInputColorDlg();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INPUTCOLOR };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	int m_x;
	int m_y;
	int m_r;
	int m_g;
	int m_b;
	int m_a;
	afx_msg void OnBnClickedOk();
	afx_msg void OnCancel();
	CString text_x;
	CString text_y;
	CString text_r;
	CString text_g;
	CString text_b;
	CString text_a;
};

class CMyInterpolationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyInterpolationDlg)
public:
	CMyInterpolationDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyInterpolationDlg();
#ifdef AFX_DESIGN_TIME
enum { IDD = IDD_INERPOLATION };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	int m_x;
	int m_y;
	int m_method;
	afx_msg void OnBnClickedOk();
	afx_msg void OnCancel();
	CString text_x;
	CString text_y;
	CString text_method;	
};

class CMyGetGuassSmoothDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyGetGuassSmoothDlg)
public:
	CMyGetGuassSmoothDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyGetGuassSmoothDlg();
#ifdef AFX_DESIGN_TIME
enum { IDD = IDD_GAUSSIANSMOOTHING };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	int m_Guass_sigma;
	afx_msg void OnBnClickedOk();
	afx_msg void OnCancel();
	CString text_Guass_sigma;

};

class CMyGetMedianFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyGetMedianFilterDlg)
public:
	CMyGetMedianFilterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyGetMedianFilterDlg();
#ifdef AFX_DESIGN_TIME
enum { IDD = IDD_MEDIANFILTER };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	int m_Median_size;
	afx_msg void OnBnClickedOk();
	afx_msg void OnCancel();
	CString text_Median_size;

};

class CMyGetBilateralFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyGetBilateralFilterDlg)
public:
	CMyGetBilateralFilterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyGetBilateralFilterDlg();
#ifdef AFX_DESIGN_TIME
enum { IDD = IDD_BILATERALFILTER };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	int m_Bilateral_size;
	int m_Bilateral_sigma_d;
	int m_Bilateral_sigma_r;
	afx_msg void OnBnClickedOk();
	afx_msg void OnCancel();
	CString text_Bilateral_size;
	CString text_Bilateral_sigma_d;
	CString text_Bilateral_sigma_r;
};

class CMyGetSharpGradDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyGetSharpGradDlg)
public:
	CMyGetSharpGradDlg(CWnd * pParent = NULL);   // 标准构造函数
	virtual ~CMyGetSharpGradDlg();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHARPGRAD };
#endif
protected:
	virtual void DoDataExchange(CDataExchange * pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	float m_sharpness;
	afx_msg void OnBnClickedOk();
	afx_msg void OnCancel();
	CString text_sharpness;
};

class CMyGetCannyEdgeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyGetCannyEdgeDlg)
public:
	CMyGetCannyEdgeDlg(CWnd * pParent = NULL);   // 标准构造函数
	virtual ~CMyGetCannyEdgeDlg();
#ifdef AFX_DESIGN_TIME
enum { IDD = IDD_CANNYEDGE };
#endif
protected:
	virtual void DoDataExchange(CDataExchange * pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	int m_lowThreshold;
	int m_highThreshold;
	int m_guass_sigma;
	afx_msg void OnBnClickedOk();
	afx_msg void OnCancel();
	CString text_lowThreshold;
	CString text_highThreshold;
	CString text_guass_sigma;
};

class CMyInputFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyInputFileDlg)
public:
	CMyInputFileDlg(CWnd* pParent = NULL);
	virtual ~CMyInputFileDlg();
#ifdef AFX_DESIGN_TIME
enum { IDD = IDD_CANNYEDGE };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	CString m_fileName;
	afx_msg void OnBnClickedOk();
	afx_msg void OnCancel();
	CString text_fileName;


};

extern CimageProcessingApp theApp;
