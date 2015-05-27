
// cv1Doc.cpp : Ccv1Doc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "cv1.h"
#include "ChildFrm.h"
#endif

#include "cv1Doc.h"

#include <propkey.h>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Ccv1Doc

IMPLEMENT_DYNCREATE(Ccv1Doc, CDocument)

BEGIN_MESSAGE_MAP(Ccv1Doc, CDocument)
END_MESSAGE_MAP()

// Ccv1Doc 생성/소멸

Ccv1Doc::Ccv1Doc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	height = width = MAX_SIZE;
}

Ccv1Doc::~Ccv1Doc()
{
}

BOOL Ccv1Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// Ccv1Doc serialization

void Ccv1Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
		ar.Write(outImg, MAX_SIZE * MAX_SIZE);
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
		CFile *infile = ar.GetFile();
		if (infile->GetLength() > MAX_SIZE * MAX_SIZE)
		{
			AfxMessageBox(L"파일의 크기가 5120x5120보다 크다!");
			return;
		}
		//ar.Read(inImg, infile->GetLength());
		ar.Read(img, infile->GetLength());
	}
}

void Ccv1Doc::newDocument(CString orgName, CString processName)
{
	CMultiDocTemplate *pTemplate = (CMultiDocTemplate*)GetDocTemplate();
	Ccv1Doc *pDoc = (Ccv1Doc*)pTemplate->CreateNewDocument();
	pDoc->SetTitle(orgName + "-" + processName);
	for (int i = 0; i < height * width; i++)
	{
		pDoc->img[i] = dstimg[i];
	}
	CChildFrame *pFrm = (CChildFrame*)pTemplate->CreateNewFrame(pDoc, NULL);
	pFrm->InitialUpdateFrame(pDoc, TRUE);
	pDoc->UpdateAllViews(NULL);
}

void Ccv1Doc::onReverseImg()
{
	std::vector<unsigned int> v1(MAX_SIZE * MAX_SIZE);
	for (int i = 0; i < MAX_SIZE*MAX_SIZE; i++)
	{
		v1[i] = img[i];
	}
	std::vector<unsigned int> v2(MAX_SIZE * MAX_SIZE);

	array_view<unsigned int, 2> avImg(MAX_SIZE, MAX_SIZE, v1);
	array_view<unsigned int, 2> avDstImg(MAX_SIZE, MAX_SIZE, v2);
	
	parallel_for_each(avDstImg.extent, [=](index<2> idx) restrict(amp)
	{
		avDstImg[idx] = 255 - avImg[idx];
	});
	avDstImg.synchronize();
	for (int i = 0; i < MAX_SIZE*MAX_SIZE; i++)
	{
		dstimg[i] = v2[i];
	}
	newDocument(this->GetTitle(), L"Reverse Image");
}

void Ccv1Doc::OnConstADD()
{	
	/*std::vector<unsigned int> v1(MAX_SIZE * MAX_SIZE);
	for (int i = 0; i < MAX_SIZE*MAX_SIZE; i++)
	{
		v1[i] = img[i];
	}
	std::vector<unsigned int> v2(MAX_SIZE * MAX_SIZE);

	array_view<unsigned int, 2> avImg(MAX_SIZE, MAX_SIZE, v1);
	array_view<unsigned int, 2> avDstImg(MAX_SIZE, MAX_SIZE, v2);

	parallel_for_each(avDstImg.extent, [=](index<2> idx) restrict(amp)
	{
		int temp = (int)(avImg[idx] + 60);
		avDstImg[idx] = temp > 255 ? 255 : temp;
	});
	avDstImg.synchronize();
	for (int i = 0; i < MAX_SIZE*MAX_SIZE; i++)
	{
		dstimg[i] = v2[i];
	}*/
	DWORD start = timeGetTime();
	DWORD end = 0;
	for (int i = 0; i < MAX_SIZE * MAX_SIZE; i++)
	{
		dstimg[i] = img[i] + 60;
	}
	end = timeGetTime();
	AllocConsole();
	_cprintf("RunningTime : %d sec", (end - start));
	newDocument(L"20095208_이창희", L"60더하기 연산");
}
void Ccv1Doc::OnLUTADD()
{
	DWORD start = timeGetTime();
	DWORD end = 0;
	std::vector<unsigned int> lut(256);

	std::vector<unsigned int> v1(MAX_SIZE * MAX_SIZE);
	for (int i = 0; i < MAX_SIZE*MAX_SIZE; i++)
	{
		v1[i] = img[i];
	}
	std::vector<unsigned int> v2(MAX_SIZE * MAX_SIZE);
	array_view<unsigned int, 1> avLUT(256, lut);
	array_view<unsigned int, 2> avImg(MAX_SIZE, MAX_SIZE, v1);
	array_view<unsigned int, 2> avDstImg(MAX_SIZE, MAX_SIZE, v2);

	parallel_for_each(avLUT.extent, [=](index<1> idx) restrict(amp)
	{
		int temp = (int)((float)idx[0] + 60);
		avLUT[idx] = temp > 255 ? 255 : temp;
	});

	parallel_for_each(avDstImg.extent, [=](index<2> idx) restrict(amp)
	{
		avDstImg[idx] = avLUT[avImg[idx]];
	});
	avDstImg.synchronize();
	for (int i = 0; i < MAX_SIZE*MAX_SIZE; i++)
	{
		dstimg[i] = v2[i];
	}
	end = timeGetTime();
	AllocConsole();
	_cprintf("RunningTime : %d sec", (end - start));
	newDocument(L"20095208_이창희", L"60더하기 연산");
}
void Ccv1Doc::OnLUTSUB()
{
	std::vector<unsigned int> lut(256);

	std::vector<unsigned int> v1(MAX_SIZE * MAX_SIZE);
	for (int i = 0; i < MAX_SIZE*MAX_SIZE; i++)
	{
		v1[i] = img[i];
	}
	std::vector<unsigned int> v2(MAX_SIZE * MAX_SIZE);
	array_view<unsigned int, 1> avLUT(256, lut);
	array_view<unsigned int, 2> avImg(MAX_SIZE, MAX_SIZE, v1);
	array_view<unsigned int, 2> avDstImg(MAX_SIZE, MAX_SIZE, v2);

	parallel_for_each(avLUT.extent, [=](index<1> idx) restrict(amp)
	{
		int temp = (int)((float)idx[0] - 60);
		avLUT[idx] = temp > 255 ? 255 : temp;
	});

	parallel_for_each(avDstImg.extent, [=](index<2> idx) restrict(amp)
	{
		avDstImg[idx] = avLUT[avImg[idx]];
	});
	avDstImg.synchronize();
	for (int i = 0; i < MAX_SIZE*MAX_SIZE; i++)
	{
		dstimg[i] = v2[i];
	}
	newDocument(L"20095208_이창희", L"60빼기 연산");
}
void Ccv1Doc::OnLUTMUL()
{
	std::vector<unsigned int> lut(256);

	std::vector<unsigned int> v1(MAX_SIZE * MAX_SIZE);
	for (int i = 0; i < MAX_SIZE*MAX_SIZE; i++)
	{
		v1[i] = img[i];
	}
	std::vector<unsigned int> v2(MAX_SIZE * MAX_SIZE);
	array_view<unsigned int, 1> avLUT(256, lut);
	array_view<unsigned int, 2> avImg(MAX_SIZE, MAX_SIZE, v1);
	array_view<unsigned int, 2> avDstImg(MAX_SIZE, MAX_SIZE, v2);

	parallel_for_each(avLUT.extent, [=](index<1> idx) restrict(amp)
	{
		int temp = (int)((float)idx[0] * 60);
		avLUT[idx] = temp > 255 ? 255 : temp;
	});

	parallel_for_each(avDstImg.extent, [=](index<2> idx) restrict(amp)
	{
		avDstImg[idx] = avLUT[avImg[idx]];
	});
	avDstImg.synchronize();
	for (int i = 0; i < MAX_SIZE*MAX_SIZE; i++)
	{
		dstimg[i] = v2[i];
	}
	newDocument(L"20095208_이창희", L"1.4 곱하기 연산");
}
void Ccv1Doc::OnLUTDIV()
{
	std::vector<unsigned int> lut(256);

	std::vector<unsigned int> v1(MAX_SIZE * MAX_SIZE);
	for (int i = 0; i < MAX_SIZE*MAX_SIZE; i++)
	{
		v1[i] = img[i];
	}
	std::vector<unsigned int> v2(MAX_SIZE * MAX_SIZE);
	array_view<unsigned int, 1> avLUT(256, lut);
	array_view<unsigned int, 2> avImg(MAX_SIZE, MAX_SIZE, v1);
	array_view<unsigned int, 2> avDstImg(MAX_SIZE, MAX_SIZE, v2);

	parallel_for_each(avLUT.extent, [=](index<1> idx) restrict(amp)
	{
		int temp = (int)((float)idx[0] / 60);
		avLUT[idx] = temp > 255 ? 255 : temp;
	});

	parallel_for_each(avDstImg.extent, [=](index<2> idx) restrict(amp)
	{
		avDstImg[idx] = avLUT[avImg[idx]];
	});
	avDstImg.synchronize();
	for (int i = 0; i < MAX_SIZE*MAX_SIZE; i++)
	{
		dstimg[i] = v2[i];
	}
	newDocument(L"20095208_이창희", L"1.4 나누기 연산");
}

