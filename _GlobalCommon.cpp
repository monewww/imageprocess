//GlobalCommon.cpp
#define _CRT_SECURE_NO_WARNINGS
#include "_GlobalCommon.h"
#include <cstring>

/**
	 功能: 从图像文件中建造DIB类
	 参数: strBmpFile --- 需要打开的BMP文件名
	 返回：文件缓冲区指针 (NULL表示失败)
**/
char *OpenBMPfile(CString strBmpFile)
{
	CFile hFile;
	if( !hFile.Open(strBmpFile,CFile::modeRead|CFile::typeBinary) )
	{
		AfxMessageBox("Failed to open the BMP file");
		return( NULL );
	}
	/**/
//	if(	hFile.Seek(0L,CFile::begin) != 0L )
//	{
//		return( NULL );
//	}
	/**/
	long lFileSize = (long)hFile.Seek(0L, CFile::end);
	char *pFileBuf = new char [lFileSize+1];
	hFile.Seek(0L, CFile::begin);
	hFile.Read(pFileBuf, lFileSize);
	hFile.Close();
	/**/
	BITMAPFILEHEADER *pBmpHead = (BITMAPFILEHEADER *)pFileBuf;
	BITMAPINFOHEADER *pBmpInfo = (BITMAPINFOHEADER *)(pFileBuf + sizeof(BITMAPFILEHEADER));
	/**/
	if(	pBmpHead->bfType   != 0x4D42 ||		//"BM"=0x424D
	    pBmpInfo->biSize   != 0x28   ||		// 位图信息子结构长度(等于40,即0x28)
		pBmpInfo->biPlanes != 0x01 )		// 此域必须等于1
	{
		AfxMessageBox("It isn't a valid BMP file");
		return( NULL );
	}
	/**/
	if( pBmpInfo->biCompression != BI_RGB )
	{
		AfxMessageBox("It is a compressed BMP file");
		return( NULL );
	}
	/**/
	if( pBmpInfo->biBitCount != 8  &&
	    pBmpInfo->biBitCount != 24 )
	{
		AfxMessageBox("Only 8-bit and 24-bit BMP files are supported");
		return( NULL );
	}
	/**/
	return( pFileBuf );
}

///////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////	

BITMAPFILEHEADER *GetDIBHEADER(char *pFileBuffer)
{
	char *p = pFileBuffer + 0;
	return( (BITMAPFILEHEADER *)p );
}

BITMAPINFOHEADER *GetDIBINFO(char *pFileBuffer)
{
	char *p = pFileBuffer + sizeof(BITMAPFILEHEADER);
	return( (BITMAPINFOHEADER *)p );
}

char *GetDIBImageData(char *pFileBuffer)
{
	const BITMAPFILEHEADER *pBmpHead = GetDIBHEADER(pFileBuffer);
	char *p = pFileBuffer + pBmpHead->bfOffBits;
	return( p );
}

//return NULL denoting no palette
RGBQUAD *GetDIBPaletteData(char *pFileBuffer,int nEntryNumber[1])
{
	char *pPaletteData = NULL;
	if( GetColorBits(pFileBuffer) <= 8 )
	{
		nEntryNumber[0] = 0;
		char *pDIBImageData = GetDIBImageData(pFileBuffer);
		pPaletteData = pFileBuffer + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		int  nNum = (DWORD)(pDIBImageData - pPaletteData) / sizeof(RGBQUAD);
		int  iUsedColors = (int)GetDIBINFO(pFileBuffer)->biClrUsed;
		if( nNum > 0 && (int)iUsedColors > 0 )
			nEntryNumber[0] = min(nNum,(int)iUsedColors);
		else
			pPaletteData = NULL;
	}
	return( (RGBQUAD *)pPaletteData );
}

int GetImageWidth(char *pFileBuffer)
{
	BITMAPINFOHEADER *pInfo = GetDIBINFO(pFileBuffer);
	return( pInfo->biWidth );
}

int GetImageHeight(char *pFileBuffer)
{
	BITMAPINFOHEADER *pInfo = GetDIBINFO(pFileBuffer);
	return( pInfo->biHeight );
}

int GetColorBits(char *pFileBuffer)
{
	BITMAPINFOHEADER *pInfo = GetDIBINFO(pFileBuffer);
	return( pInfo->biBitCount );
}

long GetUsedColors(char *pFileBuffer)
{
	BITMAPINFOHEADER *pInfo = GetDIBINFO(pFileBuffer);
	return( (long)pInfo->biClrUsed );
}

long GetWidthBytes(char *pFileBuffer)
{
	BITMAPINFOHEADER *pInfo = GetDIBINFO(pFileBuffer);
	long nBytesPerRow = 4 * ((pInfo->biWidth * pInfo->biBitCount + 31) / 32);
	return( nBytesPerRow );
}

///////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////	

void DisplayHeaderMessage(char *pBmpFileBuf)
{
	BITMAPFILEHEADER *pBmpHead = GetDIBHEADER(pBmpFileBuf);
	BITMAPINFOHEADER *pBmpInfo = GetDIBINFO(pBmpFileBuf);
	/**/
	char msg[4096];
	sprintf(msg,
	"bfType (file type) = %4.4X \n"
	"bfSize (file length) = %ld \n"
	"bfOffBits (offset of bitmap data in bytes) = %ld \n"
	"biSize (header structure length should be 40 or 0x28) = %ld \n"
	"biWidth (image width)  = %ld \n"
	"biHeight (image height) = %ld \n"
	"biPlanes (must be eaual to 1) = %u \n"
	"biBitCount (color/pixel bits) = %u \n"
	"biCompression (compressed?) = %ld \n"
	"biSizeImage (length of bitmap data in bytes must be the times of 4) = %ld \n"
	"biXPelsPerMeter (horizontal resolution of target device in pixels/metre) = %ld \n"
	"biYPelsPerMeter (vertical resolution of target device in pixels/metre) = %ld \n"
	"biColorUsed (number of colors used in bitmap,0=2**biBitCount) = %ld \n"
	"biColorImportant (number of important colors,0=all colors are important) = %ld \n\n"
	"The following is additional information: \n"
	"Bytes per row in bitmap (nBytesPerRow) = %ld \n"
	"Total bytes of bitmap (nImageSizeInByte) = %ld \n"
	"Actual pixels per row in bitmap (nPixelsPerRow) = %ld \n"
	"Total rows of bitmap (nTotalRows) = %ld \n"
	"Total colors (2**biBitCount)(nTotalColors) = %ld \n"
	"Used colors (biColorUsed)(nUsedColors) = %ld ",
	pBmpHead->bfType,
	pBmpHead->bfSize,
	pBmpHead->bfOffBits,
	pBmpInfo->biSize,
	pBmpInfo->biWidth,
	pBmpInfo->biHeight,
	pBmpInfo->biPlanes,
	pBmpInfo->biBitCount,
	pBmpInfo->biCompression,
	pBmpInfo->biSizeImage,
	pBmpInfo->biXPelsPerMeter,
	pBmpInfo->biYPelsPerMeter,
	pBmpInfo->biClrUsed,
	pBmpInfo->biClrImportant,
	GetWidthBytes(pBmpFileBuf),
	GetWidthBytes(pBmpFileBuf) * GetImageHeight(pBmpFileBuf),
	GetImageWidth(pBmpFileBuf),
	GetImageHeight(pBmpFileBuf),
	1 << GetColorBits(pBmpFileBuf),
	GetUsedColors(pBmpFileBuf) );
	AfxMessageBox(msg);
}

