
// imgproc_20181369Doc.cpp: Cimgproc20181369Doc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "imgproc_20181369.h"
#endif

#include "imgproc_20181369Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Cimgproc20181369Doc

IMPLEMENT_DYNCREATE(Cimgproc20181369Doc, CDocument)

BEGIN_MESSAGE_MAP(Cimgproc20181369Doc, CDocument)
END_MESSAGE_MAP()


// Cimgproc20181369Doc 생성/소멸

Cimgproc20181369Doc::Cimgproc20181369Doc() noexcept
{
	InputImg = NULL;
	InputImg2 = NULL;
	ResultImg = NULL;    //초기화 => NULL이면 데이터읽기X, NULL이 아니면 데이터 읽기O

	gResultImg = NULL;
}

Cimgproc20181369Doc::~Cimgproc20181369Doc()
{
	int i;

	if (InputImg != NULL)
	{
		for (i = 0; i < imageHeight; i++) //free() 함수는 동적으로 할당된 메모리를 해제하는 데 사용
			free(InputImg[i]); //이거 먼저 없애주고
		free(InputImg);  // 다음에 이걸 제거
	}

	if (InputImg2 != NULL)
	{
		for (i = 0; i < imageHeight; i++)
			free(InputImg2[i]); //이거 먼저 없애주고
		free(InputImg2);  // 다음에 이걸 제거
	}

	if (ResultImg != NULL)
	{
		for (i = 0; i < imageHeight; i++)
			free(ResultImg[i]); //이거 먼저 없애주고
		free(ResultImg);  // 다음에 이걸 제거
	}

	if (gResultImg != NULL)
	{
		for (i = 0; i < gImageHeight; i++)
			free(gResultImg[i]); //이거 먼저 없애주고
		free(gResultImg);  // 다음에 이걸 제거
	}
}

BOOL Cimgproc20181369Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// Cimgproc20181369Doc serialization

void Cimgproc20181369Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		/*CFile* fp = ar.GetFile();
		if (fp->GetLength() == 256 * 256)  ar.Read(Inputimg, 256 * 256);
		else AfxMessageBox("256x256 크기의 화일만 사용가능합니다.");*/
		LoadImageFile(ar);
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void Cimgproc20181369Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void Cimgproc20181369Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void Cimgproc20181369Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Cimgproc20181369Doc 진단

#ifdef _DEBUG
void Cimgproc20181369Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void Cimgproc20181369Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Cimgproc20181369Doc 명령


void Cimgproc20181369Doc::LoadImageFile(CArchive& ar)
{
	int maxValue;
	char type[16], buf[256];
	CFile* fp = ar.GetFile();
	CString fname = fp->GetFilePath();  //fname = filename
	bool isbmp = false;
	
	if (strcmp(strchr(fname, '.'), ".ppm") == 0 || strcmp(strchr(fname, '.'), ".PPM") == 0 ||
		strcmp(strchr(fname, '.'), ".pgm") == 0 || strcmp(strchr(fname, '.'), ".PGM") == 0) // strchr = '.'이발견되는 위치를 알려주는 함수
	{
		ar.ReadString(type, 15);//실제로 읽어지는건 P5/ 앞에는 포인터 뒤에는 읽을 개수/ 16글자인데 15개만 읽는이유 : 마지막엔 Null0가 들어가 문자가 끝났음을 알리기 때문에 하나 여유 있게 만들어야한다.
		
		do {
			ar.ReadString(buf, 256);
		} while (buf[0] == '#');
		sscanf_s(buf, "%d %d", &imageWidth, &imageHeight ); //sscanf_s() 문자열에서 데이터를 받아들이는 함수/이미지 크기 읽기

		do {
			ar.ReadString(buf, 256);
		} while (buf[0] == '#');
		sscanf_s(buf, "%d", &maxValue);

		if (strcmp(type, "P5") == 0)
			depth = 1;  //흑백
		else
			depth = 3;  //컬러

	}
	else if (strcmp(strchr(fname, '.'), ".bmp") == 0 || strcmp(strchr(fname, '.'), ".BMP") == 0)
	{
		//bitmap file header 읽기
		BITMAPFILEHEADER bmfh;
		ar.Read((LPSTR)&bmfh, sizeof(bmfh));
		//bmp파일임을 나타내는 "BM"마커가 있는지 확인
		if (bmfh.bfType != (WORD)(('M' << 8) | 'B')) return;

		//bitmap info header읽기
		BITMAPINFOHEADER bih;
		ar.Read((LPSTR)&bih, sizeof(bih));
		imageWidth = bih.biWidth;
		imageHeight = bih.biHeight;
		depth = bih.biBitCount / 8;

		//palette 처리
		if (depth == 1)
		{   //palette 존재
			BYTE palette[256 * 4];
			ar.Read(palette, 256 * 4);
		}
		isbmp = true;
	}
	else if (strcmp(strchr(fname, '.'), ".raw") == 0 || strcmp(strchr(fname, '.'), ".RAW") == 0)
	{
		if (fp->GetLength() != 256 * 256)
		{
			AfxMessageBox("256 * 256 크기의 RAW 파일만 읽을 수 있습니다.");
			return;
		}

		imageWidth = 256;
		imageHeight = 256;
		depth = 1;
			
	}

	// 메모리 할당
	InputImg = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));
	ResultImg = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));

	for (int i = 0; i < imageHeight; i++)
	{
		InputImg[i] = (unsigned char*)malloc(imageWidth * depth);
		ResultImg[i] = (unsigned char*)malloc(imageWidth * depth);
	}
	
	if (!isbmp)
	{
		//영상 데이터 읽기
		for (int i = 0; i < imageHeight; i++)
			ar.Read(InputImg[i], imageWidth * depth);
	}
	else
	{
		BYTE nu[4*3];
		int widthfile;
		widthfile = (imageWidth * 8 + 31) / 32 * 4;
		for (int i = 0; i < imageHeight; i++)
		{
			if(depth ==1)
				ar.Read(InputImg[imageHeight - 1 - i], imageWidth * depth);
			else
			{
				//BGR => RGB
				BYTE r, g, b;
				for (int j = 0; j < imageWidth; j++)
				{
					ar.Read(&b, 1); ar.Read(&g, 1); ar.Read(&r, 1);
					InputImg[imageHeight - 1 - i][3 * j + 0] = r;
					InputImg[imageHeight - 1 - i][3 * j + 1] = g;
					InputImg[imageHeight - 1 - i][3 * j + 2] = b;
				}
			}
			
			if((widthfile - imageWidth) != 0)
				ar.Read(nu, (widthfile - imageWidth) * depth);
		}
			
	}

	return;
}