void Ccv1Doc::OnPosterizing(int level)
{
	CString title;
	CString my_name;
	int step = 256 / level;
	int inc = 255 / (level - 1);

	std::vector<unsigned int> lut(256);
	std::vector<unsigned int> v1(MAX_SIZE * MAX_SIZE);

	for (int i = 0; i < MAX_SIZE*MAX_SIZE; i++)
	{
		v1[i] = img[i];
	}
	std::vector<unsigned int> v2(MAX_SIZE * MAX_SIZE);
	array_view<unsigned int, 1> avLUT(256, lut);
	array_view<unsigned int, 2> avImg(MAX_SIZE, MAX_SIZE, v1);
	array_view<unsigned int, 2> avDstImg(MAX_SIZE, MAX_SIZE, v2);

	parallel_for_each(avLUT.extent, [=](index<1> idx) restrict(amp)
	{
		avLUT[idx] = (int)(idx[0] / step) * inc;
	});

	parallel_for_each(avDstImg.extent, [=](index<2> idx) restrict(amp)
	{
		avDstImg[idx] = avLUT[avImg[idx]];
	});
	avDstImg.synchronize();
	for (int i = 0; i < MAX_SIZE*MAX_SIZE; i++)
	{
		dstimg[i] = v2[i];
	}
	my_name = L"20095208_이창희";
	title.Format(L"%d level Posterizing", level);
	newDocument(my_name, title);
}

