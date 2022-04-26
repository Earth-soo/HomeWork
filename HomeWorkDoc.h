
// HomeWorkDoc.h: CHomeWorkDoc 클래스의 인터페이스
//


#pragma once

#include <vector>
using namespace std;

//폭포수 개발 방법론(si) : 설계(구조)를 먼저한다 
//에자일 방법론(tdd -> bdd : 리펙토링)


class CDrawLine {	//변수가 많아서 그냥 클래스로 지정
public:
	COLORREF m_penColor;
	int m_nWidth;	//선의 굵기(x의 굵기만 사용한 것)
	//LOGPEN m_pen;	//style, width, color도 가진 것(위에 있는 기능들을 다 담고 있는)
	vector<CPoint> m_array;
public:
	void clear() {
		m_array.clear();
	}

	void DrawAll(CDC* pDC) const {
		pDC->MoveTo(m_array[0]);
		CPen pen(PS_SOLID, m_nWidth, m_penColor);
		CPen* pOldPen = pDC->SelectObject(&pen);

		for (const CPoint& pt : m_array) {
			pDC->LineTo(pt);
		}

		pDC->SelectObject(pOldPen);
	}

	//void DrawLastLine(CDC* pDC, CPoint& point) {
	//	CPen pen(PS_SOLID, m_nWidth, m_penColor);
	//	CPen* pOldPen = pDC->SelectObject(&pen);

	//	pDC->MoveTo(m_array[m_array.size() - 1]);	//\n이 있기 때문에 -1 해주기
	//	pDC->LineTo(point);

	//	pDC->SelectObject(pOldPen);
	//}

	void push_back(const CPoint& point) {
		m_array.push_back(point);
	}

};

class CHomeWorkDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CHomeWorkDoc() noexcept;
	DECLARE_DYNCREATE(CHomeWorkDoc)

// 특성입니다.
public:
	//COLORREF m_penColor;
	//vector<CPoint> m_array;
	CDrawLine m_line;	//선에 관한 것
	vector<CDrawLine> m_lines;	//선을 여러개 사용하기 위해
	int mode;
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
	virtual ~CHomeWorkDoc();
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
	virtual void DeleteContents();
	afx_msg void OnColorGreen();
	afx_msg void OnUpdateColorGreen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIndicatorColor(CCmdUI* pCmdUI);
	afx_msg void OnColorRed();
	afx_msg void OnUpdateColorRed(CCmdUI* pCmdUI);
};
