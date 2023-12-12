
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


void Cimgproc20181369Doc::LoadTwoImages()
{
	CFile file;
	CFileDialog dlg(TRUE);
	AfxMessageBox("두번째 이미지를 선택하세요");

	if (dlg.DoModal() == IDOK) {   // 파일 선택 대화 상자 실행 
		file.Open(dlg.GetPathName(), CFile::modeRead);   // 파일 열기 
		CArchive ar(&file, CArchive::load);
		LoadSecondImageFile(ar);     // 영상 읽기 
		file.Close();               // 파일 닫기 
	}
}

typedef struct
{
	int Px;
	int Py;
	int Qx;
	int Qy;
} control_line;

#define NUM_FRAMES 10

void Cimgproc20181369Doc::GeometryMorphing()
{
	control_line source_lines[23] =
	{ {116,7,207,5},{34,109,90,21},{55,249,30,128},{118,320,65,261},
	 {123,321,171,321},{179,319,240,264},{247,251,282,135},{281,114,228,8},
	 {78,106,123,109},{187,115,235,114},{72,142,99,128},{74,150,122,154},
	 {108,127,123,146},{182,152,213,132},{183,159,229,157},{219,131,240,154},
	 {80,246,117,212},{127,222,146,223},{154,227,174,221},{228,252,183,213},
	 {114,255,186,257},{109,258,143,277},{152,278,190,262} };
	control_line dest_lines[23] =
	{ {120,8,200,6},{12,93,96,16},{74,271,16,110},{126,336,96,290},
	 {142,337,181,335},{192,335,232,280},{244,259,288,108},{285,92,212,13},
	 {96,135,136,118},{194,119,223,125},{105,145,124,134},{110,146,138,151},
	 {131,133,139,146},{188,146,198,134},{189,153,218,146},{204,133,221,140},
	 {91,268,122,202},{149,206,159,209},{170,209,181,204},{235,265,208,199},
	 {121,280,205,284},{112,286,160,301},{166,301,214,287} };

	double u;       // 수직 교차점의 위치   
	double h;       // 제어선으로부터 픽셀의 수직 변위 
	double d;       // 제어선과 픽셀 사이의 거리 
	double tx, ty;   // 결과영상 픽셀에 대응되는 입력 영상 픽셀 사이의 변위의 합  
	double xp, yp;  // 각 제어선에 대해 계산된 입력 영상의 대응되는 픽셀 위치     
	double weight;     // 각 제어선의 가중치       
	double totalWeight; // 가중치의 합          
	double a = 0.001, b = 2.0, p = 0.75;
	unsigned char** warpedImg;
	unsigned char** warpedImg2;
	int frame;
	double fweight;
	control_line warp_lines[23];
	double tx2, ty2, xp2, yp2;
	int dest_x1, dest_y1, dest_x2, dest_y2, source_x2, source_y2;
	int x1, x2, y1, y2, src_x1, src_y1, src_x2, src_y2;
	double src_line_length, dest_line_length;
	int i, j;
	int num_lines = 23;         // 제어선의 수 
	int line, x, y, source_x, source_y, last_row, last_col;

	LoadTwoImages();

	// 중간 프레임의 워핑 결과를 저장을 위한 기억장소 할당 
	warpedImg = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));
	for (i = 0; i < imageHeight; i++) {
		warpedImg[i] = (unsigned char*)malloc(imageWidth * depth);
	}

	warpedImg2 = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));
	for (i = 0; i < imageHeight; i++) {
		warpedImg2[i] = (unsigned char*)malloc(imageWidth * depth);
	}

	for (i = 0; i < NUM_FRAMES; i++) {
		morphedImg[i] = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));
		for (j = 0; j < imageHeight; j++) {
			morphedImg[i][j] = (unsigned char*)malloc(imageWidth * depth);
		}
	}
	last_row = imageHeight - 1;
	last_col = imageWidth - 1;

	// 각 중간 프레임에 대하여 
	for (frame = 1; frame <= NUM_FRAMES; frame++)
	{
		// 중간 프레임에 대한 가중치 계산 
		fweight = (double)(frame) / NUM_FRAMES;

		// 중간 프레임에 대한 제어선 계산 
		for (line = 0; line < num_lines; line++)
		{
			warp_lines[line].Px = (int)(source_lines[line].Px +
				(dest_lines[line].Px - source_lines[line].Px) * fweight);
			warp_lines[line].Py = (int)(source_lines[line].Py +
				(dest_lines[line].Py - source_lines[line].Py) * fweight);
			warp_lines[line].Qx = (int)(source_lines[line].Qx +
				(dest_lines[line].Qx - source_lines[line].Qx) * fweight);
			warp_lines[line].Qy = (int)(source_lines[line].Qy +
				(dest_lines[line].Qy - source_lines[line].Qy) * fweight);
		}

		// 출력 영상의 각 픽셀에 대하여 
		for (y = 0; y < imageHeight; y++)
		{
			for (x = 0; x < imageWidth; x++)
			{
				totalWeight = 0.0;
				tx = 0.0;
				ty = 0.0;
				tx2 = 0.0;
				ty2 = 0.0;
				// 각 제어선에 대하여 
				for (line = 0; line < num_lines; line++)
				{
					x1 = warp_lines[line].Px;
					y1 = warp_lines[line].Py;
					x2 = warp_lines[line].Qx;
					y2 = warp_lines[line].Qy;
					dest_line_length = sqrt((double)(x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
					// 수직교차점의 위치 및 픽셀의 수직 변위 계산 
					u = (double)((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1)) /
						(double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
					h = (double)((y - y1) * (x2 - x1) - (x - x1) * (y2 - y1)) / dest_line_length;

					// 제어선과 픽셀 사이의 거리 계산 
					if (u < 0) d = sqrt((double)(x - x1) * (x - x1) + (y - y1) * (y - y1));
					else if (u > 1) d = sqrt((double)(x - x2) * (x - x2) + (y - y2) * (y - y2));
					else d = fabs(h);

					src_x1 = source_lines[line].Px;
					src_y1 = source_lines[line].Py;
					src_x2 = source_lines[line].Qx;
					src_y2 = source_lines[line].Qy;
					src_line_length = sqrt((double)(src_x2 - src_x1) * (src_x2 - src_x1) +
						(src_y2 - src_y1) * (src_y2 - src_y1));
					dest_x1 = dest_lines[line].Px;
					dest_y1 = dest_lines[line].Py;
					dest_x2 = dest_lines[line].Qx;
					dest_y2 = dest_lines[line].Qy;
					dest_line_length = sqrt((double)(dest_x2 - dest_x1) * (dest_x2 - dest_x1) +
						(dest_y2 - dest_y1) * (dest_y2 - dest_y1));

					// 입력 영상 1에서의 대응 픽셀 위치 계산 
					xp = src_x1 + u * (src_x2 - src_x1) -
						h * (src_y2 - src_y1) / src_line_length;
					yp = src_y1 + u * (src_y2 - src_y1) +
						h * (src_x2 - src_x1) / src_line_length;

					// 입력 영상 2에서의 대응 픽셀 위치 계산 
					xp2 = dest_x1 + u * (dest_x2 - dest_x1) -
						h * (dest_y2 - dest_y1) / dest_line_length;
					yp2 = dest_y1 + u * (dest_y2 - dest_y1) +
						h * (dest_x2 - dest_x1) / dest_line_length;

					// 제어선에 대한 가중치 계산 
					weight = pow((pow((double)(dest_line_length), p) / (a + d)), b);

					// 입력 영상 1의 대응 픽셀과의 변위 계산 
					tx += (xp - x) * weight;
					ty += (yp - y) * weight;

					// 입력 영상 2의 대응 픽셀과의 변위 계산 
					tx2 += (xp2 - x) * weight;
					ty2 += (yp2 - y) * weight;

					totalWeight += weight;
				}
				// 입력 영상 1의 대응 픽셀 위치 계산     
				source_x = x + (int)(tx / totalWeight + 0.5);
				source_y = y + (int)(ty / totalWeight + 0.5);

				// 입력 영상 2의 대응 픽셀 위치 계산 
				source_x2 = x + (int)(tx2 / totalWeight + 0.5);
				source_y2 = y + (int)(ty2 / totalWeight + 0.5);

				// 영상의 경계를 벗어나는지 검사 
				if (source_x < 0) source_x = 0;
				if (source_x > last_col) source_x = last_col;
				if (source_y < 0) source_y = 0;
				if (source_y > last_row) source_y = last_row;

				if (source_x2 < 0) source_x2 = 0;
				if (source_x2 > last_col) source_x2 = last_col;
				if (source_y2 < 0) source_y2 = 0;
				if (source_y2 > last_row) source_y2 = last_row;

				// 워핑 결과 저장 
				warpedImg[y][x] = InputImg[source_y][source_x];
				warpedImg2[y][x] = InputImg2[source_y2][source_x2];
			}
		}

		// 모핑 결과 합병 
		for (y = 0; y < imageHeight; y++)
			for (x = 0; x < imageWidth; x++) {
				int val = (int)((1.0 - fweight) * warpedImg[y][x] +
					fweight * warpedImg2[y][x]);
				if (val < 0) val = 0;
				if (val > 255) val = 255;
				morphedImg[frame - 1][y][x] = val;
			}
	}
}