void Ccv1Doc::OnHistogram(int in_height, int in_width)
{
	/*int gv, vmax, vmin;
	unsigned int LUT[256];
	CString title;
	CString my_name;

	ZeroMemory(m_histoarr, sizeof(m_histoarr));

	for (int i = 0; i < in_height; i++)
	{
		for (int j = 0; j < in_width; j++)
		{
			gv = (int)img[i][j];
			m_histoarr[gv]++;		//밝기에 따른 히스토그램 voting
		}
	}

	vmin = 1000000;
	vmax = 0;

	for(int i = 0; i < 256; i++)
	{
		if (m_histoarr[i] <= vmin)
		{
			vmin = m_histoarr[i];
		}
		if(m_histoarr[i] >= vmax)
		{
			vmax = m_histoarr[i];
		}
	}
	if (vmax == vmin)
		return;

	float vd = (float)(vmax - vmin);
	for (int i = 0; i < 256; i++)
	{
		m_histoarr[i] = (int)(((float)m_histoarr[i] - vmin)*255.f / vd);
	}
	memset(dstimg, 255, sizeof(dstimg));
	//for (int i = 0; i < in_height; i++)
	//{
	//	for (int j = 0; j < in_width; j++)
	//	{
	//		dstimg[i][j] = 255;
	//	}
	//}

	for (int j = 0; j < width; j++)
	{
		for (int i = 0; i < m_histoarr[j]; i++)
		{
			dstimg[255 - i][j] = 0;
		}
	}
	my_name = L"20095208_이창희";
	title.Format(L"Histogram");
	newDocument(my_name, title);*/
}
void Ccv1Doc::OnHistequal(int in_height, int in_width)
{
/*	CString title;
	CString my_name;
	unsigned int *histogram = new unsigned int[256];
	unsigned int *sum_hist = new unsigned int[256];

	ZeroMemory(histogram, sizeof(histogram));

	for (int i = 0; i < in_height; i++)
	{
		for (int j = 0; j < in_width; j++)
		{
			histogram[img[i][j]]++;
		}
	}

	int sum = 0;
	float scale_factor = 255.f / (float)(in_height * in_width);

	for (int i = 0; i < 256; i++)
	{
		sum += histogram[i];
		sum_hist[i] = (int)((sum * scale_factor) + 0.5);
	}

	for (int i = 0; i < in_height; i++)
	{
		for (int j = 0; j < in_width; j++)
		{
			dstimg[i][j] = sum_hist[img[i][j]];
		}
	}
	my_name = L"20095208_이창희";
	title.Format(L"Histogram Equal");
	newDocument(my_name, title);*/
}
void Ccv1Doc::OnHistStretch(int in_height, int in_width)
{
/*	CString title;
	CString my_name;
	int lowvalue = 255, highvalue = 0;
	for (int i = 0; i < in_height; i++)
	{
		for (int j = 0; j < in_width; j++)
		{
			if (img[i][j] < lowvalue)
			{
				lowvalue = img[i][j];
			}
		}
	}
	for (int i = 0; i < in_height; i++)
	{
		for (int j = 0; j < in_width; j++)
		{
			if (img[i][j] > highvalue)
			{
				highvalue = img[i][j];
			}
		}
	}
	//Histogram 스트레칭 계산
	float mult = 255.f / (float)(highvalue - lowvalue);
	for (int i = 0; i < in_height; i++)
	{
		for (int j = 0; j < in_width; j++)
		{
			dstimg[i][j] = (unsigned char)((img[i][j] - lowvalue) * mult);
		}
	}
	my_name = L"20095208_이창희";
	title.Format(L"Histogram Stretch");
	newDocument(my_name, title);*/
}
void Ccv1Doc::OnHistUpStretch(int in_height, int in_width, int lowpercent, int highpercent)
{
/*	CString title;
	CString my_name;
	unsigned int *histogram = new unsigned int[256];

	//ZeroMemory(histogram, sizeof(histogram));
	for (int i = 0; i < 256; i++)
	{
		histogram[i] = 0;
	}
	for (int i = 0; i < in_height; i++)
	{
		for (int j = 0; j < in_width; j++)
		{
			histogram[img[i][j]]++;
		}
	}
	unsigned int runsum = 0;
	int lowthresh = 0;
	int highthresh = 255;
	for (int i = 0; i < 256; i++)
	{
		runsum += histogram[i];
		if ((runsum * 100.0 / (float)(in_height * in_width)) >= lowpercent)
		{
			lowthresh = i;
			break;
		}
	}
	runsum = 0;
	for (int i = 255; i >= 0; i--)
	{
		runsum += histogram[i];
		if ((runsum * 100.0 / (float)(in_height * in_width)) >= highpercent)
		{
			highthresh = i;
			break;
		}
	}

	unsigned char *LUT = new unsigned char[256];

	for (int i = 0; i < lowthresh; i++)
	{
		LUT[i] = 0;
	}
	for (int i = 255; i > highthresh; i--)
	{
		LUT[i] = 255;
	}
	float scale = 255.f / (float)(highthresh - lowthresh);

	for (int i = lowthresh; i <= highthresh; i++)
	{
		LUT[i] = (unsigned char)((i - lowthresh) * scale);
	}

	for (int i = 0; i < in_height; i++)
	{
		for (int j = 0; j < in_width; j++)
		{
			dstimg[i][j] = LUT[img[i][j]];
		}
	}
	delete[] histogram;
	delete[] LUT;
	my_name = L"20095208_이창희";
	title.Format(L"Histogram UpStretch");
	newDocument(my_name, title);*/
}
void Ccv1Doc::OnBinarization(int threshold)
{
	CString title;
	CString my_name;

	std::vector<unsigned int> v1(MAX_SIZE * MAX_SIZE);

	for (int i = 0; i < MAX_SIZE*MAX_SIZE; i++)
	{
		v1[i] = img[i];
	}
	std::vector<unsigned int> v2(MAX_SIZE * MAX_SIZE);
	array_view<unsigned int, 2> avImg(MAX_SIZE, MAX_SIZE, v1);
	array_view<unsigned int, 2> avDstImg(MAX_SIZE, MAX_SIZE, v2);

	parallel_for_each(avDstImg.extent, [=](index<2> idx) restrict(amp)
	{
		if (avImg[idx] > threshold)
		{
			avDstImg[idx] = 255;
		}
		else
		{
			avDstImg[idx] = 0;
		}
	});
	avDstImg.synchronize();
	for (int i = 0; i < MAX_SIZE*MAX_SIZE; i++)
	{
		dstimg[i] = v2[i];
	}
	my_name = L"20095208_이창희";
	title.Format(L"Binarization");
	newDocument(my_name, title);
}
void Ccv1Doc::Otzu_Threshold(unsigned char * orgImg, unsigned char *outImg, int height, int width)
{
	/*register int  i, t;

	//histogram set
	int hist[256];
	float prob[256];
	for (i = 0; i<256; i++)
	{
		hist[i] = 0;
		prob[i] = 0.0f;
	}
	for (i = 0; i<height*width; i++)
		hist[(int)orgImg[i]]++;
	for (i = 0; i<256; i++)
		prob[i] = (float)hist[i] / (float)(height*width);

	float wsv_min = 100000.0f;
	float wsv_u1, wsv_u2, wsv_s1, wsv_s2;
	int wsv_t;

	for (t = 0; t<256; t++)
	{
		//q1,q2 계산
		float q1 = 0.0f, q2 = 0.0f;

		for (i = 0; i<t; i++)
			q1 += prob[i];
		for (i = t; i<256; i++)
			q2 += prob[i];
		if (q1 == 0 || q2 == 0)
			continue;

		//u1, u2 계산
		float u1 = 0.0f, u2 = 0.0f;
		for (i = 0; i<t; i++)
			u1 += i*prob[i];
		u1 /= q1;
		for (i = t; i<256; i++)
			u2 += i*prob[i];
		u2 /= q2;

		//s1,s2 계산
		float s1 = 0.0f, s2 = 0.0f;
		for (i = 0; i<t; i++)
			s1 += (i - u1)*(i - u1)*prob[i];
		s1 /= q1;
		for (i = t; i<256; i++)
			s2 += (i - u2)*(i - u2)*prob[i];
		s2 /= q2;

		float wsv = q1*s1 + q2*s2;

		if (wsv < wsv_min)
		{
			wsv_min = wsv;
			wsv_t = t;
			wsv_u1 = u1;
			wsv_u2 = u2;
			wsv_s1 = s1;
			wsv_s2 = s2;
		}
	}

	for (i = 0; i<height*width; i++)
	{
		if (orgImg[i] < wsv_t)
			outImg[i] = 0;
		else
			outImg[i] = 255;
	}
	*/
}
void Ccv1Doc::OnBinarOtzu()
{
	/*CString title;
	CString my_name;
	unsigned char* orgImg = new unsigned char[height*width];
	unsigned char* outImg = new unsigned char[height*width];

	for (int i = 0; i<height; i++)
	for (int j = 0; j<width; j++)
		orgImg[i*width + j] = img[i][j];

	Otzu_Threshold(orgImg, outImg, height, width);

	for (int i = 0; i<height; i++)
	for (int j = 0; j<width; j++)
		dstimg[i][j] = outImg[i*width + j];

	delete[]orgImg;
	delete[]outImg;

	my_name = L"20095208_이창희";
	title.Format(L"OtzuBinarization");
	newDocument(my_name, title);*/
}

