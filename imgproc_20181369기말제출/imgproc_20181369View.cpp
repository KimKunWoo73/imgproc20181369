
// imgproc_20181369View.cpp: Cimgproc20181369View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "imgproc_20181369.h"
#endif

#include "imgproc_20181369Doc.h"
#include "imgproc_20181369View.h"
#include "CAngleInputDialog.h"
#include "vfw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define MORPHING    8


// Cimgproc20181369View

IMPLEMENT_DYNCREATE(Cimgproc20181369View, CScrollView)

BEGIN_MESSAGE_MAP(Cimgproc20181369View, CScrollView)
	//	ON_COMMAND(ID_MENUTEST, &Cimgproc20181369View::OnMenutest)
	ON_COMMAND(ID_PIXEL_ADD, &Cimgproc20181369View::OnPixelAdd)
	ON_COMMAND(ID_PIXEL_MIN, &Cimgproc20181369View::OnPixelMin)
	ON_COMMAND(ID_PIXEL_MUL, &Cimgproc20181369View::OnPixelMul)
	ON_COMMAND(ID_PIXEL_DIV, &Cimgproc20181369View::OnPixelDiv)
	ON_COMMAND(ID_PIXEL_HISTO_EQ, &Cimgproc20181369View::OnPixelHistoEq)
	ON_COMMAND(ID_PIXEL_STRECH, &Cimgproc20181369View::OnPixelStrech)
	ON_COMMAND(ID_PIXEL_BINARIZATION, &Cimgproc20181369View::OnPixelBinarization)
	ON_COMMAND(ID_PIXEL_TWO_IMAGE_ADD, &Cimgproc20181369View::OnPixelTwoImageAdd)
	ON_COMMAND(ID_PIXEL_TWO_IMAGE_SUB, &Cimgproc20181369View::OnPixelTwoImageSub)
	ON_COMMAND(ID_REGION_SHARPENING, &Cimgproc20181369View::OnRegionSharpening)
	ON_COMMAND(ID_REGION_SMOOTHING, &Cimgproc20181369View::OnRegionSmoothing)
	ON_COMMAND(ID_REGION_EMBOSSING, &Cimgproc20181369View::OnRegionEmbossing)
	ON_COMMAND(ID_REGION_PREWITT, &Cimgproc20181369View::OnRegionPrewitt)
	ON_COMMAND(ID_REGION_ROBERTS, &Cimgproc20181369View::OnRegionRoberts)
	ON_COMMAND(ID_REGION_SOBEL, &Cimgproc20181369View::OnRegionSobel)
	ON_COMMAND(ID_REGION_AVERAGE_FILTERING, &Cimgproc20181369View::OnRegionAverageFiltering)
	ON_COMMAND(ID_REGION_MEDIAN_FILTERING, &Cimgproc20181369View::OnRegionMedianFiltering)
	ON_COMMAND(ID_MOPOLOGY_COLOR_TO_GRAY, &Cimgproc20181369View::OnMopologyColorToGray)
	ON_COMMAND(ID_MOPOLOGY_BINARIZATION, &Cimgproc20181369View::OnMopologyBinarization)
	ON_COMMAND(ID_MOPOLOGY_EROSION, &Cimgproc20181369View::OnMopologyErosion)
	ON_COMMAND(ID_MOPOLOGY_DILATION, &Cimgproc20181369View::OnMopologyDilation)
	ON_COMMAND(ID_MOPOLOGY_OPENING, &Cimgproc20181369View::OnMopologyOpening)
	ON_COMMAND(ID_MOPOLOGY_CLOSING, &Cimgproc20181369View::OnMopologyClosing)
	ON_COMMAND(ID_GEOMETRY_ZOOMIN_PIXEL_COPY, &Cimgproc20181369View::OnGeometryZoominPixelCopy)
	ON_COMMAND(ID_GEOMETRY_ZOOMIN_BILINEAR_INTERPOLATION, &Cimgproc20181369View::OnGeometryZoominBilinearInterpolation)
	ON_COMMAND(ID_GEOMETRY_ZOOMOUT_SUB_SAMPLING, &Cimgproc20181369View::OnGeometryZoomoutSubSampling)
	ON_COMMAND(ID_GEOMETRY_ZOOMOUT_MEAN_SUB_SAMPLING, &Cimgproc20181369View::OnGeometryZoomoutMeanSubSampling)
	ON_COMMAND(ID_GEOMETRY_AVG_SAMPLING, &Cimgproc20181369View::OnGeometryAvgSampling)
	ON_COMMAND(ID_GEOMETRY_ROTATION, &Cimgproc20181369View::OnGeometryRotation)
	ON_COMMAND(ID_GEOMETRY_MIRROR, &Cimgproc20181369View::OnGeometryMirror)
	ON_COMMAND(ID_GEOMETRY_FLIP, &Cimgproc20181369View::OnGeometryFlip)
	ON_COMMAND(ID_GEOMETRY_WARPING, &Cimgproc20181369View::OnGeometryWarping)
	ON_WM_MOUSEMOVE()
//	ON_WM_LBUTTONUP()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_COMMAND(ID_AVI_VIEW, &Cimgproc20181369View::OnAviView)
ON_COMMAND(ID_GEOMETRY_MORPHING, &Cimgproc20181369View::OnGeometryMorphing)
END_MESSAGE_MAP()

// Cimgproc20181369View 생성/소멸

Cimgproc20181369View::Cimgproc20181369View() noexcept
{
	bAviMode = false;
}

Cimgproc20181369View::~Cimgproc20181369View()
{
}

BOOL Cimgproc20181369View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// Cimgproc20181369View 그리기

void Cimgproc20181369View::OnDraw(CDC* pDC)
{
	Cimgproc20181369Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	int x, y;

	if (bAviMode)
	{
		//AVI파일 재생
		LoadAviFile(pDC);
		bAviMode = false;
		return;
	}

	if (pDoc->InputImg != NULL)
	{
		if (pDoc->depth == 1) {
			for (y = 0; y < pDoc->imageHeight; y++)
				for (x = 0; x < pDoc->imageWidth; x++)//  RGB가 같으면 흑백
					pDC->SetPixel(x, y, RGB(pDoc->InputImg[y][x], pDoc->InputImg[y][x], pDoc->InputImg[y][x]));
		}
		else
		{
			for (y = 0; y < pDoc->imageHeight; y++)
				for (x = 0; x < pDoc->imageWidth; x++)//        0 = red                      1 = green                  2 = blue
					pDC->SetPixel(x, y, RGB(pDoc->InputImg[y][3 * x + 0], pDoc->InputImg[y][3 * x + 1], pDoc->InputImg[y][3 * x + 2]));
		}
	}

	if (pDoc->ResultImg != NULL)
	{
		if (pDoc->depth == 1) {
			for (y = 0; y < pDoc->imageHeight; y++)
				for (x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(pDoc->imageWidth + 20 + x, y, RGB(pDoc->ResultImg[y][x], pDoc->ResultImg[y][x], pDoc->ResultImg[y][x]));
		}
		else
		{
			for (y = 0; y < pDoc->imageHeight; y++)
				for (x = 0; x < pDoc->imageWidth; x++)//                                  0 = red                      1 = green                  2 = blue
					pDC->SetPixel(pDoc->imageWidth + 20 + x, y, RGB(pDoc->ResultImg[y][3 * x + 0], pDoc->ResultImg[y][3 * x + 1], pDoc->ResultImg[y][3 * x + 2]));
		}
	}

	if (pDoc->InputImg2 != NULL)
	{
		if (pDoc->depth == 1) {
			for (y = 0; y < pDoc->imageHeight; y++)
				for (x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(2 * (pDoc->imageWidth) + 40 + x, y, RGB(pDoc->InputImg2[y][x], pDoc->InputImg2[y][x], pDoc->InputImg2[y][x]));
		}
		else
		{
			for (y = 0; y < pDoc->imageHeight; y++)
				for (x = 0; x < pDoc->imageWidth; x++)//                                    0 = red                      1 = green                  2 = blue
					pDC->SetPixel(2 * (pDoc->imageWidth) + 40 + x, y, RGB(pDoc->InputImg2[y][3 * x + 0], pDoc->InputImg2[y][3 * x + 1], pDoc->InputImg2[y][3 * x + 2]));
		}
	}


	if (pDoc->gResultImg != NULL)
	{
		if (pDoc->depth == 1) {
			for (y = 0; y < pDoc->gImageHeight; y++)
				for (x = 0; x < pDoc->gImageWidth; x++)
					pDC->SetPixel(x, pDoc->imageHeight + 20 + y, RGB(pDoc->gResultImg[y][x], pDoc->gResultImg[y][x], pDoc->gResultImg[y][x]));
		}
		else
		{
			for (y = 0; y < pDoc->gImageHeight; y++)
				for (x = 0; x < pDoc->gImageWidth; x++)
					pDC->SetPixel(x, pDoc->imageHeight + 20 + y, RGB(pDoc->gResultImg[y][3 * x + 0], pDoc->gResultImg[y][3 * x + 1], pDoc->gResultImg[y][3 * x + 2]));
		}
	}

	if (viewMode == MORPHING) {
		for (int y = 0; y < pDoc->imageHeight; y++)       // 두번째 입력 영상 출력 
			for (int x = 0; x < pDoc->imageWidth; x++)
				pDC->SetPixel(x + pDoc->imageWidth * 2 + 40, y,
					RGB(pDoc->InputImg2[y][x],
						pDoc->InputImg2[y][x],
						pDoc->InputImg2[y][x]));
		for (int i = 0; i < 10; i++)
			for (int y = 0; y < pDoc->imageHeight; y++)       // 모핑 결과 출력 
				for (int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 20, y,
						RGB(pDoc->morphedImg[i][y][x],
							pDoc->morphedImg[i][y][x],
							pDoc->morphedImg[i][y][x]));
	}

	return;
}

void Cimgproc20181369View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 2048;    // 크기 늘려서 스크롤 생성
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// Cimgproc20181369View 진단

#ifdef _DEBUG
void Cimgproc20181369View::AssertValid() const
{
	CScrollView::AssertValid();
}

void Cimgproc20181369View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

Cimgproc20181369Doc* Cimgproc20181369View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cimgproc20181369Doc)));
	return (Cimgproc20181369Doc*)m_pDocument;
}
#endif //_DEBUG