//Mode = 0, normal display
//   1,2,3, display grayscale image in red, green, blue colors
void DisplayImage(CDC *pDC,char *pBmpFileBuf,int disp_xL,int disp_yL,int disp_Width,int disp_Height,int mode)
{
	ASSERT( pDC != NULL );
	HDC hDC = pDC->GetSafeHdc();
	ASSERT( hDC != 0 );
	/**/
	int imageWidth  = GetImageWidth(pBmpFileBuf);
	int imageHeight = GetImageHeight(pBmpFileBuf);
	if( disp_Width <= 0 || disp_Height <= 0 )
	{
		disp_Width  = imageWidth;
		disp_Height = imageHeight;
	}
	CRect rect;
	CWnd *pWnd = pDC->GetWindow();
	pWnd->GetClientRect(&rect);
	disp_Width = min(disp_Width, rect.right - disp_xL);
	disp_Height = min(disp_Height, rect.bottom - disp_yL);
	/**/
	BITMAPINFOHEADER *pBitmapInfo = GetDIBINFO(pBmpFileBuf);
	char *pDIBImageData = GetDIBImageData(pBmpFileBuf);
	/**/
	char buf[40+256*4];
	BITMAPINFO *pBitsInfo = (BITMAPINFO *)buf;
	memcpy(&pBitsInfo->bmiHeader,pBitmapInfo,sizeof(BITMAPINFOHEADER));
	/**/
	int palleteNum = 0;
	RGBQUAD *pallete = GetDIBPaletteData(pBmpFileBuf,&palleteNum);
	for(int c = 0; c < 256; c++)
	{
		if( mode == 0 )
		{
			(pBitsInfo->bmiColors[c]).rgbRed   = (pallete!=NULL && c<palleteNum? pallete[c].rgbRed   : c);
			(pBitsInfo->bmiColors[c]).rgbGreen = (pallete!=NULL && c<palleteNum? pallete[c].rgbGreen : c);
			(pBitsInfo->bmiColors[c]).rgbBlue  = (pallete!=NULL && c<palleteNum? pallete[c].rgbBlue  : c);
		}
		else
		{
			(pBitsInfo->bmiColors[c]).rgbRed   = (mode==1? c : 0); 
			(pBitsInfo->bmiColors[c]).rgbGreen = (mode==2? c : 0); 
			(pBitsInfo->bmiColors[c]).rgbBlue  = (mode==3? c : 0); 
		}
	}
	/**/
	SetStretchBltMode(hDC,COLORONCOLOR);
	StretchDIBits(hDC,disp_xL,disp_yL,disp_Width,disp_Height,
	0,0,imageWidth,imageHeight,pDIBImageData,pBitsInfo,DIB_RGB_COLORS,SRCCOPY );
	/**/
	return;
}

///////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////	
//   象素操作

//	 读象素颜色值
//	 返回: >=0 表示象素在位图数据中的偏移值
//		    <0 失败或参数无效
long GetPixel(char *pFileBuffer,int x,int y,RGBQUAD rgb[1],bool bGray[1])
{
	int  nColorBits   = GetColorBits(pFileBuffer);
	int  nImageHeight = GetImageHeight(pFileBuffer);
	int  nBytesPerRow = GetWidthBytes(pFileBuffer);
	/**/
	long nOffInImage  = (nImageHeight-1-y) * nBytesPerRow;
	char *p = GetDIBImageData(pFileBuffer) + nOffInImage;
	/**/
	if( bGray != NULL ) *bGray = true;
	if( nColorBits == 8 )
	{
		nOffInImage += x;
		rgb[0].rgbReserved = p[x];
		rgb[0].rgbRed      = p[x];
		rgb[0].rgbGreen    = p[x];
		rgb[0].rgbBlue     = p[x];
	}
	else if( nColorBits == 24 )
	{
		if( bGray != NULL ) *bGray = false;
		nOffInImage += 3 * x;
		p += (3 * x);
		rgb[0].rgbReserved = 0;
		rgb[0].rgbRed      = p[2];
		rgb[0].rgbGreen    = p[1];
		rgb[0].rgbBlue     = p[0];
	}
	else
	{
		AfxMessageBox("It is not an 8-bit or 24-bit image");
		return( -1L );
	}
	/**/
	return( nOffInImage );
}

//  设置像素(x,y)的颜色值
void SetPixel(char *pFileBuffer,int x,int y,RGBQUAD rgb)
{
	int  nColorBits   = GetColorBits(pFileBuffer);
	int  nImageHeight = GetImageHeight(pFileBuffer);
	int  nBytesPerRow = GetWidthBytes(pFileBuffer);
	/**/
	long nOffInImage  = (nImageHeight-1-y) * nBytesPerRow;
	char *p = GetDIBImageData(pFileBuffer) + nOffInImage;
	/**/
	if( nColorBits == 8 )
	{
		 p[ x ] = rgb.rgbReserved;
	}
	else if( nColorBits == 24 )
	{
		p += (3 * x);
		p[0] = rgb.rgbBlue;
		p[1] = rgb.rgbGreen;
		p[2] = rgb.rgbRed;
	}
	else
	{
		AfxMessageBox("It is not an 8-bit or 24-bit image");
	}
	/**/
	return;
}

///////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////	
// 保存为BMP文件

BOOL SaveDIB(char *pFileBuffer,CString strBmpFile)
{
	CFile hFile;
	if( !hFile.Open(strBmpFile,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary) )
	{
		AfxMessageBox("Failed to create the BMP file");
		return( FALSE );
	}
	/**/
	BITMAPFILEHEADER *pBmpHead = (BITMAPFILEHEADER *)pFileBuffer;
	long lFileSize = pBmpHead->bfSize;
	hFile.Write(pFileBuffer,lFileSize);
	hFile.Close();
	return( TRUE );
}

