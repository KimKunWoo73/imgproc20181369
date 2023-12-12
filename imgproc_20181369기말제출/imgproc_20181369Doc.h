
// imgproc_20181369Doc.h: Cimgproc20181369Doc 클래스의 인터페이스
//


#pragma once


class Cimgproc20181369Doc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	Cimgproc20181369Doc() noexcept;
	DECLARE_DYNCREATE(Cimgproc20181369Doc)

// 특성입니다.
public:

// 작업입니다.
public:

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
	virtual ~Cimgproc20181369Doc();
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
public:
	//배열로 변수 저장하기
	//unsigned char Inputimg[256][256];    // [y][x]
	//unsigned char Inputimg2[256][256];    // [y][x]

	//unsigned char Resultimg[256][256];    // [y][x]
	//unsigned char Resultimg2[256][256];    // [y][x]
	  
	//포인터로 변수 저장하기
	unsigned char** InputImg;   //이중 포인터
	unsigned char** InputImg2;
	unsigned char** ResultImg;

	int imageWidth;
	int imageHeight;
	int depth;   // 1 = 흑백, 3 = 컬러

	int gImageWidth;
	int gImageHeight;
	//출력이미지
	unsigned char** gResultImg;

	unsigned char** morphedImg[10];

	void LoadImageFile(CArchive& ar);
	void LoadSecondImageFile(CArchive& ar);


	void GeometryMorphing();
	void LoadTwoImages();
};