void Cimgproc20181369View::OnPixelAdd()
{
	Cimgproc20181369Doc* pDoc = GetDocument();

	if (pDoc->InputImg == NULL)
		return;

	int x, y, value;
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			if (pDoc->depth == 1)
			{
				value = pDoc->InputImg[y][x] + 100;
				if (value > 255)value = 255;
				else if (value < 0) value = 0;

				pDoc->ResultImg[y][x] = value;
			}
			else
			{
				value = pDoc->InputImg[y][3 * x + 0] + 100;
				if (value > 255)value = 255;
				else if (value < 0) value = 0;

				pDoc->ResultImg[y][3 * x + 0] = value;


				value = pDoc->InputImg[y][3 * x + 1] + 100;
				if (value > 255)value = 255;
				else if (value < 0) value = 0;

				pDoc->ResultImg[y][3 * x + 1] = value;


				value = pDoc->InputImg[y][3 * x + 2] + 100;
				if (value > 255)value = 255;
				else if (value < 0) value = 0;

				pDoc->ResultImg[y][3 * x + 2] = value;
			}

		}

	Invalidate();   //Invalidate() 메서드를 호출하여 컨트롤의 새로운 값을 화면에 표시

}


void Cimgproc20181369View::OnPixelMin()
{
	Cimgproc20181369Doc* pDoc = GetDocument();

	if (pDoc->InputImg == NULL)
		return;

	int x, y, value;
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			if (pDoc->depth == 1)
			{
				value = pDoc->InputImg[y][x] - 100;
				if (value > 255)value = 255;
				else if (value < 0) value = 0;

				pDoc->ResultImg[y][x] = value;
			}
			else
			{
				value = pDoc->InputImg[y][3 * x + 0] - 100;
				if (value > 255)value = 255;
				else if (value < 0) value = 0;

				pDoc->ResultImg[y][3 * x + 0] = value;


				value = pDoc->InputImg[y][3 * x + 1] - 100;
				if (value > 255)value = 255;
				else if (value < 0) value = 0;

				pDoc->ResultImg[y][3 * x + 1] = value;


				value = pDoc->InputImg[y][3 * x + 2] - 100;
				if (value > 255)value = 255;
				else if (value < 0) value = 0;

				pDoc->ResultImg[y][3 * x + 2] = value;
			}

		}

	Invalidate();
}


void Cimgproc20181369View::OnPixelMul()
{
	Cimgproc20181369Doc* pDoc = GetDocument();

	if (pDoc->InputImg == NULL)
		return;

	int x, y, value;
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			if (pDoc->depth == 1)
			{
				value = pDoc->InputImg[y][x] * 1.5;
				if (value > 255)value = 255;
				else if (value < 0) value = 0;

				pDoc->ResultImg[y][x] = value;
			}
			else
			{
				value = pDoc->InputImg[y][3 * x + 0] * 1.5;
				if (value > 255)value = 255;
				else if (value < 0) value = 0;

				pDoc->ResultImg[y][3 * x + 0] = value;


				value = pDoc->InputImg[y][3 * x + 1] * 1.5;
				if (value > 255)value = 255;
				else if (value < 0) value = 0;

				pDoc->ResultImg[y][3 * x + 1] = value;


				value = pDoc->InputImg[y][3 * x + 2] * 1.5;
				if (value > 255)value = 255;
				else if (value < 0) value = 0;

				pDoc->ResultImg[y][3 * x + 2] = value;
			}

		}

	Invalidate();
}


void Cimgproc20181369View::OnPixelDiv()
{
	Cimgproc20181369Doc* pDoc = GetDocument();

	if (pDoc->InputImg == NULL)
		return;

	int x, y, value;
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			if (pDoc->depth == 1)
			{
				value = pDoc->InputImg[y][x] * 0.5;
				if (value > 255)value = 255;
				else if (value < 0) value = 0;

				pDoc->ResultImg[y][x] = value;
			}
			else
			{
				value = pDoc->InputImg[y][3 * x + 0] * 0.5;
				if (value > 255)value = 255;
				else if (value < 0) value = 0;

				pDoc->ResultImg[y][3 * x + 0] = value;


				value = pDoc->InputImg[y][3 * x + 1] * 0.5;
				if (value > 255)value = 255;
				else if (value < 0) value = 0;

				pDoc->ResultImg[y][3 * x + 1] = value;


				value = pDoc->InputImg[y][3 * x + 2] * 0.5;
				if (value > 255)value = 255;
				else if (value < 0) value = 0;

				pDoc->ResultImg[y][3 * x + 2] = value;
			}

		}

	Invalidate();
}


void Cimgproc20181369View::OnPixelHistoEq()  //히스토그램 평활화 함수
{
	Cimgproc20181369Doc* pDoc = GetDocument();

	int i, x, y, k;
	int acc_hist;
	int N = (pDoc->imageHeight) * (pDoc->imageWidth);
	int hist[256], sum[256];

	for (i = 0; i < 256; i++)
		hist[i] = 0;

	//히스토그램 구하기
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			k = pDoc->InputImg[y][x];
			hist[k]++;
		}

	//누적 분포 구하기
	acc_hist = 0;
	for (i = 0; i < 256; i++)
	{
		acc_hist += hist[i];
		sum[i] = acc_hist;
	}

	//픽셀 밝기 변환
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			k = pDoc->InputImg[y][x];
			pDoc->ResultImg[y][x] = (float)sum[k] / N * 255;
		}

	Invalidate();

}


void Cimgproc20181369View::OnPixelStrech() {
	Cimgproc20181369Doc* pDoc = GetDocument();

	int i, x, y, k;
	int min, max;

	//최소값과 최대값 찾기
	min = 255;
	max = 0;
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			k = pDoc->InputImg[y][x];
			if (k < min)
				min = k;
			else if (k > max)
				max = k;
		}

	//픽셀 밝기 변환
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			k = pDoc->InputImg[y][x];
			pDoc->ResultImg[y][x] = (float)(k - min) / (max - min) * 255;
		}

	Invalidate();

	//Cimgproc20181369Doc* pDoc = GetDocument();

	//int x, y;
	//int min_val = 255;
	//int max_val = 0;

	//// 최소값과 최대값 찾기
	//for (y = 0; y < pDoc->imageHeight; y++) {
	//	for (x = 0; x < pDoc->imageWidth; x++) {
	//		int pixel_value = pDoc->InputImg[y][x];
	//		if (pixel_value < min_val) {
	//			min_val = pixel_value;
	//		}
	//		if (pixel_value > max_val) {
	//			max_val = pixel_value;
	//		}
	//	}
	//}

	//// 명암대비 스트레칭 적용
	//for (y = 0; y < pDoc->imageHeight; y++) {
	//	for (x = 0; x < pDoc->imageWidth; x++) {
	//		int pixel_value = pDoc->InputImg[y][x];
	//		float stretched_value = ((float)(pixel_value - min_val) / (max_val - min_val)) * 255;
	//		pDoc->ResultImg[y][x] = stretched_value;
	//	}
	//}

	//Invalidate();
}

