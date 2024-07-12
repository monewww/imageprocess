// imageProcessingView.cpp: CimageProcessingView 类的实现
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "framework.h"
#include "imageProcessing.h"
#include "imageProcessingDoc.h"
#include "imageProcessingView.h"
#include "_GlobalCommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CimageProcessingView, CView)

BEGIN_MESSAGE_MAP(CimageProcessingView, CView)
	ON_COMMAND(ID_IMAGEPROCESS_OPENBMPFILE, &CimageProcessingView::OnImageprocessOpenbmpfile)
	ON_COMMAND(ID_IMAGEPROCESS_SAVETOFILE, &CimageProcessingView::OnImageprocessSavetofile)
	ON_COMMAND(ID_IMAGEPROCESS_DISPLAYFILEHEADER, &CimageProcessingView::OnImageprocessDisplayfileheader)
	ON_COMMAND(ID_IMAGEPROCESS_DISPLAYPALETTE, &CimageProcessingView::OnImageprocessDisplaypalette)
	ON_COMMAND(ID_IMAGEPROCESS_GETPIXELVALUE, &CimageProcessingView::OnImageprocessGetpixelvalue)
	ON_COMMAND(ID_IMAGEPROCESS_SETPIXELVALUE, &CimageProcessingView::OnImageprocessSetpixelvalue)
	ON_COMMAND(ID_IMAGEPROCESS_INERPOLATION, &CimageProcessingView::OnImageprocessInerpolation)
	ON_COMMAND(ID_IMAGEPROCESS_GAUSSSMOOTH, &CimageProcessingView::OnImageprocessGausssmooth)
	ON_COMMAND(ID_IMAGEPROCESS_MEDIANFILTER, &CimageProcessingView::OnImageprocessMedianfilter)
	ON_COMMAND(ID_IMAGEPROCESS_BILATERALFILTER, &CimageProcessingView::OnImageprocessBilateralfilter)
	ON_COMMAND(ID_IMAGEPROCESS_HISTOEQUALIZATION, &CimageProcessingView::OnImageprocessHistoequalization)
	ON_COMMAND(ID_IMAGEPROCESS_SHARPENGRAD, &CimageProcessingView::OnImageprocessSharpengrad)
	ON_COMMAND(ID_IMAGEPROCESS_CANNYEDGE, &CimageProcessingView::OnImageprocessCannyedge)
	ON_COMMAND(ID_IMAGEPROCESS_OTSUSEGMENT, &CimageProcessingView::OnImageprocessOtsusegment)
END_MESSAGE_MAP()

CimageProcessingView::CimageProcessingView() noexcept
{
	pFileBuf = NULL;
}

CimageProcessingView::~CimageProcessingView()
{
	if( pFileBuf ) 
	{
		delete [] pFileBuf;
		pFileBuf = 0;
	}
}

BOOL CimageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

#ifdef _DEBUG
void CimageProcessingView::AssertValid() const
{
	CView::AssertValid();
}

void CimageProcessingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CimageProcessingDoc *CimageProcessingView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CimageProcessingDoc)));
	return (CimageProcessingDoc*)m_pDocument;
}
#endif //_DEBUG

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