void Ccv1Doc::OnBinarAdap()
{
	/*CString title;
	CString my_name;
	unsigned char* orgImg = new unsigned char[height*width];
	unsigned char* outImg = new unsigned char[height*width];

	for (int i = 0; i<height; i++)
	for (int j = 0; j<width; j++)
		orgImg[i*width + j] = img[i][j];

	AdaptiveBinarization(orgImg, outImg, height, width);

	for (int i = 0; i<height; i++)
	for (int j = 0; j<width; j++)
		dstimg[i][j] = outImg[i*width + j];

	delete[]orgImg;
	delete[]outImg;


	my_name = L"20095208_이창희";
	title.Format(L"Ninblack Binarization");
	newDocument(my_name, title);*/
}
void Ccv1Doc::AdaptiveBinarization(unsigned char *orgImg, unsigned char *outImg, int height, int width)
{
	/*register int i, j, k, l;
	int gval, index1, index2;
	float mean, vari, thres;
	int W = 20;

	for (i = 0; i<height*width; i++)
		outImg[i] = 255;

	for (i = 0; i<height; i++)
	{
		index2 = i*width;
		for (j = 0; j<width; j++)
		{
			float gsum = 0.0f;
			float ssum = 0.0f;
			int count = 0;

			for (k = i - W; k <= i + W; k++)
			{
				index1 = k*width;
				if (k<0 || k >= height)
					continue;
				for (l = j - W; l <= j + W; l++)
				{
					if (l<0 || l >= width)
						continue;
					gval = orgImg[index1 + l];
					gsum += gval;
					ssum += gval * gval;
					count++;
				}
			}

			mean = gsum / (float)count;
			vari = ssum / (float)count - mean*mean;

			if (vari<0)
				vari = 0.0f;

			thres = mean*(1.0f - 0.02f*(1 - (float)sqrt(vari) / 128));

			if (orgImg[index2 + j]<thres)
				outImg[index2 + j] = 0;
		}
	}*/

}
void Ccv1Doc::OnSaltNPapperNoise()
{
	/*CString title;
	CString my_name;
	double noise[256][256];
	srand(time(NULL));
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			noise[i][j] = (double)(rand()) / (double)(RAND_MAX);
			if (noise[i][j] > 0.95)
				dstimg[i][j] = 255;
			else if (noise[i][j] < 0.05)
				dstimg[i][j] = 0;
			else
				dstimg[i][j] = img[i][j];
		}
	}
	my_name = L"20095208_이창희";
	title.Format(L"Salt and Papper Noise");
	newDocument(my_name, title);*/
}
void Ccv1Doc::OnSmoothingBox()
{
	CString title;
	CString my_name;
	std::vector<unsigned int> Mask(9);
	Mask[0] = 1;Mask[1] = 1;Mask[2] = 1;
	Mask[3] = 1;Mask[4] = 1;Mask[5] = 1;
	Mask[6] = 1;Mask[7] = 1;Mask[8] = 1;
	array_view<unsigned int, 2> avMask(3, 3, Mask);
	std::vector<unsigned int> v1(MAX_SIZE * MAX_SIZE);

	for (int i = 0; i < MAX_SIZE*MAX_SIZE; i++)
	{
		v1[i] = img[i];
	}
	std::vector<unsigned int> v2(MAX_SIZE * MAX_SIZE);
	array_view<unsigned int, 2> avImg(MAX_SIZE, MAX_SIZE, v1);
	array_view<unsigned int, 2> avDstImg(MAX_SIZE, MAX_SIZE, v2);

	parallel_for_each(avDstImg.extent, [=](index<2> idx) restrict(amp)
	{
		unsigned int newValue = 0;
		for (int mr = 0; mr < 3; mr++)
		{
			for (int mc = 0; mc < 3; mc++)
			{
				newValue += (avMask[mr][mc] * avImg[idx[0] + mr - 1][idx[1] + mc - 1]);
			}
		}
		newValue /= 9;
		avDstImg[idx] = newValue;
	});
	avDstImg.synchronize();
	for (int i = 0; i < MAX_SIZE*MAX_SIZE; i++)
	{
		dstimg[i] = v2[i];
	}
	my_name = L"20095208_이창희";
	title.Format(L"Box Smoothing");
	newDocument(my_name, title);
}
void Ccv1Doc::OnSmoothingGaussian()
{
	CString title;
	CString my_name;
	std::vector<unsigned int> Mask(9);
	Mask[0] = 1; Mask[1] = 2; Mask[2] = 1;
	Mask[3] = 2; Mask[4] = 4; Mask[5] = 2;
	Mask[6] = 1; Mask[7] = 2; Mask[8] = 1;
	array_view<unsigned int, 2> avMask(3, 3, Mask);
	std::vector<unsigned int> v1(MAX_SIZE * MAX_SIZE);

	for (int i = 0; i < MAX_SIZE*MAX_SIZE; i++)
	{
		v1[i] = img[i];
	}
	std::vector<unsigned int> v2(MAX_SIZE * MAX_SIZE);
	array_view<unsigned int, 2> avImg(MAX_SIZE, MAX_SIZE, v1);
	array_view<unsigned int, 2> avDstImg(MAX_SIZE, MAX_SIZE, v2);

	parallel_for_each(avDstImg.extent, [=](index<2> idx) restrict(amp)
	{
		unsigned int newValue = 0;
		for (int mr = 0; mr < 3; mr++)
		{
			for (int mc = 0; mc < 3; mc++)
			{
				newValue += (avMask[mr][mc] * avImg[idx[0] + mr - 1][idx[1] + mc - 1]);
			}
		}
		newValue /= 9;
		avDstImg[idx] = newValue;
	});
	avDstImg.synchronize();
	for (int i = 0; i < MAX_SIZE*MAX_SIZE; i++)
	{
		dstimg[i] = v2[i];
	}
	
	my_name = L"20095208_이창희";
	title.Format(L"Gaussian Smoothing");
	newDocument(my_name, title);
}