void Cimgproc20181369View::OnPixelBinarization()
{
	Cimgproc20181369Doc* pDoc = GetDocument();
	int x, y;
	int threshold = 100;

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			if (pDoc->InputImg[y][x] > threshold)
				pDoc->ResultImg[y][x] = 255;
			else
				pDoc->ResultImg[y][x] = 0;
		}

	Invalidate();

}


void Cimgproc20181369View::LoadTwoImage()
{
	Cimgproc20181369Doc* pDoc = GetDocument();
	CFileDialog dlg(TRUE);

	if (dlg.DoModal() == IDOK)   //IDOK는 "OK" 버튼의 ID
	{
		CFile file;
		file.Open(dlg.GetPathName(), CFile::modeRead);  //파일열기 (파일을 읽어올수 있는 상태로 만들어줌)
		CArchive ar(&file, CArchive::load);
		pDoc->LoadSecondImageFile(ar);
		//file.Read((pDoc->Inputimg2), 256 * 256);         //파일읽기
		file.Close();
	}
}


void Cimgproc20181369View::OnPixelTwoImageAdd()
{
	Cimgproc20181369Doc* pDoc = GetDocument();

	LoadTwoImage();

	int x, y, value;

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			if (pDoc->depth == 1)
			{
				value = 0.5 * pDoc->InputImg[y][x] + 0.5 * pDoc->InputImg2[y][x];

				if (value > 255)value = 255;
				else if (value < 0) value = 0;

				pDoc->ResultImg[y][x] = value;
			}
			else
			{
				pDoc->ResultImg[y][3 * x + 0] = 0.5 * pDoc->InputImg[y][3 * x + 0] + 0.5 * pDoc->InputImg2[y][3 * x + 0];
				pDoc->ResultImg[y][3 * x + 1] = 0.5 * pDoc->InputImg[y][3 * x + 1] + 0.5 * pDoc->InputImg2[y][3 * x + 1];
				pDoc->ResultImg[y][3 * x + 2] = 0.5 * pDoc->InputImg[y][3 * x + 2] + 0.5 * pDoc->InputImg2[y][3 * x + 2];
			}

		}

	Invalidate();
}


void Cimgproc20181369View::OnPixelTwoImageSub()
{
	Cimgproc20181369Doc* pDoc = GetDocument();

	LoadTwoImage();

	int x, y, value;

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			if (pDoc->depth == 1)
			{
				value = pDoc->InputImg[y][x] - pDoc->InputImg2[y][x];

				if (value > 255)value = 255;
				else if (value < 0) value = 0;

				pDoc->ResultImg[y][x] = value;
			}
			else
			{
				pDoc->ResultImg[y][3 * x + 0] = pDoc->InputImg[y][3 * x + 0] - pDoc->InputImg2[y][3 * x + 0];
				if (pDoc->InputImg[y][3 * x + 0] - pDoc->InputImg2[y][3 * x + 0] < 0)
					pDoc->ResultImg[y][3 * x + 0] = 0;
				pDoc->ResultImg[y][3 * x + 1] = pDoc->InputImg[y][3 * x + 1] - pDoc->InputImg2[y][3 * x + 1];
				if (pDoc->InputImg[y][3 * x + 1] - pDoc->InputImg2[y][3 * x + 1] < 0)
					pDoc->ResultImg[y][3 * x + 1] = 0;
				pDoc->ResultImg[y][3 * x + 2] = pDoc->InputImg[y][3 * x + 2] - pDoc->InputImg2[y][3 * x + 2];
				if (pDoc->InputImg[y][3 * x + 2] - pDoc->InputImg2[y][3 * x + 2] < 0)
					pDoc->ResultImg[y][3 * x + 2] = 0;
			}

		}

	Invalidate();

}


void Cimgproc20181369View::convolve(unsigned char** inImg, unsigned char** outImg, int cols, int rows, float mask[][3], int bias, int depth)
{
	int x, y;
	int i, j; //마스크의 위치를 순환하기 위한 변수
	int sum, rsum, gsum, bsum; //정수부분만 필요해서 int

	for (y = 1; y < rows - 1; y++)
		for (x = 1; x < cols - 1; x++) // 1부터 시작해서 -1까지 하면 테두리는 계산안함
		{
			if (depth == 1) // 흑백
			{
				sum = 0;
				for (j = 0; j < 3; j++)
					for (i = 0; i < 3; i++)
					{
						sum += (inImg[y + j - 1][x + i - 1] * mask[j][i]);
					}

				sum += bias;

				if (sum > 255)
					sum = 255;
				else if (sum < 0)
					sum = 0;

				outImg[y][x] = sum;
			}
			else // 컬러
			{
				rsum = 0;
				gsum = 0;
				bsum = 0;

				for (j = 0; j < 3; j++)
					for (i = 0; i < 3; i++)
					{
						rsum += (inImg[y + j - 1][3 * (x + i - 1) + 0] * mask[j][i]);
						gsum += (inImg[y + j - 1][3 * (x + i - 1) + 1] * mask[j][i]);
						bsum += (inImg[y + j - 1][3 * (x + i - 1) + 2] * mask[j][i]);
					}

				rsum += bias;
				gsum += bias;
				bsum += bias;

				if (rsum > 255)
					rsum = 255;
				else if (rsum < 0)
					rsum = 0;

				if (gsum > 255)
					gsum = 255;
				else if (gsum < 0)
					gsum = 0;

				if (bsum > 255)
					bsum = 255;
				else if (bsum < 0)
					bsum = 0;

				outImg[y][3 * x + 0] = rsum;
				outImg[y][3 * x + 1] = gsum;
				outImg[y][3 * x + 2] = bsum;
			}


		}
}


void Cimgproc20181369View::OnRegionSharpening()
{
	Cimgproc20181369Doc* pDoc = GetDocument();

	float kernel[3][3] = { {0,-1,0},
						   {-1,5,-1},
						   {0,-1,0} };  //kernel = 마스크 3x3배열

	convolve(pDoc->InputImg, pDoc->ResultImg, pDoc->imageWidth, pDoc->imageHeight, kernel, 0, pDoc->depth);

	Invalidate();
}


void Cimgproc20181369View::OnRegionSmoothing()
{
	Cimgproc20181369Doc* pDoc = GetDocument();

	float kernel[3][3] = { {1.0 / 9, 1.0 / 9, 1.0 / 9},
						   {1.0 / 9, 1.0 / 9, 1.0 / 9},
						   {1.0 / 9, 1.0 / 9, 1.0 / 9} };  //kernel = 마스크 3x3배열

	convolve(pDoc->InputImg, pDoc->ResultImg, pDoc->imageWidth, pDoc->imageHeight, kernel, 0, pDoc->depth);

	Invalidate();
}


void Cimgproc20181369View::OnRegionEmbossing()
{
	Cimgproc20181369Doc* pDoc = GetDocument();

	float kernel[3][3] = { {-1, 0 ,0},
							{0, 0, 0},
							{0, 0, 1} };  //kernel = 마스크 3x3배열

	convolve(pDoc->InputImg, pDoc->ResultImg, pDoc->imageWidth, pDoc->imageHeight, kernel, 0, pDoc->depth);

	Invalidate();
}



