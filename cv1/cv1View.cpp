
// cv1View.cpp : Ccv1View 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "cv1.h"
#endif

#include "cv1Doc.h"
#include "cv1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ccv1View

IMPLEMENT_DYNCREATE(Ccv1View, CScrollView)

BEGIN_MESSAGE_MAP(Ccv1View, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_32771, &Ccv1View::OnReverseImg)
	ON_COMMAND(ID_32772, &Ccv1View::OnConstADD)
	ON_COMMAND(ID_32773, &Ccv1View::LUTADD)
	ON_COMMAND(ID_32774, &Ccv1View::LUTSUB)
	ON_COMMAND(ID_32775, &Ccv1View::LUTMUL)
	ON_COMMAND(ID_32776, &Ccv1View::LUTDIV)
	ON_COMMAND(ID_32777, &Ccv1View::OnPosterizing)
	ON_COMMAND(ID_32778, &Ccv1View::OnPosterizing_5)
	ON_COMMAND(ID_32779, &Ccv1View::OnPosterizing_8)
	ON_COMMAND(ID_32780, &Ccv1View::OnHistogram)
	ON_COMMAND(ID_32781, &Ccv1View::OnBinarization)
	ON_COMMAND(ID_32782, &Ccv1View::OnHistoEqual)
	ON_COMMAND(ID_32783, &Ccv1View::OnHistStretch)
	ON_COMMAND(ID_32784, &Ccv1View::OnHistUpStretch)
	ON_COMMAND(ID_32785, &Ccv1View::OnBoxSmoothing)
	ON_COMMAND(ID_32786, &Ccv1View::OnGaussianSmoothing)
	ON_COMMAND(ID_32787, &Ccv1View::OnSaltNPapper)
	ON_COMMAND(ID_32788, &Ccv1View::OnSharpeningLaplacian)
	ON_COMMAND(ID_32789, &Ccv1View::OnMedianFilter)
	ON_COMMAND(ID_32790, &Ccv1View::OnEdgePrewitt)
	ON_COMMAND(ID_32791, &Ccv1View::OnEdgeSobel)
	ON_COMMAND(ID_32793, &Ccv1View::OnZoomIn)
	ON_COMMAND(ID_32792, &Ccv1View::OnZoomOut)
	ON_COMMAND(ID_32794, &Ccv1View::OnRotation)
	ON_COMMAND(ID_32795, &Ccv1View::OnErosion)
	ON_COMMAND(ID_32796, &Ccv1View::OnDilation)
	ON_COMMAND(ID_32797, &Ccv1View::OnBinarOtzu)
	ON_COMMAND(ID_32798, &Ccv1View::OnBinarAdap)
END_MESSAGE_MAP()

// Ccv1View 생성/소멸

Ccv1View::Ccv1View()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	height = width = MAX_SIZE;
	int rwsize = (((width)+31) / 32 * 4);
	bmInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO)+MAX_SIZE * sizeof(RGBQUAD));
	bmInfo->bmiHeader.biBitCount = 8;
	bmInfo->bmiHeader.biClrImportant = 256;
	bmInfo->bmiHeader.biClrUsed = 256;
	bmInfo->bmiHeader.biCompression = 0;
	bmInfo->bmiHeader.biHeight = height;
	bmInfo->bmiHeader.biPlanes = 1;
	bmInfo->bmiHeader.biSize = 40;
	bmInfo->bmiHeader.biSizeImage = rwsize * height;
	bmInfo->bmiHeader.biWidth = width;
	bmInfo->bmiHeader.biXPelsPerMeter = 0;
	bmInfo->bmiHeader.biYPelsPerMeter = 0;

	for (int i = 0; i < MAX_SIZE; i++)
	{
		bmInfo->bmiColors[i].rgbRed = bmInfo->bmiColors[i].rgbBlue = bmInfo->bmiColors[i].rgbGreen = i;
		bmInfo->bmiColors[i].rgbReserved = 0;
	}
}