void Ccv1Doc::OnSharpeningLaplacian()
{
	/*CString title;
	CString my_name;
	int MaskBox[3][3] = 
	{
		{ -1, -1, -1 },
		{ -1, 8, -1 },
		{ -1, -1, -1 }
	};
	int heightm1 = height - 1;
	int widthm1 = width - 1;
	int mr, mc, min, max, newValue, i, j;
	int *pTmpImg;
	float constVal1, constVal2;

	pTmpImg = new int[height*width];

	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			pTmpImg[i * width + j] = 0;
			dstimg[i][j] = 0;
		}
	}

	for (i = 1; i < heightm1; i++)
	{
		for (j = 1; j < widthm1; j++)
		{
			newValue = 0;
			for (mr = 0; mr < 3; mr++)
			{
				for (mc = 0; mc < 3; mc++)
				{
					newValue += (MaskBox[mr][mc] * img[i + mr - 1][j + mc - 1]);
				}
			}
			if (newValue < 0)
			{
				newValue = -newValue;
			}
			pTmpImg[i * width + j] = newValue;
		}
	}
	min = (int)10e10;
	max = (int)-10e10;
	for (i = 0; i < heightm1; i++)
	{
		for (j = 0; j < widthm1; j++)
		{
			newValue = pTmpImg[i * width + j];
			if (newValue < min)
				min = newValue;
			if (newValue > max)
				max = newValue;
		}
	}

	constVal1 = (float)(255.0 / (max - min));
	constVal2 = (float)(-225.0 * min / (max - min));

	for (i = 1; i < heightm1; i++)
	{
		for (j = 1; j < widthm1; j++)
		{
			newValue = pTmpImg[i * width + j];
			newValue = constVal1 * newValue + constVal2;
			dstimg[i][j] = (BYTE)newValue;
		}
	}

	delete[] pTmpImg;
	my_name = L"20095208_이창희";
	title.Format(L"SharpeningLaplacian");
	newDocument(my_name, title);*/

}

void Ccv1Doc::OnMedianFilter()
{
	/*CString title;
	CString my_name;

	int mc = 0, mr = 0;
	int median[9];
	int temp;
	int heightm1 = height - 1;
	int widthm1 = width - 1;
	
	for (int i = 1; i < heightm1; i++)
	{
		for (int j = 1; j < widthm1; j++)
		{
			for (mc = 0; mc < 3; mc++)
			{
				for (mr = 0; mr < 3; mr++)
				{
					median[mc*3+mr] = img[i + mc - 1][j + mr - 1];
				}
			}

			for (int k = 0; k < 9; k++)
			{
				for (int l = 0; l < 9; l++)
				{
					if (median[l] < median[l + 1])
					{
						temp = median[l];
						median[l] = median[l + 1];
						median[l + 1] = temp;
					}
				}
			}
			dstimg[i][j] = median[4];
		}
	}
	
	my_name = L"20095208_이창희";
	title.Format(L"MedianFilter");
	newDocument(my_name, title);*/
}