void Cimgproc20181369View::OnRegionPrewitt()
{
	Cimgproc20181369Doc* pDoc = GetDocument();

	float kernel_hori[3][3] = { {-1, -1 ,-1},
							{0, 0, 0},
							{1, 1, 1} };  //kernel = 마스크 3x3배열

	float kernel_verti[3][3] = { {-1, 0 ,1},
								{-1, 0, 1},
								{-1, 1, 1} };

	int x, y, i;
	int value, valueR, valueG, valueB;

	unsigned char** Er, ** Ec;    //Er과 Ec값을 저장할 새 변수
	//메모리 할당
	Er = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
	Ec = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));

	for (i = 0; i < pDoc->imageHeight; i++)
	{
		Er[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
		Ec[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
	}

	convolve(pDoc->InputImg, Er, pDoc->imageWidth, pDoc->imageHeight, kernel_hori, 128, pDoc->depth);
	convolve(pDoc->InputImg, Ec, pDoc->imageWidth, pDoc->imageHeight, kernel_verti, 128, pDoc->depth);

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			if (pDoc->depth == 1) //흑백일때
			{
				value = sqrt((Er[y][x] - 128) * (Er[y][x] - 128) + (Ec[y][x] - 128) * (Ec[y][x] - 128));  //sqrt = 루트
				if (value > 255)
					value = 255;
				else if (value < 0)
					value = 0;
				pDoc->ResultImg[y][x] = value;
			}
			else  //컬러일때
			{
				valueR = sqrt((Er[y][3 * x + 0] - 128) * (Er[y][3 * x + 0] - 128) + (Ec[y][3 * x + 0] - 128) * (Ec[y][3 * x + 0] - 128));  //RED
				valueG = sqrt((Er[y][3 * x + 1] - 128) * (Er[y][3 * x + 1] - 128) + (Ec[y][3 * x + 1] - 128) * (Ec[y][3 * x + 1] - 128));  //GREEN
				valueB = sqrt((Er[y][3 * x + 2] - 128) * (Er[y][3 * x + 2] - 128) + (Ec[y][3 * x + 2] - 128) * (Ec[y][3 * x + 2] - 128));  //BLUE

				value = sqrt(valueR * valueR + valueG * valueG + valueB * valueB);
				if (value > 255)
					value = 255;
				else if (value < 0)
					value = 0;

				pDoc->ResultImg[y][3 * x + 0] = value;
				pDoc->ResultImg[y][3 * x + 1] = value;
				pDoc->ResultImg[y][3 * x + 2] = value;
			}
		}

	//메모리 할당 해제(삭제)
	for (i = 0; i < pDoc->imageHeight; i++) //free() 함수는 동적으로 할당된 메모리를 해제하는 데 사용
	{
		free(Er[i]);
		free(Ec[i]);
	}
	free(Er);
	free(Ec);

	Invalidate();
}


void Cimgproc20181369View::OnRegionRoberts()
{
	Cimgproc20181369Doc* pDoc = GetDocument();

	float kernel_hori[3][3] = { {-1, 0 ,0},
								{0, 1, 0},
								{0, 0, 0} };  //kernel = 마스크 3x3배열

	float kernel_verti[3][3] = { {0, 0 ,-1},
								{0, 1, 0},
								{0, 0, 0} };

	int x, y, i;
	int value, valueR, valueG, valueB;

	unsigned char** Er, ** Ec;    //Er과 Ec값을 저장할 새 변수
	//메모리 할당
	Er = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
	Ec = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));

	for (i = 0; i < pDoc->imageHeight; i++)
	{
		Er[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
		Ec[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
	}

	convolve(pDoc->InputImg, Er, pDoc->imageWidth, pDoc->imageHeight, kernel_hori, 128, pDoc->depth);
	convolve(pDoc->InputImg, Ec, pDoc->imageWidth, pDoc->imageHeight, kernel_verti, 128, pDoc->depth);

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			if (pDoc->depth == 1) //흑백일때
			{
				value = sqrt((Er[y][x] - 128) * (Er[y][x] - 128) + (Ec[y][x] - 128) * (Ec[y][x] - 128));  //sqrt = 루트
				if (value > 255)
					value = 255;
				else if (value < 0)
					value = 0;
				pDoc->ResultImg[y][x] = value;
			}
			else  //컬러일때
			{
				valueR = sqrt((Er[y][3 * x + 0] - 128) * (Er[y][3 * x + 0] - 128) + (Ec[y][3 * x + 0] - 128) * (Ec[y][3 * x + 0] - 128));  //RED
				valueG = sqrt((Er[y][3 * x + 1] - 128) * (Er[y][3 * x + 1] - 128) + (Ec[y][3 * x + 1] - 128) * (Ec[y][3 * x + 1] - 128));  //GREEN
				valueB = sqrt((Er[y][3 * x + 2] - 128) * (Er[y][3 * x + 2] - 128) + (Ec[y][3 * x + 2] - 128) * (Ec[y][3 * x + 2] - 128));  //BLUE

				value = sqrt(valueR * valueR + valueG * valueG + valueB * valueB);
				if (value > 255)
					value = 255;
				else if (value < 0)
					value = 0;

				pDoc->ResultImg[y][3 * x + 0] = value;
				pDoc->ResultImg[y][3 * x + 1] = value;
				pDoc->ResultImg[y][3 * x + 2] = value;
			}
		}

	//메모리 할당 해제(삭제)
	for (i = 0; i < pDoc->imageHeight; i++) //free() 함수는 동적으로 할당된 메모리를 해제하는 데 사용
	{
		free(Er[i]);
		free(Ec[i]);
	}
	free(Er);
	free(Ec);

	Invalidate();
}


void Cimgproc20181369View::OnRegionSobel()
{
	Cimgproc20181369Doc* pDoc = GetDocument();

	float kernel_hori[3][3] = { {-1, -2 ,-1},
								{0, 0, 0},
								{1, 2, 1} };  //kernel = 마스크 3x3배열

	float kernel_verti[3][3] = { {-1, 0 ,1},
								{-2, 0, 2},
								{-1, 1, 1} };

	int x, y, i;
	int value, valueR, valueG, valueB;

	unsigned char** Er, ** Ec;    //Er과 Ec값을 저장할 새 변수
	//메모리 할당
	Er = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
	Ec = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));

	for (i = 0; i < pDoc->imageHeight; i++)
	{
		Er[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
		Ec[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
	}

	convolve(pDoc->InputImg, Er, pDoc->imageWidth, pDoc->imageHeight, kernel_hori, 128, pDoc->depth);
	convolve(pDoc->InputImg, Ec, pDoc->imageWidth, pDoc->imageHeight, kernel_verti, 128, pDoc->depth);

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			if (pDoc->depth == 1) //흑백일때
			{
				value = sqrt((Er[y][x] - 128) * (Er[y][x] - 128) + (Ec[y][x] - 128) * (Ec[y][x] - 128));  //sqrt = 루트
				if (value > 255)
					value = 255;
				else if (value < 0)
					value = 0;
				pDoc->ResultImg[y][x] = value;
			}
			else  //컬러일때
			{
				valueR = sqrt((Er[y][3 * x + 0] - 128) * (Er[y][3 * x + 0] - 128) + (Ec[y][3 * x + 0] - 128) * (Ec[y][3 * x + 0] - 128));  //RED
				valueG = sqrt((Er[y][3 * x + 1] - 128) * (Er[y][3 * x + 1] - 128) + (Ec[y][3 * x + 1] - 128) * (Ec[y][3 * x + 1] - 128));  //GREEN
				valueB = sqrt((Er[y][3 * x + 2] - 128) * (Er[y][3 * x + 2] - 128) + (Ec[y][3 * x + 2] - 128) * (Ec[y][3 * x + 2] - 128));  //BLUE

				value = sqrt(valueR * valueR + valueG * valueG + valueB * valueB);
				if (value > 255)
					value = 255;
				else if (value < 0)
					value = 0;

				pDoc->ResultImg[y][3 * x + 0] = value;
				pDoc->ResultImg[y][3 * x + 1] = value;
				pDoc->ResultImg[y][3 * x + 2] = value;
			}
		}

	//메모리 할당 해제(삭제)
	for (i = 0; i < pDoc->imageHeight; i++) //free() 함수는 동적으로 할당된 메모리를 해제하는 데 사용
	{
		free(Er[i]);
		free(Ec[i]);
	}
	free(Er);
	free(Ec);

	Invalidate();
}


