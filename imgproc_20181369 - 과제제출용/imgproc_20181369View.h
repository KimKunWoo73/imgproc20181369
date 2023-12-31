﻿
// imgproc_20181369View.h: Cimgproc20181369View 클래스의 인터페이스
//

#pragma once


class Cimgproc20181369View : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	Cimgproc20181369View() noexcept;
	DECLARE_DYNCREATE(Cimgproc20181369View)

// 특성입니다.
public:
	Cimgproc20181369Doc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~Cimgproc20181369View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnMenutest();
	afx_msg void OnPixelAdd();
	afx_msg void OnPixelMin();
	afx_msg void OnPixelMul();
	afx_msg void OnPixelDiv();
	afx_msg void OnPixelHistoEq();
	afx_msg void OnPixelStrech();
	afx_msg void OnPixelBinarization();
	afx_msg void OnPixelTwoImageAdd();
	void LoadTwoImage();
	afx_msg void OnPixelTwoImageSub();
	afx_msg void OnRegionSharpening();
	void convolve(unsigned char** inImg, unsigned char** outImg, int cols, int rows, float mask[][3], int bias, int depth);
	afx_msg void OnRegionSmoothing();
	afx_msg void OnRegionEmbossing();
	afx_msg void OnRegionPrewitt();
	afx_msg void OnRegionRoberts();
	afx_msg void OnRegionSobel();
	afx_msg void OnRegionAverageFiltering();
	afx_msg void OnRegionMedianFiltering();
	afx_msg void OnMopologyColorToGray();
	afx_msg void OnMopologyBinarization();
	afx_msg void OnMopologyErosion();
	afx_msg void OnMopologyDilation();
	afx_msg void OnMopologyOpening();
	void CopyResultToInput();
	afx_msg void OnMopologyClosing();
	afx_msg void OnGeometryZoominPixelCopy();
	afx_msg void OnGeometryZoominBilinearInterpolation();
	afx_msg void OnGeometryZoomoutSubSampling();
	afx_msg void OnGeometryZoomoutMeanSubSampling();
	afx_msg void OnGeometryAvgSampling();
	afx_msg void OnGeometryRotation();
};

#ifndef _DEBUG  // imgproc_20181369View.cpp의 디버그 버전
inline Cimgproc20181369Doc* Cimgproc20181369View::GetDocument() const
   { return reinterpret_cast<Cimgproc20181369Doc*>(m_pDocument); }
#endif