Ccv1View::~Ccv1View()
{
	free(bmInfo);
}

BOOL Ccv1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// Ccv1View 그리기

void Ccv1View::OnDraw(CDC* pDC)
{
	Ccv1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//m_RevImg[i][j] = pDoc->img[height - i - 1][j];
			m_RevImg[i][j] = pDoc->img[(height*(height-i-1))+j];
		}
	}
	SetDIBitsToDevice(pDC->GetSafeHdc(), 0, 0, width, height, 0, 0, 0, height, m_RevImg, bmInfo, DIB_RGB_COLORS);

}

void Ccv1View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	//CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	//sizeTotal.cx = sizeTotal.cy = 100;
	//SetScrollSizes(MM_TEXT, sizeTotal);

	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CSize sizeViewPageTotal;
	sizeViewPageTotal.cx = pDoc->width;
	sizeViewPageTotal.cy = pDoc->height;
	SetScrollSizes(MM_TEXT, sizeViewPageTotal);
	ResizeParentToFit(TRUE);
}


// Ccv1View 인쇄

BOOL Ccv1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void Ccv1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void Ccv1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// Ccv1View 진단

#ifdef _DEBUG
void Ccv1View::AssertValid() const
{
	CScrollView::AssertValid();
}

void Ccv1View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

Ccv1Doc* Ccv1View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Ccv1Doc)));
	return (Ccv1Doc*)m_pDocument;
}
#endif //_DEBUG


// Ccv1View 메시지 처리기


void Ccv1View::OnReverseImg()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	/*for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			pDoc->outImg[i][j] = 255 - pDoc->inImg[i][j];
		}
	}
	Invalidate(FALSE);*/
	pDoc->onReverseImg();
}


void Ccv1View::OnConstADD()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	pDoc->OnConstADD();
}


void Ccv1View::LUTADD()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnLUTADD();
}


void Ccv1View::LUTSUB()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnLUTSUB();
}


void Ccv1View::LUTMUL()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnLUTMUL();
}


void Ccv1View::LUTDIV()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnLUTDIV();
}


void Ccv1View::OnPosterizing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnPosterizing(3);
}


void Ccv1View::OnPosterizing_5()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnPosterizing(5);
}


void Ccv1View::OnPosterizing_8()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnPosterizing(8);
}


void Ccv1View::OnHistogram()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnHistogram(height, width);
}


void Ccv1View::OnBinarization()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnBinarization(100);
}


void Ccv1View::OnHistoEqual()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnHistequal(height, width);
}


void Ccv1View::OnHistStretch()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnHistStretch(height, width);
}


void Ccv1View::OnHistUpStretch()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnHistUpStretch(height, width, 20, 20);
}


void Ccv1View::OnBoxSmoothing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnSmoothingBox();
}


void Ccv1View::OnGaussianSmoothing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnSmoothingGaussian();
}


void Ccv1View::OnSaltNPapper()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnSaltNPapperNoise();
}


void Ccv1View::OnSharpeningLaplacian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnSharpeningLaplacian();
}


void Ccv1View::OnMedianFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMedianFilter();
}


void Ccv1View::OnEdgePrewitt()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEdgePrewitt();
}


void Ccv1View::OnEdgeSobel()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEdgeSobel();
}


void Ccv1View::OnZoomIn()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnZoomIn(height, width, 2.0);
}


void Ccv1View::OnZoomOut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnZoomOut(height, width, 0.7);
}


void Ccv1View::OnRotation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnRotation(height, width, height/2, width/2, 70);
}


void Ccv1View::OnErosion()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnErosion();
}


void Ccv1View::OnDilation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnDilation();
}


void Ccv1View::OnBinarOtzu()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnBinarOtzu();
}


void Ccv1View::OnBinarAdap()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Ccv1Doc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnBinarAdap();
}
