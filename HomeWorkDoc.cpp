
// HomeWorkDoc.cpp: CHomeWorkDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "HomeWork.h"
#endif

#include "HomeWorkDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CHomeWorkDoc

IMPLEMENT_DYNCREATE(CHomeWorkDoc, CDocument)

BEGIN_MESSAGE_MAP(CHomeWorkDoc, CDocument)
	ON_COMMAND(IDM_COLOR_GREEN, &CHomeWorkDoc::OnColorGreen)
	ON_UPDATE_COMMAND_UI(IDM_COLOR_GREEN, &CHomeWorkDoc::OnUpdateColorGreen)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_COLOR, &CHomeWorkDoc::OnUpdateIndicatorColor)
	ON_COMMAND(IDM_COLOR_RED, &CHomeWorkDoc::OnColorRed)
	ON_UPDATE_COMMAND_UI(IDM_COLOR_RED, &CHomeWorkDoc::OnUpdateColorRed)
END_MESSAGE_MAP()


// CHomeWorkDoc 생성/소멸

CHomeWorkDoc::CHomeWorkDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	TRACE("size_type = %d\n", sizeof(m_lines.size()));

}

CHomeWorkDoc::~CHomeWorkDoc()
{
}

BOOL CHomeWorkDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.
	m_line.m_nWidth = 10;
	m_line.m_penColor = RGB(255, 0, 0);

	//vector <int> arr;
	//vector <int> arr2;

	//arr.reserve(100);	//처음 데이터가 들어갈 때 100개만큼 용량이 확보됨(메모리를 추가적으로 늘릴 필요 없음)
	//arr.resize(100);

	//arr.push_back(100);	//capacity 1
	//arr.push_back(200);	//2배, 2, 2
	//arr.push_back(300);	//2배, 3, 4
	//arr.push_back(400);	//2배, 4, 4
	//arr.push_back(500);	//2배, 5, 8

	//TRACE("1 size = %d\n", arr.size());
	//TRACE("1 size = %d\n", arr.size());	//100

	//arr.resize(50);
	//TRACE("3 size = %d\n", arr.size());	//50
	//TRACE("4 size = %d\n", arr.size());	
	return TRUE;
}




// CHomeWorkDoc serialization

void CHomeWorkDoc::Serialize(CArchive& ar)
{
	int lines_size = m_lines.size();
	int array_size;

	//vector<POINT> arr(10);	//연속된 메모리(10개)를 사용하여 관리(자동으로 증가(2배씩), 주소는 달라짐)
	//list<POINT> list;	//list 구조는 연속된 메모리가 아님(5개를 읽는다고 하면 1개만 유효한 값, 4개는 쓰레기값)

	if (ar.IsStoring())
	{
		ar << lines_size;
		for (const CDrawLine& line : m_lines) {
			array_size = line.m_array.size();
			
			ar << line.m_nWidth;	//73~78까지 선 하나 저장하는데 필요한 것
			ar << line.m_penColor;
			ar << array_size;	//ram에서 hd로 저장
			//버퍼 크기만큼 한번에 불러오기(1번 write)
			ar.Write(&line.m_array[0], array_size * sizeof(POINT));	//저장 시작주소(배열의 시작주소), 저장할 양(배열 * 포인트(x,y)(포인트는 long 타입))
			//hd로 데이터를 보내는데 차이가 남
			
			//for (const CPoint pt : line.m_array) {	//io가 반복적으로 사용돼서 시간이 느려질 수 있음(블럭단위(버퍼)로 가져오기)(100번 write)
			//	ar << pt.x;
			//	ar << pt.y;
			//}
		}
	}
	else
	{
		CPoint pt;

		ar >> lines_size;
		for (int i = 0; i < lines_size;i++) {
			ar >> m_line.m_nWidth;
			ar >> m_line.m_penColor;
			ar >> array_size;
			m_line.clear();

			m_line.m_array.reserve(array_size);	//배열 공간 확보하기
			m_line.m_array.resize(array_size);	//배열크기 확보하기(resize)
			
			ar.Read(&m_line.m_array[0], sizeof(POINT) * array_size);

;			/*for (int j = 0; j < array_size;j++) {
				ar >> pt.x;
				ar >> pt.y;
				m_line.m_array.push_back(pt);
			}*/
			m_lines.push_back(m_line);
		}
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CHomeWorkDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CHomeWorkDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CHomeWorkDoc::SetSearchContent(const CString& value)
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

// CHomeWorkDoc 진단

#ifdef _DEBUG
void CHomeWorkDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHomeWorkDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CHomeWorkDoc 명령


void CHomeWorkDoc::DeleteContents()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_line.clear();
	m_lines.clear();

	CDocument::DeleteContents();
}


void CHomeWorkDoc::OnColorGreen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	mode = 1;
	m_line.m_penColor = RGB(0, 255, 0);
}


void CHomeWorkDoc::OnUpdateColorGreen(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_line.m_penColor == RGB(0, 255, 0));
}

void CHomeWorkDoc::OnUpdateIndicatorColor(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	switch (m_line.m_penColor) {
	case RGB(255, 0, 0):
		pCmdUI->SetText(_T("빨간색"));
		break;
	case RGB(0,255, 0):
		pCmdUI->SetText(_T("초록색"));
		break;
	default:
		pCmdUI->SetText(_T(""));
		break;
	}
}


void CHomeWorkDoc::OnColorRed()
{
	mode = 1;
	m_line.m_penColor = RGB(255, 0, 0);
}


void CHomeWorkDoc::OnUpdateColorRed(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_line.m_penColor == RGB(255, 0, 0));
}