void Cimgproc20181369View::OnRegionAverageFiltering()
{
	Cimgproc20181369Doc* pDoc = GetDocument();

	int x, y, i, j;
	int xpos, ypos;
	int sum, sumR, sumG, sumB;
	int pixelCount;

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			sum = 0;
			sumR = 0;
			sumG = 0;
			sumB = 0;
			pixelCount = 0;

			for (j = -2; j <= 2; j++)
				for (i = -2; i <= 2; i++)
				{
					xpos = x + i;
					ypos = y + j;
					if (xpos >= 0 && xpos <= pDoc->imageWidth - 1 && ypos >= 0 && ypos <= pDoc->imageHeight - 1)
					{
						if (pDoc->depth == 1)
							sum += pDoc->InputImg[y + j][x + i];
						else
						{
							sumR += pDoc->InputImg[y + j][3 * (x + i) + 0];
							sumG += pDoc->InputImg[y + j][3 * (x + i) + 1];
							sumB += pDoc->InputImg[y + j][3 * (x + i) + 2];
						}
						pixelCount++;
					}
				}
			if (pDoc->depth == 1)
				pDoc->ResultImg[y][x] = sum / pixelCount;
			else
			{
				pDoc->ResultImg[y][3 * x + 0] = sumR / pixelCount;
				pDoc->ResultImg[y][3 * x + 1] = sumG / pixelCount;
				pDoc->ResultImg[y][3 * x + 2] = sumB / pixelCount;
			}

		}

	Invalidate();
}


void Cimgproc20181369View::OnRegionMedianFiltering()
{
	Cimgproc20181369Doc* pDoc = GetDocument();

	int x, y, i, j, k;
	int n[9], temp;

	if (pDoc->depth == 1)
	{
		for (y = 1; y < pDoc->imageHeight - 1; y++)
			for (x = 1; x < pDoc->imageWidth - 1; x++)
			{
				n[0] = pDoc->InputImg[y - 1][x - 1];
				n[1] = pDoc->InputImg[y - 1][x - 0];
				n[2] = pDoc->InputImg[y - 1][x + 1];
				n[3] = pDoc->InputImg[y - 0][x - 1];
				n[4] = pDoc->InputImg[y - 0][x - 0];
				n[5] = pDoc->InputImg[y - 0][x + 1];
				n[6] = pDoc->InputImg[y + 1][x - 1];
				n[7] = pDoc->InputImg[y + 1][x - 0];
				n[8] = pDoc->InputImg[y + 1][x + 1];

				//버블소팅(오름차순)
				for (i = 8; i > 0; i--)
					for (j = 0; j < i; j++)
					{
						if (n[j] > n[j + 1])
						{
							temp = n[j + 1];
							n[j + 1] = n[j];
							n[j] = temp;
						}
					}

				pDoc->ResultImg[y][x] = n[4];

			}
	}
	else
	{
		for (k = 0; k < 3; k++)
			for (y = 1; y < pDoc->imageHeight - 1; y++)
				for (x = 1; x < pDoc->imageWidth - 1; x++)
				{
					n[0] = pDoc->InputImg[y - 1][3 * (x - 1) + k];
					n[1] = pDoc->InputImg[y - 1][3 * (x - 0) + k];
					n[2] = pDoc->InputImg[y - 1][3 * (x + 1) + k];
					n[3] = pDoc->InputImg[y - 0][3 * (x - 1) + k];
					n[4] = pDoc->InputImg[y - 0][3 * (x - 0) + k];
					n[5] = pDoc->InputImg[y - 0][3 * (x + 1) + k];
					n[6] = pDoc->InputImg[y + 1][3 * (x - 1) + k];
					n[7] = pDoc->InputImg[y + 1][3 * (x - 0) + k];
					n[8] = pDoc->InputImg[y + 1][3 * (x + 1) + k];

					//버블소팅(오름차순)
					for (i = 8; i > 0; i--)
						for (j = 0; j < i; j++)
						{
							if (n[j] > n[j + 1])
							{
								temp = n[j + 1];
								n[j + 1] = n[j];
								n[j] = temp;
							}
						}

					pDoc->ResultImg[y][3 *x + k] = n[4];
				}
	}
	Invalidate();
}


void Cimgproc20181369View::OnMopologyColorToGray()
{
	Cimgproc20181369Doc* pDoc = GetDocument();
	if (pDoc->depth == 1)
		return;

	int x, y;
	int gray;

	for (y = 0; y < pDoc->imageHeight - 1; y++)
		for (x = 0; x < pDoc->imageWidth - 1; x++)
		{
			gray = (pDoc->InputImg[y][3 * x + 0] + pDoc->InputImg[y][3 * x + 1] + pDoc->InputImg[y][3 * x + 2]) / 3;
			pDoc->InputImg[y][3 * x + 0] = gray;
			pDoc->InputImg[y][3 * x + 1] = gray;
			pDoc->InputImg[y][3 * x + 2] = gray;
		}

	Invalidate();
}


void Cimgproc20181369View::OnMopologyBinarization()
{
	Cimgproc20181369Doc* pDoc = GetDocument();

	int x, y;
	int gray, threshold = 100;

	for (y = 0; y < pDoc->imageHeight - 1; y++)
		for (x = 0; x < pDoc->imageWidth - 1; x++)
		{
			if (pDoc->depth == 1)
			{
				if (pDoc->InputImg[y][x] > threshold)
					pDoc->InputImg[y][x] = 255;
				else
					pDoc->InputImg[y][x] = 0;
			}
			else
			{
				if ((pDoc->InputImg[y][3 * x + 0] + pDoc->InputImg[y][3 * x + 1] + pDoc->InputImg[y][3 * x + 2]) / 3 > threshold)
				{
					pDoc->InputImg[y][3 * x + 0] = 255;
					pDoc->InputImg[y][3 * x + 1] = 255;
					pDoc->InputImg[y][3 * x + 2] = 255;
				}
				else
				{
					pDoc->InputImg[y][3 * x + 0] = 0;
					pDoc->InputImg[y][3 * x + 1] = 0;
					pDoc->InputImg[y][3 * x + 2] = 0;
				}
			}

		}

	Invalidate();
}


void Cimgproc20181369View::OnMopologyErosion()
{
	Cimgproc20181369Doc* pDoc = GetDocument();

	int x, y, i, j;
	int min = 255;
	int minR, minG, minB;

	for (y = 1; y < pDoc->imageHeight - 1; y++)
		for (x = 1; x < pDoc->imageWidth - 1; x++)
		{
			min = 255; minR = 255; minG = 255; minB = 255;
			for (j = -1; j <= 1; j++)
				for (i = -1; i <= 1; i++)
				{
					if (pDoc->depth == 1)
					{
						if (pDoc->InputImg[y + j][x + i] < min)
							min = pDoc->InputImg[y + j][x + i];
					}
					else
					{
						if (pDoc->InputImg[y + j][3 * (x + i) + 0] < minR)
							minR = pDoc->InputImg[y + j][3 * (x + i) + 0];

						if (pDoc->InputImg[y + j][3 * (x + i) + 1] < minG)
							minG = pDoc->InputImg[y + j][3 * (x + i) + 1];

						if (pDoc->InputImg[y + j][3 * (x + i) + 2] < minB)
							minB = pDoc->InputImg[y + j][3 * (x + i) + 2];
					}
				}

			if (pDoc->depth == 1)
				pDoc->ResultImg[y][x] = min;
			else
			{
				pDoc->ResultImg[y][3 * x + 0] = minR;
				pDoc->ResultImg[y][3 * x + 1] = minG;
				pDoc->ResultImg[y][3 * x + 2] = minB;
			}
		}

	Invalidate();
}


void Cimgproc20181369View::OnMopologyDilation()
{
	Cimgproc20181369Doc* pDoc = GetDocument();

	int x, y, i, j;
	int max, maxR, maxG, maxB;

	for (y = 1; y < pDoc->imageHeight - 1; y++)
		for (x = 1; x < pDoc->imageWidth - 1; x++)
		{
			max = 0; maxR = 0; maxG = 0; maxB = 0;
			for (j = -1; j <= 1; j++)
				for (i = -1; i <= 1; i++)
				{
					if (pDoc->depth == 1)
					{
						if (pDoc->InputImg[y + j][x + i] > max)
							max = pDoc->InputImg[y + j][x + i];
					}
					else
					{
						if (pDoc->InputImg[y + j][3 * (x + i) + 0] > maxR)
							maxR = pDoc->InputImg[y + j][3 * (x + i) + 0];

						if (pDoc->InputImg[y + j][3 * (x + i) + 1] > maxG)
							maxG = pDoc->InputImg[y + j][3 * (x + i) + 1];

						if (pDoc->InputImg[y + j][3 * (x + i) + 2] > maxB)
							maxB = pDoc->InputImg[y + j][3 * (x + i) + 2];
					}
				}

			if (pDoc->depth == 1)
				pDoc->ResultImg[y][x] = max;
			else
			{
				pDoc->ResultImg[y][3 * x + 0] = maxR;
				pDoc->ResultImg[y][3 * x + 1] = maxG;
				pDoc->ResultImg[y][3 * x + 2] = maxB;
			}
		}

	Invalidate();
}


