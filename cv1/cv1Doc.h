
// cv1Doc.h : Ccv1Doc 클래스의 인터페이스
//


#pragma once

class Ccv1Doc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	Ccv1Doc();
	DECLARE_DYNCREATE(Ccv1Doc)

// 특성입니다.
public:
	unsigned char inImg[MAX_SIZE * MAX_SIZE];
	unsigned char outImg[MAX_SIZE * MAX_SIZE];

	unsigned char img[MAX_SIZE * MAX_SIZE];
	unsigned char dstimg[MAX_SIZE * MAX_SIZE];

	int m_histoarr[256];
	int height;
	int width;
// 작업입니다.
public:
	void newDocument(CString orgName, CString processName);
//픽셀처리
	void onReverseImg();
	void OnConstADD();
	void OnLUTADD();
	void OnLUTSUB();
	void OnLUTMUL();
	void OnLUTDIV();

	void OnPosterizing(int level);
	void OnHistogram(int in_height, int in_width);
	void OnHistequal(int in_height, int in_width);
	void OnHistStretch(int in_height, int in_width);
	void OnHistUpStretch(int in_height, int in_width, int lowpercent, int highpercent);
	void OnBinarization(int threshold);
//오쯔이진화
	void Otzu_Threshold(unsigned char * orgImg, unsigned char *outImg, int height, int width);
	void OnBinarOtzu();
//
	void OnBinarAdap();
	void AdaptiveBinarization(unsigned char *orgImg, unsigned char *outImg, int height, int width);

//마스크처리
	void OnSaltNPapperNoise();
	void OnSmoothingBox();
	void OnSmoothingGaussian();

	void OnSharpeningLaplacian();
	void OnMedianFilter();
	void OnEdgePrewitt();
	void OnEdgeSobel();

//기하 변형
	void OnZoomIn(int height, int width, float zoomInfactor);
	void OnZoomOut(int height, int width, float zoomInfactor);
	void OnRotation(int height, int width, int center_r, int center_c, float rotationAngle);

//모폴로지
	void OnErosion();
	void OnDilation();

//C++AMP를 이용한 영상처리
	void OnAMPADD();
	void OnAMPSUB();
	void OnAMPMUL();
	void OnAMPDIV();
// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~Ccv1Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
