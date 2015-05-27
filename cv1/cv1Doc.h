
// cv1Doc.h : Ccv1Doc Ŭ������ �������̽�
//


#pragma once

class Ccv1Doc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	Ccv1Doc();
	DECLARE_DYNCREATE(Ccv1Doc)

// Ư���Դϴ�.
public:
	unsigned char inImg[MAX_SIZE * MAX_SIZE];
	unsigned char outImg[MAX_SIZE * MAX_SIZE];

	unsigned char img[MAX_SIZE * MAX_SIZE];
	unsigned char dstimg[MAX_SIZE * MAX_SIZE];

	int m_histoarr[256];
	int height;
	int width;
// �۾��Դϴ�.
public:
	void newDocument(CString orgName, CString processName);
//�ȼ�ó��
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
//��������ȭ
	void Otzu_Threshold(unsigned char * orgImg, unsigned char *outImg, int height, int width);
	void OnBinarOtzu();
//
	void OnBinarAdap();
	void AdaptiveBinarization(unsigned char *orgImg, unsigned char *outImg, int height, int width);

//����ũó��
	void OnSaltNPapperNoise();
	void OnSmoothingBox();
	void OnSmoothingGaussian();

	void OnSharpeningLaplacian();
	void OnMedianFilter();
	void OnEdgePrewitt();
	void OnEdgeSobel();

//���� ����
	void OnZoomIn(int height, int width, float zoomInfactor);
	void OnZoomOut(int height, int width, float zoomInfactor);
	void OnRotation(int height, int width, int center_r, int center_c, float rotationAngle);

//��������
	void OnErosion();
	void OnDilation();

//C++AMP�� �̿��� ����ó��
	void OnAMPADD();
	void OnAMPSUB();
	void OnAMPMUL();
	void OnAMPDIV();
// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~Ccv1Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