void Cimgproc20181369View::CopyResultToInput()
{
	Cimgproc20181369Doc* pDoc = GetDocument();

	int x, y;

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++)
		{
			pDoc->InputImg[y][x] = pDoc->ResultImg[y][x];
		}
}


void Cimgproc20181369View::OnMopologyOpening()
{
	OnMopologyErosion();
	CopyResultToInput();
	OnMopologyErosion();
	CopyResultToInput();
	OnMopologyErosion();

	CopyResultToInput();
	OnMopologyDilation();
	CopyResultToInput();
	OnMopologyDilation();
	CopyResultToInput();
	OnMopologyDilation();

	Invalidate();

}


void Cimgproc20181369View::OnMopologyClosing()
{
	OnMopologyDilation();
	CopyResultToInput();
	OnMopologyDilation();
	CopyResultToInput();
	OnMopologyDilation();

	CopyResultToInput();
	OnMopologyErosion();
	CopyResultToInput();
	OnMopologyErosion();
	CopyResultToInput();
	OnMopologyErosion();

	Invalidate();
}


void Cimgproc20181369View::OnGeometryZoominPixelCopy()
{
	Cimgproc20181369Doc* pDoc = GetDocument();
	int i, x, y;
	int xscale = 3;
	int yscale = 2;

	if (pDoc->gResultImg != NULL)
	{
		for (i = 0; i < pDoc->gImageHeight; i++)
			free(pDoc->gResultImg[i]); //이거 먼저 없애주고
		free(pDoc->gResultImg);  // 다음에 이걸 제거
	}

	pDoc->gImageWidth = pDoc->imageWidth * xscale;
	pDoc->gImageHeight = pDoc->imageHeight * yscale;
	//메모리 할당
	pDoc->gResultImg = (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
	for (i = 0; i < pDoc->gImageHeight; i++)
	{
		pDoc->gResultImg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);
	}

	////전방향 사상
	//for (y = 0; y < pDoc->imageHeight; y++)
	//	for (x = 0; x < pDoc->imageWidth; x++)
	//	{
	//		pDoc->gResultImg[y * yscale][x * xscale] = pDoc->InputImg[y][x];
	//	}

	//역방향 사상
	for (y = 0; y < pDoc->gImageHeight; y++)
		for (x = 0; x < pDoc->gImageWidth; x++)
		{
			if (pDoc->depth == 1)
				pDoc->gResultImg[y][x] = pDoc->InputImg[y / yscale][x / xscale];
			else
			{
				pDoc->gResultImg[y][3 * x + 0] = pDoc->InputImg[y / yscale][3 * (x / xscale) + 0];
				pDoc->gResultImg[y][3 * x + 1] = pDoc->InputImg[y / yscale][3 * (x / xscale) + 1];
				pDoc->gResultImg[y][3 * x + 2] = pDoc->InputImg[y / yscale][3 * (x / xscale) + 2];
			}
		}

	Invalidate();
}

void Cimgproc20181369View::OnGeometryZoominBilinearInterpolation()
{
	Cimgproc20181369Doc* pDoc = GetDocument();

	int i, x, y;
	float xscale = 2.1;
	float yscale = 1.5;
	float src_x, src_y;
	float alpha, beta;
	int Ax, Ay, Bx, By, Cx, Cy, Dx, Dy;
	int E, F;

	if (pDoc->gResultImg != NULL)
	{
		for (i = 0; i < pDoc->gImageHeight; i++)
			free(pDoc->gResultImg[i]); //이거 먼저 없애주고
		free(pDoc->gResultImg);  // 다음에 이걸 제거
	}

	pDoc->gImageWidth = pDoc->imageWidth * xscale;
	pDoc->gImageHeight = pDoc->imageHeight * yscale;
	//메모리 할당
	pDoc->gResultImg = (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
	for (i = 0; i < pDoc->gImageHeight; i++)
	{
		pDoc->gResultImg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);
	}

	//역방향 사상
	for (y = 0; y < pDoc->gImageHeight; y++)
		for (x = 0; x < pDoc->gImageWidth; x++)
		{
			src_x = x / xscale;
			src_y = y / yscale;
			alpha = src_x - (int)src_x;
			beta = src_y - (int)src_y;

			Ax = (int)src_x;
			Ay = (int)src_y;
			Bx = Ax + 1;
			By = Ay;
			Cx = Ax;
			Cy = Ay + 1;
			Dx = Ax + 1;
			Dy = Ay + 1;

			if (Bx > pDoc->imageWidth - 1)     Bx = pDoc->imageWidth - 1;
			if (Cy > pDoc->imageWidth - 1)     Cy = pDoc->imageWidth - 1;
			if (Dx > pDoc->imageWidth - 1) 	   Dx = pDoc->imageWidth - 1;
			if (Dy > pDoc->imageWidth - 1) 	   Dy = pDoc->imageWidth - 1;

			if (pDoc->depth == 1)
			{
				E = (1 - alpha) * pDoc->InputImg[Ay][Ax] + alpha * pDoc->InputImg[By][Bx];
				F = (1 - alpha) * pDoc->InputImg[Cy][Cx] + alpha * pDoc->InputImg[Dy][Dx];

				pDoc->gResultImg[y][x] = (1 - beta) * E + beta * F;
			}
			else
			{
				E = (1 - alpha) * pDoc->InputImg[Ay][3 * Ax + 0] + alpha * pDoc->InputImg[By][3 * Bx + 0];
				F = (1 - alpha) * pDoc->InputImg[Cy][3 * Cx + 0] + alpha * pDoc->InputImg[Dy][3 * Dx + 0];

				pDoc->gResultImg[y][3 * x + 0] = (1 - beta) * E + beta * F;

				E = (1 - alpha) * pDoc->InputImg[Ay][3 * Ax + 1] + alpha * pDoc->InputImg[By][3 * Bx + 1];
				F = (1 - alpha) * pDoc->InputImg[Cy][3 * Cx + 1] + alpha * pDoc->InputImg[Dy][3 * Dx + 1];

				pDoc->gResultImg[y][3 * x + 1] = (1 - beta) * E + beta * F;

				E = (1 - alpha) * pDoc->InputImg[Ay][3 * Ax + 2] + alpha * pDoc->InputImg[By][3 * Bx + 2];
				F = (1 - alpha) * pDoc->InputImg[Cy][3 * Cx + 2] + alpha * pDoc->InputImg[Dy][3 * Dx + 2];

				pDoc->gResultImg[y][3 * x + 2] = (1 - beta) * E + beta * F;
			}

		}

	Invalidate();

}


void Cimgproc20181369View::OnGeometryZoomoutSubSampling()
{
	Cimgproc20181369Doc* pDoc = GetDocument();
	int x, y, i;

	int xscale = 3; // 3분의 1로 축소
	int yscale = 2; // 2분의 1로 축소

	if (pDoc->gResultImg != NULL)
	{
		for (i = 0; i < pDoc->gImageHeight; i++)
			free(pDoc->gResultImg[i]); //이거 먼저 없애주고
		free(pDoc->gResultImg);  // 다음에 이걸 제거
	}

	pDoc->gImageWidth = (pDoc->imageWidth) / xscale;
	pDoc->gImageHeight = (pDoc->imageHeight) / yscale;

	//메모리할당
	pDoc->gResultImg = (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
	for (i = 0; i < pDoc->gImageHeight; i++)
	{
		pDoc->gResultImg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);
	}

	//역방향 사상
	for (y = 0; y < pDoc->gImageHeight; y++)
		for (x = 0; x < pDoc->gImageWidth; x++)
		{
			if (pDoc->depth == 1)
				pDoc->gResultImg[y][x] = pDoc->InputImg[y * yscale][x * xscale];
			else
			{
				pDoc->gResultImg[y][3 * x + 0] = pDoc->InputImg[y * yscale][3 * (x * xscale) + 0];
				pDoc->gResultImg[y][3 * x + 1] = pDoc->InputImg[y * yscale][3 * (x * xscale) + 1];
				pDoc->gResultImg[y][3 * x + 2] = pDoc->InputImg[y * yscale][3 * (x * xscale) + 2];
			}
		}
	Invalidate();
}