///////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////	
// 图像插值

/**
	 功能: 图像插值
		   nMethod  插值算法
					0 = 最临近插值法
					1 = (双)线性插值法
	 返回: 新图像的BMP文件缓冲区首地址
           NULL 表示失败（内存不足）
**/
char *ImageInterpolation(char *pBmpFileBuf,int newWidth,int newHeight,int nMethod)
{
	BITMAPFILEHEADER *pFileHeader = (BITMAPFILEHEADER *)pBmpFileBuf;
	BITMAPINFOHEADER *pDIBInfo = (BITMAPINFOHEADER *)(pBmpFileBuf + sizeof(BITMAPFILEHEADER));
//	char *pDIBData = pBmpFileBuf + pFileHeader->bfOffBits;
	int  orgWidth  = pDIBInfo->biWidth;
	int  orgHeight = pDIBInfo->biHeight;
	int  colorBits = pDIBInfo->biBitCount;
	/**/
	long bytesPerRow = 4 * ((newWidth * colorBits + 31) / 32);
	long newBmpFileSize = pFileHeader->bfOffBits + bytesPerRow * newHeight;
	char *pNewBmpFileBuf = new char [newBmpFileSize];
	memcpy(pNewBmpFileBuf, pBmpFileBuf, pFileHeader->bfOffBits);
	BITMAPFILEHEADER *pNewFileHeader = (BITMAPFILEHEADER *)pNewBmpFileBuf;
	BITMAPINFOHEADER *pNewDIBInfo = (BITMAPINFOHEADER *)(pNewBmpFileBuf + sizeof(BITMAPFILEHEADER));
	pNewFileHeader->bfSize = newBmpFileSize;
	pNewDIBInfo->biWidth = newWidth;
	pNewDIBInfo->biHeight = newHeight;
	pNewDIBInfo->biSizeImage = bytesPerRow * newHeight;
//	char *pNewDIBData = pNewBmpFileBuf + pFileHeader->bfOffBits;
	/**/
	/**/
	float xScale  = (float)orgWidth  / (float)newWidth;
	float yScale  = (float)orgHeight / (float)newHeight;
	for(int y = 0; y < newHeight; y++)
	{
		float fy = y * yScale;
		for(int x = 0; x < newWidth; x++)
		{
			RGBQUAD rgb;
			float fx = x * xScale;
			if( nMethod == 0 )		//最临近插值法
			{
				int xx = min( (int)(fx+0.5), orgWidth - 1 );
				int yy = min( (int)(fy+0.5), orgHeight - 1 );
				GetPixel(pBmpFileBuf, xx, yy, &rgb);
			}
			else
			{						//(双)线性插值法
				RGBQUAD rgbLT,rgbRT,rgbLB,rgbRB;
				int   x1 = (int)fx;
				int   x2 = min(x1+1, orgWidth-1);
				float dx = fx - (float)x1;
				int   y1 = (int)fy;
				int   y2 = min(y1+1, orgHeight-1);
				float dy = fy - (float)y1;
				GetPixel(pBmpFileBuf, x1, y1, &rgbLT);
				GetPixel(pBmpFileBuf, x2, y1, &rgbRT);
				GetPixel(pBmpFileBuf, x1, y2, &rgbLB);
				GetPixel(pBmpFileBuf, x2, y2, &rgbRB);
				for(int N = 0; N < 4; N++)
				{
					float v1 = ((BYTE *)&rgbLT)[N] + dy * (((BYTE *)&rgbLB)[N] - ((BYTE *)&rgbLT)[N]);
					float v2 = ((BYTE *)&rgbRT)[N] + dy * (((BYTE *)&rgbRB)[N] - ((BYTE *)&rgbRT)[N]);
					((BYTE *)&rgb)[N] = (int)(v1 + dx * (v2 - v1) + 0.5);
				}
			}
			SetPixel(pNewBmpFileBuf, x, y, rgb);
		}
	}
	/**/
	return( pNewBmpFileBuf );
}
//从对话框中获取高斯函数的均方差，对图像做高斯平滑
void GaussianSmooth(char* pBmpFileBuf, int MSE)
{
	float SD=sqrt(MSE);
	int BitCount = GetColorBits(pBmpFileBuf);
	if (BitCount == 8)
	{
		//处理灰度图像
		int n = int(6 * SD) - 1;//窗口宽度,一般为6*sigma-1
		int mid = (n + 1) / 2;
		//动态分配高斯核数组
		double** Gs;
		Gs = new double* [n];
		for (int i = 0; i < n; i++) Gs[i] = new double[n];
		double sum = 0;//计算和，便于归一化
		for (int i = 1; i <= n; i++)
		{
			for (int j = 1; j <= n; j++)
			{
				Gs[i - 1][j - 1] = exp(-((mid - i) * (mid - i) + (mid - j) * (mid - j)) / (double)(2 * MSE));
				sum += Gs[i - 1][j - 1];
			}
		}
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				Gs[i][j] = Gs[i][j] / sum;
			}
		}
		int imageWidth = GetImageWidth(pBmpFileBuf);
		int imageHeight = GetImageHeight(pBmpFileBuf);
		//动态分配新的图片像素储存空间,存储计算后数据
		char* NewP = new char[imageHeight * imageWidth];

		for (int w = 0; w < imageWidth; w++)
		{
			for (int h = 0; h < imageHeight; h++)
			{
				//对每个像素进行处理
				double k = 0;
				for (int i = 1; i <= n; i++)
				{
					for (int j = 1; j <= n; j++)
					{
						RGBQUAD rgb;
						int x = w + (i - mid);
						int y = h + (j - mid);
						//超出边界的点使用边界点的值
						if (x < 0 || x >= imageWidth || y < 0 || y >= imageHeight)
							GetPixel(pBmpFileBuf, w, h, &rgb, 0);
						else GetPixel(pBmpFileBuf, x, y, &rgb, 0);
						k += rgb.rgbReserved * Gs[i - 1][j - 1];
					}
				}
				NewP[h * imageWidth + w] = (int)k;
			}
		}
		for (int w = 0; w < imageWidth; w++)
		{
			for (int h = 0; h < imageHeight; h++)
			{
				//将所有像素的计算值写回原处		
				RGBQUAD rgb;
				rgb.rgbReserved = NewP[h * imageWidth + w];
				SetPixel(pBmpFileBuf, w, h, rgb);
			}
		}


		//释放空间
		for (int i = 0; i < n; i++) delete[] Gs[i];
		delete[] Gs;
		delete[] NewP;
	}
	else if (BitCount == 24)
	{
		//处理彩色图像
		int n = int(6 * SD) - 1;//窗口宽度,窗口半径为3sigma
		int mid = (n + 1) / 2;
		//动态分配高斯核数组
		double** Gs;
		Gs = new double* [n];
		for (int i = 0; i < n; i++) Gs[i] = new double[n];
		double sum = 0;//计算和，便于归一化
		for (int i = 1; i <= n; i++)
		{
			for (int j = 1; j <= n; j++)
			{
				Gs[i - 1][j - 1] = exp(-((mid - i) * (mid - i) + (mid - j) * (mid - j)) / (double)(2 * MSE));
				sum += Gs[i - 1][j - 1];
			}
		}
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				Gs[i][j] = Gs[i][j] / sum;
			}
		}
		int imageWidth = GetImageWidth(pBmpFileBuf);
		int imageHeight = GetImageHeight(pBmpFileBuf);
		//动态分配新的图片像素储存空间,存储计算后数据
		char* NewP = new char[imageHeight * imageWidth * 3];

		for (int w = 0; w < imageWidth; w++)
		{
			for (int h = 0; h < imageHeight; h++)
			{
				//对每个像素进行处理
				double k[3] = { 0,0,0 };
				for (int
					i = 1; i <= n; i++)
				{
					for (int j = 1; j <= n; j++)
					{
						RGBQUAD rgb;
						int x = w + (i - mid);
						int y = h + (j - mid);
						//超出边界的点使用中心值
						if (x < 0 || x >= imageWidth || y < 0 || y >= imageHeight)
							GetPixel(pBmpFileBuf, w, h, &rgb, 0);
						else GetPixel(pBmpFileBuf, x, y, &rgb, 0);
						k[0] += rgb.rgbRed * Gs[i - 1][j - 1];
						k[1] += rgb.rgbGreen * Gs[i - 1][j - 1];
						k[2] += rgb.rgbBlue * Gs[i - 1][j - 1];
					}
				}
				NewP[h * imageWidth * 3 + w * 3] = (int)k[2];
			}
		}

		for (int w = 0; w < imageWidth; w++)
		{
			for (int h = 0; h < imageHeight; h++)
			{
				//将所有像素的计算值写回原处		
				RGBQUAD rgb;
				rgb.rgbRed = NewP[h * imageWidth * 3 + w * 3 + 2];
				rgb.rgbGreen = NewP[h * imageWidth * 3 + w * 3 + 1];
				rgb.rgbBlue = NewP[h * imageWidth * 3 + w * 3];
				SetPixel(pBmpFileBuf, w, h, rgb);
				//printf("(%d,%d)处像素值已修改为(%d,%d,%d)\n", w, h, rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue);
			}
		}
		for (int i = 0; i < n; i++) delete[] Gs[i];
		delete[] Gs;
		delete[] NewP;

	}
	else
	{
		AfxMessageBox("It is not an 8-bit or 24-bit image");
	}
}