void Ccv1Doc::OnEdgePrewitt()
{
/*	CString title;
	CString my_name;
	int MaskPrewittX[3][3] = { { -1, 0, 1 },
							{ -1, 0, 1 },
							{ -1, 0, 1 } };
	int MaskPrewittY[3][3] = { { 1, 1, 1 },
							{ 0, 0, 0 },
							{ -1, -1, -1 } };
	int heightm1 = height - 1;
	int widthm1 = width - 1;
	int newValue, min, max, where;
	int *pImgPrewittX, *pImgPrewittY;
	float constVal1, constVal2;

	pImgPrewittX = new int[height*width];
	pImgPrewittY = new int[height*width];

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			dstimg[i][j] = 0;
			where = i * width + j;
			pImgPrewittX[where] = 0;
			pImgPrewittY[where] = 0;
		}
	}
	for (int i = 1; i < heightm1; i++)
	{
		for (int j = 1; j < widthm1; j++)
		{
			newValue = 0;
			for (int mr = 0; mr < 3; mr++)
			{
				for (int mc = 0; mc < 3; mc++)
				{
					newValue += (MaskPrewittX[mr][mc] * img[i + mr - 1][j + mc - 1]);
				}
			}
			pImgPrewittX[i * width + j] = newValue;
		}
	}
	for (int i = 1; i < heightm1; i++)
	{
		for (int j = 1; j < widthm1; j++)
		{
			newValue = 0;
			for (int mr = 0; mr < 3; mr++)
			{
				for (int mc = 0; mc < 3; mc++)
				{
					newValue += (MaskPrewittY[mr][mc] * img[i + mr - 1][j + mc - 1]);
				}
			}
			pImgPrewittY[i * width + j] = newValue;
		}
	}

	for (int i = 0; i < heightm1; i++)
	{
		for (int j = 0; j < widthm1; j++)
		{
			where = i * width + j;
			constVal1 = pImgPrewittX[where];
			constVal2 = pImgPrewittY[where];

			if (constVal1 < 0)
				constVal1 = -constVal1;
			if (constVal2 < 0)
				constVal2 = -constVal2;
			pImgPrewittX[where] = constVal1 + constVal2;
		}
	}

	min = (int)10e10;
	max = (int)-10e10;

	for (int i = 1; i < heightm1; i++)
	{
		for (int j = 1; j < widthm1; j++)
		{
			newValue = pImgPrewittX[i * width + j];
			if (newValue < min)
				min = newValue;
			if (newValue > max)
				max = newValue;
		}
	}

	constVal1 = (float)(255.0 / (max - min));
	constVal2 = (float)(-255.0 * min / (max - min));

	for (int i = 0; i < heightm1; i++)
	{
		for (int j = 0; j < widthm1; j++)
		{
			newValue = pImgPrewittX[i * width + j];
			newValue = constVal1 * newValue + constVal2;
			dstimg[i][j] = (BYTE)newValue;
		}
	}

	delete[] pImgPrewittX;
	delete[] pImgPrewittY;

	my_name = L"20095208_이창희";
	title.Format(L"Prewitt Edge");
	newDocument(my_name, title);*/
}

void Ccv1Doc::OnEdgeSobel()
{
	/*CString title;
	CString my_name;
	int MaskSobelX[3][3] = { { -1, 0, 1 },
	{ -2, 0, 2 },
	{ -1, 0, 1 } };
	int MaskSobelY[3][3] = { { 1, 2, 1 },
	{ 0, 0, 0 },
	{ -1, -2, -1 } };
	int heightm1 = height - 1;
	int widthm1 = width - 1;
	int newValue, min, max, where;
	int *pImgSobelX, *pImgSobelY;
	float constVal1, constVal2;

	pImgSobelX = new int[height*width];
	pImgSobelY = new int[height*width];

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			dstimg[i][j] = 0;
			where = i * width + j;
			pImgSobelX[where] = 0;
			pImgSobelY[where] = 0;
		}
	}
	for (int i = 1; i < heightm1; i++)
	{
		for (int j = 1; j < widthm1; j++)
		{
			newValue = 0;
			for (int mr = 0; mr < 3; mr++)
			{
				for (int mc = 0; mc < 3; mc++)
				{
					newValue += (MaskSobelX[mr][mc] * img[i + mr - 1][j + mc - 1]);
				}
			}
			pImgSobelX[i * width + j] = newValue;
		}
	}
	for (int i = 1; i < heightm1; i++)
	{
		for (int j = 1; j < widthm1; j++)
		{
			newValue = 0;
			for (int mr = 0; mr < 3; mr++)
			{
				for (int mc = 0; mc < 3; mc++)
				{
					newValue += (MaskSobelY[mr][mc] * img[i + mr - 1][j + mc - 1]);
				}
			}
			pImgSobelY[i * width + j] = newValue;
		}
	}

	for (int i = 0; i < heightm1; i++)
	{
		for (int j = 0; j < widthm1; j++)
		{
			where = i * width + j;
			constVal1 = pImgSobelX[where];
			constVal2 = pImgSobelY[where];

			if (constVal1 < 0)
				constVal1 = -constVal1;
			if (constVal2 < 0)
				constVal2 = -constVal2;
			pImgSobelX[where] = constVal1 + constVal2;
		}
	}

	min = (int)10e10;
	max = (int)-10e10;

	for (int i = 1; i < heightm1; i++)
	{
		for (int j = 1; j < widthm1; j++)
		{
			newValue = pImgSobelX[i * width + j];
			if (newValue < min)
				min = newValue;
			if (newValue > max)
				max = newValue;
		}
	}

	constVal1 = (float)(255.0 / (max - min));
	constVal2 = (float)(-255.0 * min / (max - min));

	for (int i = 0; i < heightm1; i++)
	{
		for (int j = 0; j < widthm1; j++)
		{
			newValue = pImgSobelX[i * width + j];
			newValue = constVal1 * newValue + constVal2;
			dstimg[i][j] = (BYTE)newValue;
		}
	}

	delete[] pImgSobelX;
	delete[] pImgSobelY;

	my_name = L"20095208_이창희";
	title.Format(L"Sobel Edge");
	newDocument(my_name, title);*/
}

void Ccv1Doc::OnZoomIn(int height, int width, float zoomInfactor)
{
	
	/*CString title;
	CString my_name;

	BYTE *pZoomImg;
	BYTE newValue;
	int new_height = (int)(height * zoomInfactor);
	int new_width = (int)(width * zoomInfactor);
	int heightm1 = height - 1;
	int widthm1 = width - 1;
	int where, org_where;
	int r, c;
	float r_orgr, r_orgc;
	int i_orgr, i_orgc;
	float sr, sc;
	float i1, i2, i3, i4;

	pZoomImg = new BYTE[new_height * new_width];

	for (r = 0; r < new_height; r++)
	{
		for (c = 0; c < new_width; c++)
		{
			r_orgr = r / zoomInfactor;
			r_orgc = c / zoomInfactor;

			i_orgr = floor(r_orgr);
			i_orgc = floor(r_orgc);

			sr = r_orgr - i_orgr;
			sc = r_orgc - i_orgc;

			if (i_orgr < 0 || i_orgr > heightm1 || i_orgc < 0 || i_orgc > widthm1)
			{
				where = r * new_width + c;
				pZoomImg[where] = 0;
			}
			else
			{
				i1 = (float)img[i_orgr][i_orgc];
				i2 = (float)img[i_orgr][i_orgc+1];
				i3 = (float)img[i_orgr+1][i_orgc+1];
				i4 = (float)img[i_orgr+1][i_orgc];

				newValue = (BYTE)(i1 * (1 - sc) * (1 - sr) + i2 * sc * (1 - sr) + i3 * sc * sr + i4 * (1 - sc) * sr);
				where = r * new_width + c;
				pZoomImg[where] = newValue;
			}
		}
	}

	for (r = 0; r < height; r++)
	{
		for (c = 0; c < width; c++)
		{
			dstimg[r][c] = pZoomImg[r * new_width + c];
		}
	}

	delete[] pZoomImg;
	
	my_name = L"20095208_이창희";
	title.Format(L"ZoomIn");
	newDocument(my_name, title);*/
}