void Cimgproc20181369View::OnGeometryZoomoutMeanSubSampling()
{
	OnRegionSmoothing();
	CopyResultToInput();
	OnGeometryZoomoutSubSampling();
}


void Cimgproc20181369View::OnGeometryAvgSampling()
{
	Cimgproc20181369Doc* pDoc = GetDocument();
	int x, y, i, j;
	int sum, sumR, sumG, sumB;

	int xscale = 3; // 3분의 1로 축소
	int yscale = 2; // 2분의 1로 축소
	int src_x, src_y;

	if (pDoc->gResultImg != NULL)
	{
		for (i = 0; i < pDoc->gImageHeight; i++)
			free(pDoc->gResultImg[i]); //이거 먼저 없애주고
		free(pDoc->gResultImg);  // 다음에 이걸 제거
	}

	pDoc->gImageWidth = (pDoc->imageWidth) / xscale;
	pDoc->gImageHeight = (pDoc->imageHeight) / yscale;

	//메모리할당
	pDoc->gResultImg = (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
	for (i = 0; i < pDoc->gImageHeight; i++)
	{
		pDoc->gResultImg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);
	}

	//전방향 사상
	for (y = 0; y < pDoc->imageHeight; y += yscale)
		for (x = 0; x < pDoc->imageWidth; x += xscale)
		{
			sum = 0; sumR = 0; sumG = 0; sumB = 0;

			for (j = 0; j < yscale; j++)
				for (i = 0; i < xscale; i++)
				{
					src_x = x + i;
					src_y = y + j;

					if (src_x > pDoc->imageWidth - 1)        src_x = pDoc->imageWidth - 1;
					if (src_y > pDoc->imageHeight - 1)       src_y = pDoc->imageHeight - 1;

					if (pDoc->depth == 1)
						sum += pDoc->InputImg[src_y][src_x];
					else
					{
						sumR += pDoc->InputImg[src_y][3 * src_x + 0];
						sumG += pDoc->InputImg[src_y][3 * src_x + 1];
						sumB += pDoc->InputImg[src_y][3 * src_x + 2];
					}

				}
			if (pDoc->depth == 1)
				pDoc->gResultImg[y / yscale][x / xscale] = sum / (xscale * yscale);
			else
			{
				pDoc->gResultImg[y / yscale][3 * (x / xscale) + 0] = sumR / (xscale * yscale);
				pDoc->gResultImg[y / yscale][3 * (x / xscale) + 1] = sumG / (xscale * yscale);
				pDoc->gResultImg[y / yscale][3 * (x / xscale) + 2] = sumB / (xscale * yscale);
			}
		}

	Invalidate();
}


#define PI 3.14159265358979
void Cimgproc20181369View::OnGeometryRotation()
{
	Cimgproc20181369Doc* pDoc = GetDocument();
	CAngleInputDialog dlg;

	int x, y, i, j;
	int angle = 0;    //단위는 degree 각도 설정
	float radian;
	int Cx, Cy, Oy;
	int x_source, y_source, xdiff, ydiff;


	dlg.m_iAngle = angle;
	if (dlg.DoModal() == IDCANCEL)    return;
	angle = dlg.m_iAngle;

	if (pDoc->gResultImg != NULL)
	{
		for (i = 0; i < pDoc->gImageHeight; i++)
			free(pDoc->gResultImg[i]); //이거 먼저 없애주고
		free(pDoc->gResultImg);  // 다음에 이걸 제거
	}

	radian = 2 * PI / 360 * angle;//        fabs = 절대값으로 만드는 함수
	pDoc->gImageWidth = pDoc->imageHeight * fabs(cos(PI / 2 - radian)) + pDoc->imageWidth * cos(radian);
	pDoc->gImageHeight = pDoc->imageHeight * fabs(cos(radian)) + pDoc->imageWidth * fabs(cos(PI / 2 - radian));

	//메모리 할당
	pDoc->gResultImg = (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
	for (i = 0; i < pDoc->gImageHeight; i++)
	{
		pDoc->gResultImg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);
	}

	//중심점
	Cx = pDoc->imageWidth / 2;
	Cy = pDoc->imageHeight / 2;

	//y의 마지막 좌표
	Oy = pDoc->imageHeight - 1;
	xdiff = (pDoc->gImageWidth - pDoc->imageWidth) / 2;
	ydiff = (pDoc->gImageHeight - pDoc->imageHeight) / 2;

	for (y = -ydiff; y < pDoc->gImageHeight - ydiff; y++)
		for (x = -xdiff; x < pDoc->gImageWidth - xdiff; x++)
		{
			x_source = (Oy - y - Cy) * sin(radian) + (x - Cx) * cos(radian) + Cx;
			y_source = Oy - ((Oy - y - Cy) * cos(radian) - (x - Cx) * sin(radian) + Cy);

			if (pDoc->depth == 1)
			{
				if (x_source < 0 || x_source > pDoc->imageWidth - 1 ||
					y_source < 0 || y_source > pDoc->imageHeight - 1)
					pDoc->gResultImg[y + ydiff][x + xdiff] = 255; //빈영역 색상
				else
				{
					pDoc->gResultImg[y + ydiff][x + xdiff] = pDoc->InputImg[y_source][x_source];
				}
			}
			else
			{
				if (x_source < 0 || x_source > pDoc->imageWidth - 1 ||
					y_source < 0 || y_source > pDoc->imageHeight - 1)
				{
					pDoc->gResultImg[y + ydiff][3 * (x + xdiff) + 0] = 255;
					pDoc->gResultImg[y + ydiff][3 * (x + xdiff) + 1] = 255;
					pDoc->gResultImg[y + ydiff][3 * (x + xdiff) + 2] = 255;
				}
				else
				{
					pDoc->gResultImg[y + ydiff][3 * (x + xdiff) + 0] = pDoc->InputImg[y_source][3 * x_source + 0];
					pDoc->gResultImg[y + ydiff][3 * (x + xdiff) + 1] = pDoc->InputImg[y_source][3 * x_source + 1];
					pDoc->gResultImg[y + ydiff][3 * (x + xdiff) + 2] = pDoc->InputImg[y_source][3 * x_source + 2];
				}
			}

		}
	Invalidate();
}

void Cimgproc20181369View::OnGeometryMirror()
{
	Cimgproc20181369Doc* pDoc = GetDocument();
	int x, y;

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			if (pDoc->depth == 1)
				pDoc->ResultImg[y][x] = pDoc->InputImg[y][pDoc->imageWidth - 1 - x];
			else
			{
				pDoc->ResultImg[y][3 * x + 0] = pDoc->InputImg[y][3 * (pDoc->imageWidth - 1 - x) + 0];
				pDoc->ResultImg[y][3 * x + 1] = pDoc->InputImg[y][3 * (pDoc->imageWidth - 1 - x) + 1];
				pDoc->ResultImg[y][3 * x + 2] = pDoc->InputImg[y][3 * (pDoc->imageWidth - 1 - x) + 2];
			}
		}
	Invalidate();
}

void Cimgproc20181369View::OnGeometryFlip()
{
	Cimgproc20181369Doc* pDoc = GetDocument();
	int x, y;

	for (y = 0; y < pDoc->imageHeight; y++) {
		for (x = 0; x < pDoc->imageWidth; x++) {
			if (pDoc->depth == 1) {
				pDoc->ResultImg[pDoc->imageHeight - 1 - y][x] = pDoc->InputImg[y][x];
			}
			else {
				// 컬러 이미지 뒤집기
				pDoc->ResultImg[pDoc->imageHeight - 1 - y][3 * x + 0] = pDoc->InputImg[y][3 * x + 0];
				pDoc->ResultImg[pDoc->imageHeight - 1 - y][3 * x + 1] = pDoc->InputImg[y][3 * x + 1];
				pDoc->ResultImg[pDoc->imageHeight - 1 - y][3 * x + 2] = pDoc->InputImg[y][3 * x + 2];
				
			}
		}
	}
	Invalidate();
}