void MedianFilter(char* pBmpFileBuf, int SD)
{
	int BitCount = GetColorBits(pBmpFileBuf);
	if (BitCount == 8)
	{
		//处理灰度图像
		int n = 2 * SD + 1;//窗口宽度
		int mid = (n + 1) / 2;
		int imageWidth = GetImageWidth(pBmpFileBuf);
		int imageHeight = GetImageHeight(pBmpFileBuf);
		//动态分配新的图片像素储存空间,存储计算后数据
		char* NewP = new char[imageHeight * imageWidth];

		for (int w = 0; w < imageWidth; w++)
		{
			for (int h = 0; h < imageHeight; h++)
			{
				//对每个像素进行处理
				int* k = new int[n * n];
				for (int i = 1; i <= n; i++)
				{
					for (int j = 1; j <= n; j++)
					{
						RGBQUAD rgb;
						int x = w + (i - mid);
						int y = h + (j - mid);
						//超出边界的点使用边界点的值
						if (x < 0 || x >= imageWidth || y < 0 || y >= imageHeight)
							GetPixel(pBmpFileBuf, w, h, &rgb, 0);
						else GetPixel(pBmpFileBuf, x, y, &rgb, 0);
						k[(i - 1) * n + j - 1] = rgb.rgbReserved;
					}
				}
				//对窗口内的值进行排序
				for (int i = 0; i < n * n; i++)
				{
					for (int j = i + 1; j < n * n; j++)
					{
						if (k[i] > k[j])
						{
							int temp = k[i];
							k[i] = k[j];
							k[j] = temp;
						}
					}
				}
				NewP[h * imageWidth + w] = k[(n * n) / 2];
				delete[] k;
			}
		}
		for (int w = 0; w < imageWidth; w++)
		{
			for (int h = 0; h < imageHeight; h++)
			{
				//将所有像素的计算值写回原处		
				RGBQUAD rgb;
				rgb.rgbReserved = NewP[h * imageWidth + w];
				SetPixel(pBmpFileBuf, w, h, rgb);
			}
		}
		delete[] NewP;
	}
	else if (BitCount == 24)
	{
				//处理彩色图像
		int n = 2 * SD + 1;//窗口宽度
		int mid = (n + 1) / 2;
		int imageWidth = GetImageWidth(pBmpFileBuf);
		int imageHeight = GetImageHeight(pBmpFileBuf);
		//动态分配新的图片像素储存空间,存储计算后数据
		char* NewP = new char[imageHeight * imageWidth * 3];

		for (int w = 0; w < imageWidth; w++)
		{
			for (int h = 0; h < imageHeight; h++)
			{
				//对每个像素进行处理
				int* k[3] = { new int[n * n],new int[n * n],new int[n * n] };
				for (int i = 1; i <= n; i++)
				{
					for (int j = 1; j <= n; j++)
					{
						RGBQUAD rgb;
						int x = w + (i - mid);
						int y = h + (j - mid);
						//超出边界的点使用中心值
						if (x < 0 || x >= imageWidth || y < 0 || y >= imageHeight)
							GetPixel(pBmpFileBuf, w, h, &rgb, 0);
						else GetPixel(pBmpFileBuf, x, y, &rgb, 0);
						k[0][(i - 1) * n + j - 1] = rgb.rgbRed;
						k[1][(i - 1) * n + j - 1] = rgb.rgbGreen;
						k[2][(i - 1) * n + j - 1] = rgb.rgbBlue;
					}
				}
				//对窗口内的值进行排序
				for (int i = 0; i < n * n; i++)
				{
					for (int j = i + 1; j < n * n; j++)
					{
						if (k[0][i] > k[0][j])
						{
							int temp = k[0][i];
							k[0][i] = k[0][j];
						}
					}
				}
				for (int i = 0; i < n * n; i++)
				{
					for (int j = i + 1; j < n * n; j++)
					{
						if (k[1][i] > k[1][j])
						{
							int temp = k[1][i];
							k[1][i] = k[1][j];
						}
					}
				}
				for (int i = 0; i < n * n; i++)
				{
					for (int j = i + 1; j < n * n; j++)
					{
						if (k[2][i] > k[2][j])
						{
							int temp = k[2][i];
							k[2][i] = k[2][j];
						}
					}
				}
				NewP[h * imageWidth * 3 + w * 3] = k[2][(n * n) / 2];
				NewP[h * imageWidth * 3 + w * 3 + 1] = k[1][(n * n) / 2];
				NewP[h * imageWidth * 3 + w * 3 + 2] = k[0][(n * n) / 2];
				delete[] k[0];
				delete[] k[1];
				delete[] k[2];
			}
		}
		for (int w = 0; w < imageWidth; w++)
		{
			for (int h = 0; h < imageHeight; h++)
			{
				//将所有像素的计算值写回原处		
				RGBQUAD rgb;
				rgb.rgbRed = NewP[h * imageWidth * 3 + w * 3 + 2];
				rgb.rgbGreen = NewP[h * imageWidth * 3 + w * 3 + 1];
				rgb.rgbBlue = NewP[h * imageWidth * 3 + w * 3];
				SetPixel(pBmpFileBuf, w, h, rgb);
				//printf("(%d,%d)处像素值已修改为(%d,%d,%d)\n", w, h, rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue);
			}
		}
		delete[] NewP;
	}
	else
	{
		AfxMessageBox("It is not an 8-bit or 24-bit image");
	}
}