void Ccv1Doc::OnZoomOut(int height, int width, float zoomInfactor)
{
	CString title;
	CString my_name;
	int new_height = (int)(height * zoomInfactor);
	int new_width = (int)(width * zoomInfactor);

	std::vector<unsigned int> ZoomImg(new_height*new_width);
	std::vector<unsigned int> v1(MAX_SIZE * MAX_SIZE);

	for (int i = 0; i < MAX_SIZE*MAX_SIZE; i++)
	{
		v1[i] = img[i];
	}
	array_view<unsigned int, 2> avZoomImg(new_height, new_width, ZoomImg);
	std::vector<unsigned int> v2(MAX_SIZE * MAX_SIZE);
	array_view<unsigned int, 2> avImg(MAX_SIZE, MAX_SIZE, v1);
	array_view<unsigned int, 2> avDstImg(MAX_SIZE, MAX_SIZE, v2);

	parallel_for_each(avZoomImg.extent, [=](index<2> idx) restrict(amp)
	{
		unsigned int newValue;
		int heightm1 = height - 1;
		int widthm1 = width - 1;
		int where;
		float r_orgr, r_orgc;
		int i_orgr, i_orgc;
		float sr, sc;
		float i1, i2, i3, i4;

		r_orgr = idx[0] / zoomInfactor;
		r_orgc = idx[1] / zoomInfactor;

		i_orgr = r_orgr;
		i_orgc = r_orgc;

		sr = r_orgr - i_orgr;
		sc = r_orgc - i_orgc;

		if (i_orgr < 0 || i_orgr > heightm1 || i_orgc < 0 || i_orgc > widthm1)
		{
			where = idx[0] * new_width + idx[1];
			avZoomImg[idx] = 0;
		}
		else
		{
			i1 = (float)avImg[i_orgr][i_orgc];
			i2 = (float)avImg[i_orgr][i_orgc + 1];
			i3 = (float)avImg[i_orgr + 1][i_orgc + 1];
			i4 = (float)avImg[i_orgr + 1][i_orgc];

			newValue = (unsigned int)(i1 * (1 - sc) * (1 - sr) + i2 * sc * (1 - sr) + i3 * sc * sr + i4 * (1 - sc) * sr);
			where = idx[0] * new_width + idx[1];
			avZoomImg[idx] = newValue;
		}
	});
	avZoomImg.synchronize();

	for (int r = 0; r < new_height; r++)
	{
		for (int c = 0; c < new_width; c++)
		{
			dstimg[(r*new_height) + c] = ZoomImg[r * new_width + c];
		}
	}
	/*CString title;
	CString my_name;

	BYTE *pZoomImg;
	BYTE newValue;
	int new_height = (int)(height * zoomInfactor);
	int new_width = (int)(width * zoomInfactor);
	int heightm1 = height - 1;
	int widthm1 = width - 1;
	int where, org_where;
	int r, c;
	float r_orgr, r_orgc;
	int i_orgr, i_orgc;
	float sr, sc;
	float i1, i2, i3, i4;

	pZoomImg = new BYTE[new_height * new_width];

	for (r = 0; r < new_height; r++)
	{
		for (c = 0; c < new_width; c++)
		{
			r_orgr = r / zoomInfactor;
			r_orgc = c / zoomInfactor;

			i_orgr = floor(r_orgr);
			i_orgc = floor(r_orgc);

			sr = r_orgr - i_orgr;
			sc = r_orgc - i_orgc;

			if (i_orgr < 0 || i_orgr > heightm1 || i_orgc < 0 || i_orgc > widthm1)
			{
				where = r * new_width + c;
				pZoomImg[where] = 0;
			}
			else
			{
				i1 = (float)img[i_orgr][i_orgc];
				i2 = (float)img[i_orgr][i_orgc+1];
				i3 = (float)img[i_orgr+1][i_orgc+1];
				i4 = (float)img[i_orgr+1][i_orgc];

				newValue = (BYTE)(i1 * (1 - sc) * (1 - sr) + i2 * sc * (1 - sr) + i3 * sc * sr + i4 * (1 - sc) * sr);
				where = r * new_width + c;
				pZoomImg[where] = newValue;
			}
		}
	}

	for (r = 0; r < new_height; r++)
	{
		for (c = 0; c < new_width; c++)
		{
			dstimg[r][c] = pZoomImg[r * new_width + c];
		}
	}

	delete[] pZoomImg;
	*/
	my_name = L"20095208_이창희";
	title.Format(L"ZoomOut");
	newDocument(my_name, title);
}
void Ccv1Doc::OnRotation(int height, int width, int center_r, int center_c, float rotationAngle)
{
	/*CString title;
	CString my_name;


	BYTE *pRotateImg;
	BYTE newValue;
	int heightm1 = height - 1;
	int widthm1 = width - 1;
	int where, org_where;
	int r, c;
	float r_orgr, r_orgc;
	int i_orgr, i_orgc;
	float sr, sc;
	float i1, i2, i3, i4;
	float cosAngle, sinAngle;
	float rotationAngleRad = (float)(rotationAngle * 3.14159265 / 180);


	pRotateImg = new BYTE[height * width];

	for (r = 0; r < height; r++)
	{
		for (c = 0; c < width; c++)
		{
			cosAngle = (float)cos(rotationAngleRad);
			sinAngle = (float)sin(rotationAngleRad);

			r_orgr = -sinAngle*(c - center_c) + cosAngle*(r - center_r) + center_r;
			r_orgc = cosAngle*(c - center_c) + sinAngle*(r - center_r) + center_c;

			i_orgr = floor(r_orgr);
			i_orgc = floor(r_orgc);

			sr = r_orgr - i_orgr;
			sc = r_orgc - i_orgc;

			if (i_orgr < 0 || i_orgr > heightm1 || i_orgc < 0 || i_orgc > widthm1)
			{
				where = r * width + c;
				pRotateImg[where] = 0;
			}
			else
			{
				i1 = (float)img[i_orgr][i_orgc];
				i2 = (float)img[i_orgr][i_orgc+1];
				i3 = (float)img[i_orgr+1][i_orgc+1];
				i4 = (float)img[i_orgr+1][i_orgc];

				newValue = (BYTE)(i1 * (1 - sc) * (1 - sr) + i2 * sc * (1 - sr) + i3 * sc * sr + i4 * (1 - sc) * sr);
				where = r * width + c;
				pRotateImg[where] = newValue;
			}
		}
	}

	for (r = 0; r < height; r++)
	{
		for (c = 0; c < width; c++)
		{
			dstimg[r][c] = pRotateImg[r * width + c];
		}
	}

	delete[] pRotateImg;

	my_name = L"20095208_이창희";
	title.Format(L"Rotation");
	newDocument(my_name, title);*/
}