// CimageProcessingView 绘图
void CimageProcessingView::OnDraw(CDC *pDC)
{
	CimageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if( pFileBuf != NULL )
	{
		DisplayImage(pDC,pFileBuf,10,10,0,0,0);
	}
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//Here are the functions to be programmed by you!

//Open a BMP file
void CimageProcessingView::OnImageprocessOpenbmpfile()
{
	LPCTSTR lpszFilter = "BMP Files (*.bmp)|*.bmp||";
	CFileDialog dlg(TRUE,NULL,NULL,OFN_NOCHANGEDIR,lpszFilter,NULL);
	if( dlg.DoModal() != IDOK ) return;
	if( pFileBuf != NULL )
	{
		delete [] pFileBuf;
	}
	pFileBuf = OpenBMPfile( dlg.GetPathName() );
	Invalidate();
	UpdateWindow();
}

//Save to a new BMP file
void CimageProcessingView::OnImageprocessSavetofile()
{
	if(pFileBuf == NULL) return;
	/**/
	//Add your code to choose the new BMP filename
	//从对话框中获取文件名
	CMyInputFileDlg dlg;
	if (dlg.DoModal() != IDOK) return;
	//保存文件
	CString strBmpFile = dlg.m_fileName;
	//文件命前添加_TestImages
	strBmpFile = _T("_TestImages\\") + strBmpFile;
	SaveDIB(pFileBuf, strBmpFile);
}

//Display BMP file header
void CimageProcessingView::OnImageprocessDisplayfileheader()
{
	if(pFileBuf == NULL) return;
	/**/
	DisplayHeaderMessage(pFileBuf);
}

//Display Pallete
void CimageProcessingView::OnImageprocessDisplaypalette()
{
	if(pFileBuf == NULL) return;
	/**/
	int num = 0;
	RGBQUAD *pallete = GetDIBPaletteData(pFileBuf,&num);
	if( pallete == NULL )
	{
		AfxMessageBox("No palette");
	}
	else
	{
		CString paletteString;
		for (int i = 0; i < num; i++) {
			RGBQUAD color = pallete[i];
			// 将颜色的 RGB 值添加到字符串中
			CString strColor;
			strColor.Format(_T("Color %d: R=%d, G=%d, B=%d ,A=%d \n"), i + 1, color.rgbRed, color.rgbGreen, color.rgbBlue,color.rgbReserved);
			paletteString += strColor;
		}
		AfxMessageBox(paletteString);
	}
}

//Get pixel value
void CimageProcessingView::OnImageprocessGetpixelvalue()
{
	if(pFileBuf == NULL) return;
	/**/
	//Add your code to choose the coordinate (x,y)
	int x = 100;
	int y = 100;
	//输入坐标
	CMyInputXYDlg dlg;
	dlg.DoModal();
	x = dlg.m_x;
	y = dlg.m_y;
	if (x < 0 || x >= GetImageWidth(pFileBuf) || y < 0 || y >= GetImageHeight(pFileBuf)) {
		AfxMessageBox("Invalid coordinate");
		return;
	}
	RGBQUAD rgb;
	bool bGray;
	GetPixel(pFileBuf,x,y,&rgb,&bGray);
	char buf[100];
	if( bGray )
		sprintf(buf, "(%d,%d) = %d",x,y,rgb.rgbReserved);
	else
		sprintf(buf, "(%d,%d) = (%d,%d,%d)",x,y,rgb.rgbRed,rgb.rgbGreen,rgb.rgbBlue);
	AfxMessageBox( buf );
}

//Set pixel value
void CimageProcessingView::OnImageprocessSetpixelvalue()
{
	if(pFileBuf == NULL) return;
	/**/
	//Add your code to choose the coordinate (x,y)
	int x = 100;
	int y = 100;
	CMyInputColorDlg dlg;
	dlg.DoModal();
	x = dlg.m_x;
	y = dlg.m_y;
	if (x < 0 || x >= GetImageWidth(pFileBuf) || y < 0 || y >= GetImageHeight(pFileBuf)) {
		AfxMessageBox("Invalid coordinate");
		return;
	}
	RGBQUAD rgb;
	rgb.rgbRed = dlg.m_r;
	rgb.rgbGreen = dlg.m_g;
	rgb.rgbBlue = dlg.m_b;
	rgb.rgbReserved = dlg.m_a;
	SetPixel(pFileBuf,x,y,rgb);
	Invalidate();
	UpdateWindow();
}

//Image interpolaion
void CimageProcessingView::OnImageprocessInerpolation()
{
	if(pFileBuf == NULL) return;
	/**/
	//Add your code to choose method (nearest or bilinear) and zoom factors
	int newWidth  = 500;
	int newHeight = 490;
	int new_method = 0;
	CMyInterpolationDlg dlg;
	dlg.DoModal();
	newWidth = dlg.m_x;
	newHeight = dlg.m_y;
	new_method = dlg.m_method;
	if (newWidth <= 0 || newHeight <= 0)
	{
		AfxMessageBox("Invalid zoom factors");
		return;
	}
	if (new_method != 0 && new_method != 1)
	{
		AfxMessageBox("Invalid method");
		return;
	}
	//Image interpolation
	char *pNewImage = ImageInterpolation(pFileBuf,newWidth,newHeight, new_method);
	delete [] pFileBuf;
	pFileBuf = pNewImage;
	Invalidate();
	UpdateWindow();
}

//Gaussian smoothing
void CimageProcessingView::OnImageprocessGausssmooth()
{
	if(pFileBuf == NULL) return;
	/**/
	//Add your code to choose the sigma value
	int sigma = 1;
	CMyGetGuassSmoothDlg dlg;
	dlg.DoModal();
	sigma = dlg.m_Guass_sigma;
	if (sigma <= 0)
	{
		AfxMessageBox("Invalid sigma value");
		return;
	}
	//Gaussian smoothing
	GaussianSmooth(pFileBuf,sigma);
	Invalidate();
	UpdateWindow();

}

//Median filtering
void CimageProcessingView::OnImageprocessMedianfilter()
{
		if (pFileBuf == NULL) return;
	/**/
	//Add your code to choose the window size
	int size = 3;
	CMyGetMedianFilterDlg dlg;
	dlg.DoModal();
	size = dlg.m_Median_size;
	if (size <= 0)
	{
		AfxMessageBox("Invalid window size");
		return;
	}
	//Median filtering
	MedianFilter(pFileBuf, size);
	Invalidate();
	UpdateWindow();
}

//Bilateral filtering
void CimageProcessingView::OnImageprocessBilateralfilter()
{
	if (pFileBuf == NULL) return;
	/**/
	//Add your code to choose the window size, sigma_d and sigma_r
	int size = 3;
	double sigma_d = 1.0;
	double sigma_r = 1.0;
	CMyGetBilateralFilterDlg dlg;
	dlg.DoModal();
	size = dlg.m_Bilateral_size;
	sigma_d = dlg.m_Bilateral_sigma_d;
	sigma_r = dlg.m_Bilateral_sigma_r;
	if (size <= 0 || sigma_d <= 0 || sigma_r <= 0)
	{
		AfxMessageBox("Invalid parameters");
		return;
	}
	//Bilateral filtering
	BilateralFilter(pFileBuf, size, sigma_d, sigma_r);
	Invalidate();
	UpdateWindow();
}

//Histogram equalization
void CimageProcessingView::OnImageprocessHistoequalization()
{
	if (pFileBuf == NULL) return;
	Histoequalization(pFileBuf);
	Invalidate();
	UpdateWindow();
}

//Sharpening by gradient
void CimageProcessingView::OnImageprocessSharpengrad()
{
	if (pFileBuf == NULL) return;
	float sharpness = 0.5;
	CMyGetSharpGradDlg dlg;
	dlg.DoModal();
	sharpness = dlg.m_sharpness;
	if (sharpness <= 0)
	{
		AfxMessageBox("Invalid sharpness value");
		return;
	}
	//Sharpening by gradient
	SharpenGrad(pFileBuf, sharpness);
	Invalidate();
	UpdateWindow();
}

//Cany edge detection
void CimageProcessingView::OnImageprocessCannyedge()
{		if (pFileBuf == NULL) return;
	/**/
	//Add your code to choose the threshold values
	int Gusss_sigma = 1;
	int lowThreshold = 50;
	int highThreshold = 100;
	CMyGetCannyEdgeDlg dlg;
	dlg.DoModal();
	Gusss_sigma = dlg.m_guass_sigma;
	lowThreshold = dlg.m_lowThreshold;
	highThreshold = dlg.m_highThreshold;
	if (lowThreshold <= 0 || highThreshold <= 0 || lowThreshold >= highThreshold)
	{
		AfxMessageBox("Invalid threshold values");
		return;
	}
	//Canny edge detection
	CannyEdge(pFileBuf, lowThreshold, highThreshold,Gusss_sigma);
	Invalidate();
	UpdateWindow();
}

//Otsu segmentation
void CimageProcessingView::OnImageprocessOtsusegment()
{
	if (pFileBuf == NULL) return;
	//Otsu segmentation
	OtsuSegment(pFileBuf);
	Invalidate();
	UpdateWindow();
}