void BilateralFilter(char* pBmpFileBuf, int nWindowSize, double sigma_d, double sigma_r)
{
	int BitCount = GetColorBits(pBmpFileBuf);
	if (BitCount == 8)
	{
		//处理灰度图像
		int n = nWindowSize;//窗口宽度
		int mid = (n + 1) / 2;
		int imageWidth = GetImageWidth(pBmpFileBuf);
		int imageHeight = GetImageHeight(pBmpFileBuf);
		//动态分配新的图片像素储存空间,存储计算后数据
		char* NewP = new char[imageHeight * imageWidth];

		for (int w = 0; w < imageWidth; w++)
		{
			for (int h = 0; h < imageHeight; h++)
			{
				//对每个像素进行处理
				double k = 0;
				double sum = 0;
				for (int i = 1; i <= n; i++)
				{
					for (int j = 1; j <= n; j++)
					{
						RGBQUAD rgbxy;
						RGBQUAD rgbwh;
						int x = w + (i - mid);
						int y = h + (j - mid);
						//超出边界的点使用边界点的值
						if (x < 0 || x >= imageWidth || y < 0 || y >= imageHeight)
							GetPixel(pBmpFileBuf, w, h, &rgbxy, 0);
						else GetPixel(pBmpFileBuf, x, y, &rgbxy, 0);
						GetPixel(pBmpFileBuf, w, h, &rgbwh, 0);
						double d = sqrt((w - x) * (w - x) + (h - y) * (h - y));
						double r = abs(rgbxy.rgbReserved - rgbwh.rgbReserved);
						double weight = exp(-d * d / (2 * sigma_d * sigma_d) - r * r / (2 * sigma_r * sigma_r));
						k += rgbxy.rgbReserved * weight;
						sum += weight;
					}
				}
				NewP[h * imageWidth + w] = (int)(k / sum);
			}
		}
		for (int w = 0; w < imageWidth; w++)
		{
			for (int h = 0; h < imageHeight; h++)
			{
				//将所有像素的计算值写回原处		
				RGBQUAD rgb;
				rgb.rgbReserved = NewP[h * imageWidth + w];
				SetPixel(pBmpFileBuf, w, h, rgb);
			}
		}
		delete[] NewP;
	}
	else if (BitCount == 24) {
				//处理彩色图像
		int n = nWindowSize;//窗口宽度
		int mid = (n + 1) / 2;
		int imageWidth = GetImageWidth(pBmpFileBuf);
		int imageHeight = GetImageHeight(pBmpFileBuf);
		//动态分配新的图片像素储存空间,存储计算后数据
		char* NewP = new char[imageHeight * imageWidth * 3];

		for (int w = 0; w < imageWidth; w++)
		{
			for (int h = 0; h < imageHeight; h++)
			{
				//对每个像素进行处理
				double k[3] = { 0,0,0 };
				double sum = 0;
				for (int i = 1; i <= n; i++)
				{
					for (int j = 1; j <= n; j++)
					{
						RGBQUAD rgb;
						int x = w + (i - mid);
						int y = h + (j - mid);
						//超出边界的点使用中心值
						if (x < 0 || x >= imageWidth || y < 0 || y >= imageHeight)
							GetPixel(pBmpFileBuf, w, h, &rgb, 0);
						else GetPixel(pBmpFileBuf, x, y, &rgb, 0);
						double d = sqrt((w - x) * (w - x) + (h - y) * (h - y));
						double r = sqrt((rgb.rgbRed - GetPixel(pBmpFileBuf, w, h, &rgb, 0)) * (rgb.rgbRed - GetPixel(pBmpFileBuf, w, h, &rgb, 0)) +
													(rgb.rgbGreen - GetPixel(pBmpFileBuf, w, h, &rgb, 0)) * (rgb.rgbGreen - GetPixel(pBmpFileBuf, w, h, &rgb, 0)) +
													(rgb.rgbBlue - GetPixel(pBmpFileBuf, w, h, &rgb, 0)) * (rgb.rgbBlue - GetPixel(pBmpFileBuf, w, h, &rgb, 0
													)));
						double weight = exp(-d * d / (2 * sigma_d * sigma_d) - r * r / (2 * sigma_r * sigma_r));
						k[0] += rgb.rgbRed * weight;
						k[1] += rgb.rgbGreen * weight;
						k[2] += rgb.rgbBlue * weight;
						sum += weight;
					}
				}
				NewP[h * imageWidth * 3 + w * 3] = (int)(k[2] / sum);
				NewP[h * imageWidth * 3 + w * 3 + 1] = (int)(k[1] / sum);
				NewP[h * imageWidth * 3 + w * 3 + 2] = (int)(k[0] / sum);
			}
		}
		for (int w = 0; w < imageWidth; w++)
		{
			for (int h = 0; h < imageHeight; h++)
			{
				//将所有像素的计算值写回原处		
				RGBQUAD rgb;
				rgb.rgbRed = NewP[h * imageWidth * 3 + w * 3 + 2];
				rgb.rgbGreen = NewP[h * imageWidth * 3 + w * 3 + 1];
				rgb.rgbBlue = NewP[h * imageWidth * 3 + w * 3];
				SetPixel(pBmpFileBuf, w, h, rgb);
				//printf("(%d,%d)处像素值已修改为(%d,%d,%d)\n", w, h, rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue);
			}
		}
		delete[] NewP;
	}
	else
	{
		AfxMessageBox("It is not an 8-bit or 24-bit image");
	}

}