void Cimgproc20181369Doc::LoadSecondImageFile(CArchive& ar)
{
	int temp_w, temp_h, temp_depth;
	int maxValue;
	char type[16], buf[256];
	CFile* fp = ar.GetFile();
	CString fname = fp->GetFilePath();  //fname = filename
	bool isbmp = false;

	if (strcmp(strchr(fname, '.'), ".ppm") == 0 || strcmp(strchr(fname, '.'), ".PPM") == 0 ||
		strcmp(strchr(fname, '.'), ".pgm") == 0 || strcmp(strchr(fname, '.'), ".PGM") == 0) // strchr = '.'이발견되는 위치를 알려주는 함수
	{
		ar.ReadString(type, 15);//실제로 읽어지는건 P5/ 앞에는 포인터 뒤에는 읽을 개수/ 16글자인데 15개만 읽는이유 : 마지막엔 Null0가 들어가 문자가 끝났음을 알리기 때문에 하나 여유 있게 만들어야한다.

		do {
			ar.ReadString(buf, 256);
		} while (buf[0] == '#');
		sscanf_s(buf, "%d %d", &temp_w, &temp_h); //sscanf_s() 문자열에서 데이터를 받아들이는 함수/이미지 크기 읽기

		do {
			ar.ReadString(buf, 256);
		} while (buf[0] == '#');
		sscanf_s(buf, "%d", &maxValue);

		if (strcmp(type, "P5") == 0)
			temp_depth = 1;  //흑백
		else
			temp_depth = 3;  //컬러

	}
	else if (strcmp(strchr(fname, '.'), ".bmp") == 0 || strcmp(strchr(fname, '.'), ".BMP") == 0)
	{
		//bitmap file header 읽기
		BITMAPFILEHEADER bmfh;
		ar.Read((LPSTR)&bmfh, sizeof(bmfh));
		//bmp파일임을 나타내는 "BM"마커가 있는지 확인
		if (bmfh.bfType != (WORD)(('M' << 8) | 'B')) return;

		//bitmap info header읽기
		BITMAPINFOHEADER bih;
		ar.Read((LPSTR)&bih, sizeof(bih));
		temp_w = bih.biWidth;
		temp_h = bih.biHeight;
		temp_depth = bih.biBitCount / 8;

		//palette 처리
		if (temp_depth == 1)
		{   //palette 존재
			BYTE palette[256 * 4];
			ar.Read(palette, 256 * 4);
		}
		isbmp = true;
	}
	else if (strcmp(strchr(fname, '.'), ".raw") == 0 || strcmp(strchr(fname, '.'), ".RAW") == 0)
	{
		if (fp->GetLength() != 256 * 256)
		{
			AfxMessageBox("256 * 256 크기의 RAW 파일만 읽을 수 있습니다.");
			return;
		}

		temp_w = 256;
		temp_h = 256;
		temp_depth = 1;
	}

	if (imageWidth != temp_w || imageHeight != temp_h || depth != temp_depth)
	{
		AfxMessageBox("두 파일의 width, height, depth 값이 다르면 읽을 수 없습니다.");
		return;
	}

	// 메모리 할당
	InputImg2 = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));
	
	for (int i = 0; i < imageHeight; i++)
	{
		InputImg2[i] = (unsigned char*)malloc(imageWidth * depth);
	}

	if (!isbmp)
	{
		//영상 데이터 읽기
		for (int i = 0; i < imageHeight; i++)
			ar.Read(InputImg2[i], imageWidth * depth);
	}
	else
	{
		BYTE nu[4 * 3];
		int widthfile;
		widthfile = (imageWidth * 8 + 31) / 32 * 4;
		for (int i = 0; i < imageHeight; i++)
		{
			if (depth == 1)
				ar.Read(InputImg2[imageHeight - 1 - i], imageWidth * depth);
			else
			{
				//BGR => RGB
				BYTE r, g, b;
				for (int j = 0; j < imageWidth; j++)
				{
					ar.Read(&b, 1); ar.Read(&g, 1); ar.Read(&r, 1);
					InputImg2[imageHeight - 1 - i][3 * j + 0] = r;
					InputImg2[imageHeight - 1 - i][3 * j + 1] = g;
					InputImg2[imageHeight - 1 - i][3 * j + 2] = b;
				}
			}

			if ((widthfile - imageWidth) != 0)
				ar.Read(nu, (widthfile - imageWidth) * depth);
		}

	}

	return;

}