#pragma once


// CScrollTest 대화 상자

class CScrollTest : public CDialogEx
{
	DECLARE_DYNAMIC(CScrollTest)

public:
	CScrollTest(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CScrollTest();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SCROLL_TEST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void AdjustLayout();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	int	m_iScroll;
	int m_iScrollEnd;
	BOOL CanScrollDown();
};