void Histoequalization(char* pBmpFileBuf)
{
	int BitCount = GetColorBits(pBmpFileBuf);
	if (BitCount == 8)
	{
		//处理灰度图像
		int imageWidth = GetImageWidth(pBmpFileBuf);
		int imageHeight = GetImageHeight(pBmpFileBuf);
		int nBytesPerRow = GetWidthBytes(pBmpFileBuf);
		int nTotalColors = 256;
		int nUsedColors = GetUsedColors(pBmpFileBuf);
		int nPixelsPerRow = GetImageWidth(pBmpFileBuf);
		//动态分配直方图数组
		int* hist = new int[nTotalColors];
		for (int i = 0; i < nTotalColors; i++) hist[i] = 0;
		//计算直方图
		for (int h = 0; h < imageHeight; h++)
		{
			for (int w = 0; w < imageWidth; w++)
			{
				RGBQUAD rgb;
				GetPixel(pBmpFileBuf, w, h, &rgb);
				hist[rgb.rgbReserved]++;
			}
		}
		//计算累积直方图
		int* hist_sum = new int[nTotalColors];
		hist_sum[0] = hist[0];
		for (int i = 1; i < nTotalColors; i++)
		{
			hist_sum[i] = hist_sum[i - 1] + hist[i];
		}
		//计算映射表
		int* map = new int[nTotalColors];
		for (int i = 0; i < nTotalColors; i++)
		{
			map[i] = (int)(255.0 * hist_sum[i] / (imageWidth * imageHeight) + 0.5);
		}
		//映射
		for (int h = 0; h < imageHeight; h++)
		{
			for (int w = 0; w < imageWidth; w++)
			{
				RGBQUAD rgb;
				GetPixel(pBmpFileBuf, w, h, &rgb);
				rgb.rgbReserved = map[rgb.rgbReserved];
				SetPixel(pBmpFileBuf, w, h, rgb);

			}
		}
		delete[] hist;
		delete[] hist_sum;
		delete[] map;

	}
	else if (BitCount == 24)
	{
				//处理彩色图像
		int imageWidth = GetImageWidth(pBmpFileBuf);
		int imageHeight = GetImageHeight(pBmpFileBuf);
		int nBytesPerRow = GetWidthBytes(pBmpFileBuf);
		int nTotalColors = 256;
		int nUsedColors = GetUsedColors(pBmpFileBuf);
		int nPixelsPerRow = GetImageWidth(pBmpFileBuf);
		//动态分配直方图数组
		int* hist[3] = { new int[nTotalColors],new int[nTotalColors],new int[nTotalColors] };
		for (int i = 0; i < nTotalColors; i++) hist[0][i] = hist[1][i] = hist[2][i] = 0;
		//计算直方图
		for (int h = 0; h < imageHeight; h++)
		{
			for (int w = 0; w < imageWidth; w++)
			{
				RGBQUAD rgb;
				GetPixel(pBmpFileBuf, w, h, &rgb);
				hist[0][rgb.rgbRed]++;
				hist[1][rgb.rgbGreen]++;
				hist[2][rgb.rgbBlue]++;
			}
		}
		//计算累积直方图
		int* hist_sum[3] = { new int[nTotalColors],new int[nTotalColors],new int[nTotalColors] };
		hist_sum[0][0] = hist[0][0];
		hist_sum[1][0] = hist[1][0];
		hist_sum[2][0] = hist[2][0];
		for (int i = 1; i < nTotalColors; i++)
		{
			hist_sum[0][i] = hist_sum[0][i - 1] + hist[0][i];
			hist_sum[1][i] = hist_sum[1][i - 1] + hist[1][i];
			hist_sum[2][i] = hist_sum[2][i - 1] + hist[2][i];
		}
		//计算映射表
		int* map[3] = { new int[nTotalColors],new int[nTotalColors],new int[nTotalColors] };
		for (int i = 0; i < nTotalColors; i++)
		{
			map[0][i] = (int)(255.0 * hist_sum[0][i] / (imageWidth * imageHeight) + 0.5);
			map[1][i] = (int)(255.0 * hist_sum[1][i] / (imageWidth * imageHeight) + 0.5);
			map[2][i] = (int)(255.0 * hist_sum[2][i] / (imageWidth * imageHeight) + 0.5);
		}
		//映射
		for (int h = 0; h < imageHeight; h++)
		{
			for (int w = 0; w < imageWidth; w++)
			{
				RGBQUAD rgb;
				GetPixel(pBmpFileBuf, w, h, &rgb);
				rgb.rgbRed = map[2][rgb.rgbRed];
				rgb.rgbGreen = map[1][rgb.rgbGreen];
				rgb.rgbBlue = map[0][rgb.rgbBlue];
				SetPixel(pBmpFileBuf, w, h, rgb);

			}
		}
		delete[] hist[0];
		delete[] hist[1];
		delete[] hist[2];
		delete[] hist_sum[0];
		delete[] hist_sum[1];
		delete[] hist_sum[2];
		delete[] map[0];
		delete[] map[1];
		delete[] map[2];
	}
	else
		{
		AfxMessageBox("It is not an 8-bit or 24-bit image");
	}
}

