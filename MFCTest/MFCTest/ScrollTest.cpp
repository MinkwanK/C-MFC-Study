// ScrollTest.cpp: 구현 파일
//

#include "pch.h"
#include "MFCTest.h"
#include "ScrollTest.h"
#include "afxdialogex.h"


// CScrollTest 대화 상자

IMPLEMENT_DYNAMIC(CScrollTest, CDialogEx)

CScrollTest::CScrollTest(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SCROLL_TEST, pParent)
{
	m_iScroll = 0;
}

CScrollTest::~CScrollTest()
{
}


BEGIN_MESSAGE_MAP(CScrollTest, CDialogEx)
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CScrollTest 메시지 처리기




void CScrollTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BOOL CScrollTest::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rc, rcClient;
	GetClientRect(rcClient);
	GetDlgItem(IDC_STATIC_GUIDE)->GetWindowRect(rc);
	ScreenToClient(rc);

	m_iScrollEnd = rcClient.bottom - rc.bottom;
	AdjustLayout();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CScrollTest::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	//CRect rc, rcClient;
	//if (GetDlgItem(IDC_STATIC_GUIDE))
	//{
	//	GetClientRect(rcClient);
	//	GetDlgItem(IDC_STATIC_GUIDE)->GetWindowRect(rc);
	//	ScreenToClient(rc);
	//	m_iScrollEnd = rcClient.bottom - rc.bottom;
	//}
	AdjustLayout();
}

void CScrollTest::AdjustLayout()
{
	CRect rc, rcClient;
	GetClientRect(rcClient);	

	if (GetDlgItem(IDC_STATIC_GUIDE))
	{
		GetDlgItem(IDC_STATIC_GUIDE)->GetWindowRect(rc);
		ScreenToClient(rc);
		int iGap = 5;

		rc.left = rcClient.left + iGap;
		rc.right = rcClient.right - iGap;
		GetDlgItem(IDC_STATIC_GUIDE)->MoveWindow(rc);
	}
}




BOOL CScrollTest::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int iDir = 0;
	BOOL bForce = FALSE;
	const int iMaxScroll = 0;
	if (zDelta > 0)
	{
		if (m_iScroll == -1)
			bForce = TRUE;

		m_iScroll += 1;
		iDir = 1;

		if (m_iScroll > iMaxScroll)
			m_iScroll = iMaxScroll;

		CString str;
		str.Format(_T("START: 마우스 휠 UP %d"), m_iScroll);
		GetDlgItem(IDC_STATIC_SCROLL_TEXT)->SetWindowText(str);
		GetDlgItem(IDC_STATIC_END_TEXT)->SetWindowText(str);
	}
	else
	{
		if (CanScrollDown())
		{
			m_iScroll -= 1;
			iDir = -1;
		}

		CString str;
		str.Format(_T("START: 마우스 휠 DOWN %d"), m_iScroll);
		GetDlgItem(IDC_STATIC_SCROLL_TEXT)->SetWindowText(str);
		GetDlgItem(IDC_STATIC_END_TEXT)->SetWindowText(str);
	}

	if ((m_iScroll != iMaxScroll) || bForce)
	{
		ScrollWindow(0, iDir * 15);

		if (bForce)
			bForce = FALSE;
	}

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}




BOOL CScrollTest::CanScrollDown()
{
	CRect rc, rcClient;
	GetClientRect(rcClient);
	GetDlgItem(IDC_STATIC_GUIDE)->GetWindowRect(rc);
	ScreenToClient(rc);

	int iScrollEnd = rcClient.bottom - rc.bottom;
	if (iScrollEnd > m_iScrollEnd)
		return false;
	else
		return true;
}