typedef struct
{
	int Px;
	int Py;
	int Qx;
	int Qy;
}control_line;

control_line mctrl_source = { 100, 100, 150, 150 };
control_line mctrl_dest = { 100, 100, 200, 200 };


void Cimgproc20181369View::OnGeometryWarping()
{
	Cimgproc20181369Doc* pDoc = GetDocument();

	control_line source_lines[5] = { {100,100,150,150},
		{0,0,pDoc->imageWidth - 1,0} ,{pDoc->imageWidth - 1,0,pDoc->imageWidth - 1,pDoc->imageHeight - 1} ,
		{pDoc->imageWidth - 1,pDoc->imageHeight - 1,0,pDoc->imageHeight - 1} ,{0,pDoc->imageHeight - 1,0,0} };
	control_line dest_lines[5] = { {100,100,200,200},
		{0,0,pDoc->imageWidth - 1,0} ,{pDoc->imageWidth - 1,0,pDoc->imageWidth - 1,pDoc->imageHeight - 1} ,
		{pDoc->imageWidth - 1,pDoc->imageHeight - 1,0,pDoc->imageHeight - 1} ,{0,pDoc->imageHeight - 1,0,0} };

	source_lines[0] = mctrl_source;
	dest_lines[0] = mctrl_dest;

	int x, y;

	double u;
	double h;
	double d;
	double tx, ty;
	double xp, yp;

	double weight;
	double totalweight;
	double a = 0.001;
	double b = 2.0;
	double p = 0.75;

	int x1, x2, y1, y2;
	int src_x1, src_y1, src_x2, src_y2;
	double src_line_length, dest_line_length;

	int num_lines = 5;
	int line;
	int source_x, source_y;
	int last_row, last_col;

	last_row = pDoc->imageHeight - 1;
	last_col = pDoc->imageWidth - 1;

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			tx = 0.0;
			ty = 0.0;
			totalweight = 0.0;
			//각제어선의 영향을 계산
			for (line = 0; line < num_lines; line++)
			{
				x1 = dest_lines[line].Px;
				y1 = dest_lines[line].Py;
				x2 = dest_lines[line].Qx;
				y2 = dest_lines[line].Qy;

				dest_line_length = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

				u = (double)((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1)) /
					(double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

				h = ((y - y1) * (x2 - x1) - (x - x1) * (y2 - y1)) / dest_line_length;

				if (u < 0)		d = sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
				else if (u > 1)	d = sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));
				else				d = fabs(h);

				src_x1 = source_lines[line].Px;
				src_y1 = source_lines[line].Py;
				src_x2 = source_lines[line].Qx;
				src_y2 = source_lines[line].Qy;

				src_line_length = sqrt((src_x2 - src_x1) * (src_x2 - src_x1) +
					(src_y2 - src_y1) * (src_y2 - src_y1));

				xp = src_x1 + u * (src_x2 - src_x1) - h * (src_y2 - src_y1) / src_line_length;
				yp = src_y1 + u * (src_y2 - src_y1) + h * (src_x2 - src_x1) / src_line_length;

				weight = pow(pow(dest_line_length, p) / (a + d), b);

				tx += (xp - x) * weight;
				ty += (yp - y) * weight;
				totalweight += weight;
			}
			source_x = x + (tx / totalweight);
			source_y = y + (ty / totalweight);

			if (source_x < 0)		source_x = 0;
			if (source_x > last_col)	source_x = last_col;
			if (source_y < 0)		source_y = 0;
			if (source_y > last_row)	source_y = last_row;

			if (pDoc->depth == 1)
				pDoc->ResultImg[y][x] = pDoc->InputImg[source_y][source_x];
			else
			{
				pDoc->ResultImg[y][3 * x + 0] = pDoc->InputImg[source_y][3 * source_x + 0];
				pDoc->ResultImg[y][3 * x + 1] = pDoc->InputImg[source_y][3 * source_x + 1];
				pDoc->ResultImg[y][3 * x + 2] = pDoc->InputImg[source_y][3 * source_x + 2];
			}
		}
	Invalidate();
}


CPoint mpos_st, mpos_end;

void Cimgproc20181369View::OnLButtonDown(UINT nFlags, CPoint point)
{
	mpos_st = point;

	CScrollView::OnLButtonDown(nFlags, point);
}


void Cimgproc20181369View::OnLButtonUp(UINT nFlags, CPoint point)
{
	mpos_end = point;

	CDC* pDC = GetDC();
	CPen rpen;
	rpen.CreatePen(PS_SOLID, 0, RGB(255, 0, 0));
	pDC->SelectObject(&rpen);

	pDC->MoveTo(mpos_st);
	pDC->LineTo(mpos_end);

	ReleaseDC(pDC);  //GetDC를 해제 해는 함수

	int Ax, Ay, Bx, By;
	Ax = mpos_st.x;
	Ay = mpos_st.y;
	Bx = mpos_end.x;
	By = mpos_end.y;

	if (Ax < Bx) mctrl_source.Px = Ax - (Bx - Ax) / 2;
	else         mctrl_source.Px = Ax + (Ax - Bx) / 2;

	if (Ay < By) mctrl_source.Py = Ay - (By - Ay) / 2;
	else         mctrl_source.Py = Ay + (Ay - By) / 2;

	mctrl_dest.Px = mctrl_source.Px;
	mctrl_dest.Py = mctrl_source.Py;

	mctrl_source.Qx = mpos_st.x;
	mctrl_source.Qy = mpos_st.y;
	mctrl_dest.Qx = mpos_end.x;
	mctrl_dest.Qy = mpos_end.y;

	CScrollView::OnLButtonUp(nFlags, point);
}

void Cimgproc20181369View::OnAviView()
{
	CFileDialog dlg(true, "", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"AVI파일(*.avi)|*.avi|모든파일|*.*|");

	if (dlg.DoModal() == IDOK)
	{
		AVIFileName = dlg.GetPathName();
		bAviMode = true;
		Invalidate();
	}
}

void Cimgproc20181369View::LoadAviFile(CDC* pDC)
{
	PAVIFILE pavi;
	AVIFILEINFO fi;
	int stm;
	PAVISTREAM pstm = NULL;
	AVISTREAMINFO si;
	PGETFRAME pfrm = NULL;
	int frame;
	LPBITMAPINFOHEADER pbmpih;
	unsigned char* image;
	int x, y;
	
	AVIFileInit();
	AVIFileOpen(&pavi, AVIFileName, OF_READ|OF_SHARE_DENY_NONE, NULL);
	AVIFileInfo(pavi, &fi, sizeof(AVIFILEINFO));

	for (stm = 0; stm < fi.dwStreams; stm++)
	{
		AVIFileGetStream(pavi, &pstm, 0, stm);
		AVIStreamInfo(pstm, &si, sizeof(si));
		if (si.fccType == streamtypeVIDEO)
		{
			pfrm = AVIStreamGetFrameOpen(pstm, NULL);
			//                      si.dwLength
			for (frame = 0; frame < 100; frame++)
			{
				pbmpih = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pfrm, frame);
				if (!pbmpih) continue;
				
				image = (unsigned char*)((LPSTR)pbmpih + (WORD)pbmpih->biSize);
				/*
				for(y = 0; y < fi.dwHeight; y++)
					for (x = 0; x < fi.dwWidth; x++)
					{   //일차원 포인터를 이용해서 출력하는 방법
						pDC->SetPixel(x, fi.dwHeight - 1 - y,
							RGB(image[(y * fi.dwWidth + x) * 3 + 2],
								image[(y * fi.dwWidth + x) * 3 + 1],
							    image[(y * fi.dwWidth + x) * 3 + 0]));
					}
                */
				::SetDIBitsToDevice(pDC->GetSafeHdc(), 0, 0, fi.dwWidth, fi.dwHeight,
					0, 0, 0, fi.dwWidth,
					image, (BITMAPINFO*)pbmpih, DIB_RGB_COLORS);
				Sleep(30);
			}
		}
	}
}

void Cimgproc20181369View::OnGeometryMorphing()
{
	Cimgproc20181369Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	viewMode = MORPHING;
	pDoc->GeometryMorphing();

	Invalidate();
}