void SharpenGrad(char* pBmpFileBuf, float sharpness)
{
	int BitCount = GetColorBits(pBmpFileBuf);
	//int Laplacian[3][3] = { {0,-1,0},{-1,4,-1},{0,-1,0} };
	if (BitCount == 8)
	{
	//处理灰度图像
	int imageWidth = GetImageWidth(pBmpFileBuf);
	int imageHeight = GetImageHeight(pBmpFileBuf);
	//动态分配新的图片像素储存空间,存储计算后数据
	char* NewP = new char[imageHeight * imageWidth];

	for (int w = 0; w < imageWidth; w++)
	{
		for (int h = 0; h < imageHeight; h++)
		{
			//对每个像素进行处理
			RGBQUAD rgb;
			int k = 0;
			GetPixel(pBmpFileBuf, w, h, &rgb, 0);
			k = rgb.rgbReserved;
			for (int i = -1; i <= 1; i++)
			{
				for (int j = -1; j <= 1; j++)
				{
					RGBQUAD rgb;
					int x = w + i;
					int y = h + j;
					//超出边界的点使用中心值
					if (x < 0 || x >= imageWidth || y < 0 || y >= imageHeight)
						GetPixel(pBmpFileBuf, w, h, &rgb, 0);
					else GetPixel(pBmpFileBuf, x, y, &rgb, 0);
					if (i == 0 && j == 0)
					{
						k += int(sharpness* 4 * rgb.rgbReserved);
					}
					else
					{
						k -= int(sharpness*rgb.rgbReserved);
					}
				}
			}
			NewP[h * imageWidth + w] = min(k,255) ;
		}
	}
	for (int w = 0; w < imageWidth; w++)
	{
		for (int h = 0; h < imageHeight; h++)
		{
			//将所有像素的计算值写回原处		
			RGBQUAD rgb;
			rgb.rgbReserved = NewP[h * imageWidth + w];
			SetPixel(pBmpFileBuf, w, h, rgb);
		}
	}
	delete[] NewP;
	}
	else if (BitCount == 24) {
				//处理彩色图像
		int imageWidth = GetImageWidth(pBmpFileBuf);
		int imageHeight = GetImageHeight(pBmpFileBuf);
		//动态分配新的图片像素储存空间,存储计算后数据
		char* NewP = new char[imageHeight * imageWidth * 3];

		for (int w = 0; w < imageWidth; w++)
		{
			for (int h = 0; h < imageHeight; h++)
			{
				//对每个像素进行处理
				int k[3] = { 0,0,0 };
				int sum = 0;
				for (int i = -1; i <= 1; i++)
				{
					for (int j = -1; j <= 1; j++)
					{
						RGBQUAD rgb;
						int x = w + i;
						int y = h + j;
						//超出边界的点使用中心值
						if (x < 0 || x >= imageWidth || y < 0 || y >= imageHeight)
							GetPixel(pBmpFileBuf, w, h, &rgb, 0);
						else GetPixel(pBmpFileBuf, x, y, &rgb, 0);
						if (i == 0 && j == 0)
						{
							k[0] += 9 * rgb.rgbRed;
							k[1] += 9 * rgb.rgbGreen;
							k[2] += 9 * rgb.rgbBlue;
						}
						else
						{
							k[0] -= rgb.rgbRed;
							k[1] -= rgb.rgbGreen;
							k[2] -= rgb.rgbBlue;
						}
					}
				}
				NewP[h * imageWidth * 3 + w * 3] = (int)(k[2] / sharpness);
				NewP[h * imageWidth * 3 + w * 3 + 1] = (int)(k[1] / sharpness);
				NewP[h * imageWidth * 3 + w * 3 + 2] = (int)(k[0] / sharpness);
			}
		}
		for (int w = 0; w < imageWidth; w++)
		{
			for (int h = 0; h < imageHeight; h++)
			{
				//将所有像素的计算值写回原处		
				RGBQUAD rgb;
				rgb.rgbRed = NewP[h * imageWidth * 3 + w * 3 + 2];
				rgb.rgbGreen = NewP[h * imageWidth * 3 + w * 3 + 1];
				rgb.rgbBlue = NewP[h * imageWidth * 3 + w * 3];
				SetPixel(pBmpFileBuf, w, h, rgb);
				//printf("(%d,%d)处像素值已修改为(%d,%d,%d)\n", w, h, rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue);
			}
		}
		delete[] NewP;
	}
	else
	{
		AfxMessageBox("It is not an 8-bit or 24-bit image");
	}
}

void CannyEdge(char* pFileBuf, int lowThreshold, int highThreshold,int Guass_sigma)
{
	//1.高斯滤波，调用之前的函数
	GaussianSmooth(pFileBuf, Guass_sigma);
	//2.使用Sobel算子计算像素梯度
	int BitCount = GetColorBits(pFileBuf);
	int imageWidth = GetImageWidth(pFileBuf);
	int imageHeight = GetImageHeight(pFileBuf);
	if (BitCount == 8)
	{
		//处理灰度图像
		//soble算子
		int soble_x[3][3] = { -1,0,1,-2,0,2,-1,0,1 };
		int soble_y[3][3] = { 1,2,1,0,0,0,-1,-2,-1 };
		float pi = 3.1415926;
		//动态分配梯度储存数组
		int* Grad_x = new int[imageHeight * imageWidth];
		int* Grad_y = new int[imageHeight * imageWidth];
		double* G = new double[imageWidth * imageHeight];
		//梯度方向数组，1,2,3,4分别代表—,|,/,\四个方向
		int* D = new int[imageWidth * imageHeight];
		//动态分配新的图片像素储存空间,存储计算后数据
		char* NewP = new char[imageHeight * imageWidth];
		//计算梯度
		for (int w = 0; w < imageWidth; w++)
		{
			for (int h = 0; h < imageHeight; h++)
			{
				//对每个像素进行处理
				int Gx = 0;
				int Gy = 0;
				for (int i = 1; i <= 3; i++)
				{
					for (int j = 1; j <= 3; j++)
					{
						RGBQUAD rgb;
						int x = w + (i - 2);
						int y = h + (j - 2);
						//超出边界的点使用中心值
						if (x < 0 || x >= imageWidth || y < 0 || y >= imageHeight)
							GetPixel(pFileBuf, w, h, &rgb, 0);
						else GetPixel(pFileBuf, x, y, &rgb, 0);
						Gx += rgb.rgbReserved * soble_x[i - 1][j - 1];
						Gy += rgb.rgbReserved * soble_y[i - 1][j - 1];
					}
				}
				//printf("x，y方向梯度为%d，%d\n", Gx, Gy);
				Grad_x[h * imageWidth + w] = Gx;
				Grad_y[h * imageWidth + w] = Gy;
				G[h * imageWidth + w] = sqrt(Gx * Gx + Gy * Gy);
				double t=atan2(Gy, Gx);
				//printf("角度为%lf\n", t);
				if ((t >= -pi / 8 && t < pi / 8) || (t < -pi * 7 / 8) || (t >= pi * 7 / 8))
					D[h * imageWidth + w] = 1;
				else if ((t >= pi * 3 / 8 && t < pi * 5 / 8) || (t >= -pi * 5 / 8 && t < -pi * 3 / 8))
					D[h * imageWidth + w] = 2;
				else if ((t >= pi / 8 && t < pi * 3 / 8) || (t >= -pi * 7 / 8 && t < -pi * 5 / 8))
					D[h * imageWidth + w] = 3;
				else if ((t >= pi * 5 / 8 && t < pi * 7 / 8) || (t >= -pi * 3 / 8 && t < -pi / 8))
					D[h * imageWidth + w] = 4;
				else
				{
					AfxMessageBox("梯度方向计算错误");
				}
			}
		}
		for (int w = 0; w < imageWidth; w++)
		{
			for (int h = 0; h < imageHeight; h++)
			{
				//进行非极大抑制		
				switch (D[h * imageWidth + w])
				{
				case 1:
					//若超出边界不予处理
					if (w - 1 < 0 || w + 1 >= imageWidth) break;
					//若为极大值则保留，否则置为0
					if (G[h * imageWidth + w - 1] <= G[h * imageWidth + w] &&
						G[h * imageWidth + w + 1] <= G[h * imageWidth + w])
						NewP[h * imageWidth + w] = G[h * imageWidth + w];
					else NewP[h * imageWidth + w] = 0;
					break;
				case 2:
					if (h - 1 < 0 || h + 1 >= imageWidth) break;
					if (G[(h - 1) * imageWidth + w] <= G[h * imageWidth + w] &&
						G[(h + 1) * imageWidth + w] <= G[h * imageWidth + w])
						NewP[h * imageWidth + w] = G[h * imageWidth + w];
					else NewP[h * imageWidth + w] = 0;
					break;
				case 3:
					if (w - 1 < 0 || w + 1 >= imageWidth) break;
					if (h - 1 < 0 || h + 1 >= imageWidth) break;
					if (G[(h + 1) * imageWidth + w - 1] <= G[h * imageWidth + w] &&
						G[(h - 1) * imageWidth + w + 1] <= G[h * imageWidth + w])
						NewP[h * imageWidth + w] = G[h * imageWidth + w];
					else NewP[h * imageWidth + w] = 0;
					break;
				case 4:
					if (w - 1 < 0 || w + 1 >= imageWidth) break;
					if (h - 1 < 0 || h + 1 >= imageWidth) break;
					if (G[(h - 1) * imageWidth + w - 1] <= G[h * imageWidth + w] &&
						G[(h + 1) * imageWidth + w + 1] <= G[h * imageWidth + w])
						NewP[h * imageWidth + w] = G[h * imageWidth + w];
					else NewP[h * imageWidth + w] = 0;
					break;
				default:
					NewP[h * imageWidth + w] = 0;
				}
			}
		}
		for (int w = 0; w < imageWidth; w++)
		{
			for (int h = 0; h < imageHeight; h++)
			{
				//将所有像素的计算值写回原处		
				RGBQUAD rgb;
				if (NewP[h * imageWidth + w] > 255) rgb.rgbReserved = 255;
				else rgb.rgbReserved = NewP[h * imageWidth + w];
				SetPixel(pFileBuf, w, h, rgb);
				//printf("(%d,%d)处像素值已修改为%d\n", w, h, rgb.rgbReserved);
			}
		}

		//释放空间
		delete[] Grad_x;
		delete[] Grad_y;
		delete[] G;
		delete[] D;
		delete[] NewP;
	}
	else if (BitCount == 24)
	{
		//处理彩色图像
		AfxMessageBox("彩色图像处理有待实现");
	}
	else
	{
		AfxMessageBox("It is not an 8-bit or 24-bit image");
	}
}

