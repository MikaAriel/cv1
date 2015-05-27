
// cv1View.h : Ccv1View Ŭ������ �������̽�
//

#pragma once


class Ccv1View : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	Ccv1View();
	DECLARE_DYNCREATE(Ccv1View)

// Ư���Դϴ�.
public:
	Ccv1Doc* GetDocument() const;

// �۾��Դϴ�.
public:
	BITMAPINFO *bmInfo;
	int height;
	int width;
	unsigned char m_RevImg[MAX_SIZE][MAX_SIZE];

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~Ccv1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnReverseImg();
	afx_msg void OnConstADD();
	afx_msg void LUTADD();
	afx_msg void LUTSUB();
	afx_msg void LUTMUL();
	afx_msg void LUTDIV();
	afx_msg void OnPosterizing();
	afx_msg void OnPosterizing_5();
	afx_msg void OnPosterizing_8();
	afx_msg void OnHistogram();
	afx_msg void OnBinarization();
	afx_msg void OnHistoEqual();
	afx_msg void OnHistStretch();
	afx_msg void OnHistUpStretch();
	afx_msg void OnBoxSmoothing();
	afx_msg void OnGaussianSmoothing();
	afx_msg void OnSaltNPapper();
	afx_msg void OnSharpeningLaplacian();
	afx_msg void OnMedianFilter();
	afx_msg void OnEdgePrewitt();
	afx_msg void OnEdgeSobel();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnRotation();
	afx_msg void OnErosion();
	afx_msg void OnDilation();
	afx_msg void OnBinarOtzu();
	afx_msg void OnBinarAdap();
};

#ifndef _DEBUG  // cv1View.cpp�� ����� ����
inline Ccv1Doc* Ccv1View::GetDocument() const
   { return reinterpret_cast<Ccv1Doc*>(m_pDocument); }
#endif