void Ccv1Doc::OnErosion()
{
	CString title;
	CString my_name;
	std::vector<unsigned int> Mask(9);
	Mask[0] = 255; Mask[1] = 0; Mask[2] = 255;
	Mask[3] = 0; Mask[4] = 0; Mask[5] = 0;
	Mask[6] = 255; Mask[7] = 0; Mask[8] = 255;
	array_view<unsigned int, 2> avMask(3, 3, Mask);
	std::vector<unsigned int> v1(MAX_SIZE * MAX_SIZE);
	std::vector<unsigned int> v2(MAX_SIZE * MAX_SIZE);
	for (int i = 0; i < MAX_SIZE*MAX_SIZE; i++)
	{
		v1[i] = img[i];
		v2[i] = 0;
	}
	array_view<unsigned int, 2> avImg(MAX_SIZE, MAX_SIZE, v1);
	array_view<unsigned int, 2> avDstImg(MAX_SIZE, MAX_SIZE, v2);

	parallel_for_each(avDstImg.extent, [=](index<2> idx) restrict(amp)
	{
		int flag = 0;
		if (idx[0] == 0 || idx[1] == 0)
			return;
		if (idx[0] == MAX_SIZE-1 || idx[1] == MAX_SIZE-1)
			return;

		for (int mr = 0; mr < 3; mr++)
		{
			for (int mc = 0; mc < 3; mc++)
			{
				if (avMask[mr][mc] == 0 &&
					avMask[mr][mc] != avImg[idx[0] + mr - 1][idx[1] + mc - 1])
					flag++;
			}
		}
		if (flag == 0)
			avDstImg[idx] = 0;
		else
			avDstImg[idx] = 255;
	});
	avDstImg.synchronize();
	for (int i = 0; i < MAX_SIZE*MAX_SIZE; i++)
	{
		dstimg[i] = v2[i];
	}
	/*CString title;
	CString my_name;

	int mr, mc;
	int newValue;
	int MaskBox[3][3] = { { 255, 0, 255 }, { 0, 0, 0 }, { 255, 0, 255 } };

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			dstimg[i][j] = 0;
		}
	}
	for (int i = 1; i < height - 1; i++)
	{
		for (int j = 1; j < width - 1; j++)
		{
			int flag = 0;
			for (mr = 0; mr < 3; mr++)
			{
				for (mc = 0; mc < 3; mc++)
				{
					if (MaskBox[mr][mc] == 0 &&
						MaskBox[mr][mc] != img[i + mr - 1][j + mc - 1])
						flag++;
				}
			}
			if (flag == 0)
				dstimg[i][j] = 0;
			else
				dstimg[i][j] = 255;
		}
	}
	*/
	my_name = L"20095208_이창희";
	title.Format(L"Erosion");
	newDocument(my_name, title);
}
void Ccv1Doc::OnDilation()
{
	/*CString title;
	CString my_name;

	int SumMask = 0;
	int mr, mc;
	int newValue;
	int MaskBox[3][3] = { { 255, 0, 255 }, { 0, 0, 0 }, { 255, 0, 255 } };

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			dstimg[i][j] = 0;
		}
	}
	for (int i = 1; i < height - 1; i++)
	{
		for (int j = 1; j < width - 1; j++)
		{
			int flag = 0;
			for (mr = 0; mr < 3; mr++)
			{
				for (mc = 0; mc < 3; mc++)
				{
					if (MaskBox[mr][mc] == 0 &&
						MaskBox[mr][mc] == img[i + mr - 1][j + mc - 1])
						flag++;
				}
			}
			if (flag != 0)
				dstimg[i][j] = 0;
			else
				dstimg[i][j] = 255;
		}
	}

	my_name = L"20095208_이창희";
	title.Format(L"Dilation");
	newDocument(my_name, title);*/
}

void Ccv1Doc::OnAMPADD()
{
	/*std::vector<unsigned int> LUT(256);
	array_view<unsigned int, 1> avLUT(256, LUT);
	array_view<unsigned int, 2> avImg(MAX_SIZE, MAX_SIZE, img);
	array_view<unsigned int, 2> avDst(MAX_SIZE, MAX_SIZE, dstimg);

	parallel_for_each(avLUT.extent, [=](index<1> idx) restrict(amp)
	{
		int temp = (int)((float)idx[0] + 60);
		avLUT[idx] = temp > 255 ? 255 : temp;
	});
	avLUT.synchronize();
	parallel_for_each(avDst.extent, [=](index<2> idx) restrict(amp)
	{
		
		avDst[idx] = avLUT[avImg[idx[0]][idx[1]]];
	});
	newDocument(L"20095208_이창희", L"60더하기 연산");*/
}
#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void Ccv1Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void Ccv1Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void Ccv1Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Ccv1Doc 진단

#ifdef _DEBUG
void Ccv1Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void Ccv1Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Ccv1Doc 명령