//Otsu算法
void OtsuSegment(char* pFileBuf)
{
	int BitCount = GetColorBits(pFileBuf);
	if (BitCount == 8)
	{
		//处理灰度图像
		int imageWidth = GetImageWidth(pFileBuf);
		int imageHeight = GetImageHeight(pFileBuf);
		int nBytesPerRow = GetWidthBytes(pFileBuf);
		int nTotalColors = 256;
		int nUsedColors = GetUsedColors(pFileBuf);
		int nPixelsPerRow = GetImageWidth(pFileBuf);
		//动态分配直方图数组
		int* hist = new int[nTotalColors];
		for (int i = 0; i < nTotalColors; i++) hist[i] = 0;
		//计算直方图
		for (int h = 0; h < imageHeight; h++)
		{
			for (int w = 0; w < imageWidth; w++)
			{
				RGBQUAD rgb;
				GetPixel(pFileBuf, w, h, &rgb);
				hist[rgb.rgbReserved]++;
			}
		}
		//计算累积直方图
		int* hist_sum = new int[nTotalColors];
		hist_sum[0] = hist[0];
		for (int i = 1; i < nTotalColors; i++)
		{
			hist_sum[i] = hist_sum[i - 1] + hist[i];
		}
		//计算映射表
		int* map = new int[nTotalColors];
		for (int i = 0; i < nTotalColors; i++)
		{
			map[i] = (int)(255.0 * hist_sum[i] / (imageWidth * imageHeight) + 0.5);
		}
		//计算类间方差
		double* interclass_variance = new double[nTotalColors];
		for (int i = 0; i < nTotalColors; i++)
		{
			double w0 = double(hist_sum[i]) / (imageWidth * imageHeight);
			double w1 = 1 - w0;
			if (w0 == 0 || w1 == 0)
			{
				interclass_variance[i] = 0;
				//AfxMessageBox("w0或w1为0");
				continue;
			}
			double u0 = 0;
			double u1 = 0;
			for (int j = 0; j < i; j++)
			{
				u0 += j * hist[j];

			}
			u0 /= hist_sum[i];
			for (int j = i; j < nTotalColors; j++)
			{
				u1 += j * hist[j];
			}

			u1 /= (imageWidth * imageHeight - hist_sum[i]);

			/*CString message;
			message.Format("最佳阈值为%f,%f", u0,u1);
			AfxMessageBox(message);*/
			interclass_variance[i] = w0 * w1 * (u0 - u1) * (u0 - u1);

		}
		//找到最大类间方差对应的阈值
		int threshold = 0;
		double max = -99999;
		for (int i = 1; i < nTotalColors; i++)
		{
			if (interclass_variance[i] > max)
			{
				max = interclass_variance[i];
				threshold = i;
			}
		}
		CString message;
		message.Format("最佳阈值为%d", threshold);
		AfxMessageBox(message);
		//映射
		for (int h = 0; h < imageHeight; h++)
		{
			for (int w = 0; w < imageWidth; w++)
			{
				RGBQUAD rgb;
				GetPixel(pFileBuf, w, h, &rgb);
				if (rgb.rgbReserved > threshold)
					rgb.rgbReserved = 255;
				else rgb.rgbReserved = 0;
				SetPixel(pFileBuf, w, h, rgb);
			}
		}
		delete[] hist;
		delete[] hist_sum;
		delete[] map;
		delete[] interclass_variance;

	}
	else if (BitCount == 24)
	{
		//处理彩色图像
		AfxMessageBox("彩色图像处理有待实现");
	}
	else
	{
		AfxMessageBox("It is not an 8-bit or 24-bit image");
	}
}