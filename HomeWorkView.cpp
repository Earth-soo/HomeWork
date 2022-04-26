
// HomeWorkView.cpp: CHomeWorkView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "HomeWork.h"
#endif

#include "HomeWorkDoc.h"
#include "HomeWorkView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHomeWorkView

IMPLEMENT_DYNCREATE(CHomeWorkView, CView)

BEGIN_MESSAGE_MAP(CHomeWorkView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(IDM_RECTANGULAR, &CHomeWorkView::OnRectangular)
	ON_COMMAND(IDM_CIRCLE, &CHomeWorkView::OnCircle)
	ON_COMMAND(IDM_BITMAP, &CHomeWorkView::OnBitmap)
	ON_UPDATE_COMMAND_UI(IDM_RECTANGULAR, &CHomeWorkView::OnUpdateRectangular)
	ON_UPDATE_COMMAND_UI(IDM_CIRCLE, &CHomeWorkView::OnUpdateCircle)
	ON_UPDATE_COMMAND_UI(IDM_BITMAP, &CHomeWorkView::OnUpdateBitmap)
END_MESSAGE_MAP()

// CHomeWorkView 생성/소멸

CHomeWorkView::CHomeWorkView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_buttonDown = false;
}

CHomeWorkView::~CHomeWorkView()
{
}

BOOL CHomeWorkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CHomeWorkView 그리기

void CHomeWorkView::OnDraw(CDC* pDC)
{
	CHomeWorkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	for (const CDrawLine& line : pDoc->m_lines) {
		line.DrawAll(pDC);
	}
}


// CHomeWorkView 인쇄

BOOL CHomeWorkView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CHomeWorkView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CHomeWorkView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CHomeWorkView 진단

#ifdef _DEBUG
void CHomeWorkView::AssertValid() const
{
	CView::AssertValid();
}

void CHomeWorkView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHomeWorkDoc* CHomeWorkView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHomeWorkDoc)));
	return (CHomeWorkDoc*)m_pDocument;
}
#endif //_DEBUG


// CHomeWorkView 메시지 처리기


void CHomeWorkView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CHomeWorkDoc* pDoc = GetDocument();
	SetCapture();
	m_buttonDown = true;

	switch (pDoc->mode) {
	case 1:
		pDoc->m_line.m_array.clear();
		pDoc->m_line.m_array.push_back(point);
		break;
	case 2 : case 3: case 4:
		Start = point;
		break;
	}
	CView::OnLButtonDown(nFlags, point);
}


void CHomeWorkView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CHomeWorkDoc* pDoc = GetDocument();
	ReleaseCapture();

	switch (pDoc->mode) {
	case 1:
		pDoc->m_lines.push_back(pDoc->m_line);
		pDoc->SetModifiedFlag();
		break;
	case 2:
	{
		CDC* pDC1 = GetDC();
		pDC1->Rectangle(Start.x, Start.y, Finish.x, Finish.y);
		ReleaseDC(pDC1);
		break;
	}
	case 3:
	{
		CDC* pDC2 = GetDC();
		pDC2->Ellipse(Start.x, Start.y, Finish.x, Finish.y);
		ReleaseDC(pDC2);
		break;
	}
	case 4:
	{
		CDC* pDC3 = GetDC();

		CDC MemDC;
		BITMAP bmpinfo;
		MemDC.CreateCompatibleDC(pDC3);

		CBitmap bmp;
		CBitmap* pOld = NULL;
		bmp.LoadBitmap(IDB_BITMAP1);
		bmp.GetBitmap(&bmpinfo);

		pOld = MemDC.SelectObject(&bmp);

		pDC3->StretchBlt(Start.x, Start.y, (Finish.x - Start.x), (Finish.y - Start.y),
			&MemDC, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, SRCCOPY);

		MemDC.SelectObject(pOld);
		
		MemDC.DeleteDC();
		ReleaseDC(pDC3);
		break;
	}
	}

	m_buttonDown = false;
	CView::OnLButtonUp(nFlags, point);
}

#include "MainFrm.h"

void CHomeWorkView::OnMouseMove(UINT nFlags, CPoint point)
{
	CHomeWorkDoc* pDoc = GetDocument();
	if (m_buttonDown) {
		switch (pDoc->mode) {
		case 1:
			if ((nFlags & MK_LBUTTON) == MK_LBUTTON) {
				CClientDC dc(this);

				pDoc->m_line.DrawAll(&dc);
				pDoc->m_line.push_back(point);
				break;
			}
		case 2: case 3: case 4:
			if ((nFlags & MK_LBUTTON) == MK_LBUTTON) {
				Finish = point;
				break;
			}
		}

		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		pMainFrame->SetMousePositionOutput(point);

		CView::OnMouseMove(nFlags, point);
	}
}



void CHomeWorkView::OnRectangular()
{
	CHomeWorkDoc* pDoc = GetDocument();
	pDoc->mode = 2;
}

void CHomeWorkView::OnCircle()
{
	CHomeWorkDoc* pDoc = GetDocument();
	pDoc->mode = 3;
}

void CHomeWorkView::OnBitmap()
{
	CHomeWorkDoc* pDoc = GetDocument();
	pDoc->mode = 4;
}

void CHomeWorkView::OnUpdateRectangular(CCmdUI* pCmdUI)
{
	CHomeWorkDoc* pDoc = GetDocument();
	pCmdUI->SetCheck(pDoc->mode == 2);
}

void CHomeWorkView::OnUpdateCircle(CCmdUI* pCmdUI)
{
	CHomeWorkDoc* pDoc = GetDocument();
	pCmdUI->SetCheck(pDoc->mode == 3);
}


void CHomeWorkView::OnUpdateBitmap(CCmdUI* pCmdUI)
{
	CHomeWorkDoc* pDoc = GetDocument();
	pCmdUI->SetCheck(pDoc->mode == 4);
}
