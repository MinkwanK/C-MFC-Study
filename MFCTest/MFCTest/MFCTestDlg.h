
// MFCTestDlg.h: 헤더 파일
//

#pragma once
#include "Animal.h"
#include "Dog.h"
#include "Person.h"
// CMFCTestDlg 대화 상자
class CMFCTestDlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCTestDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CProgressCtrl m_progress;
	CProgressCtrl m_progress2;
	CRITICAL_SECTION m_cs;
	void Thread1();
	void Thread2();
	void Thread3();
	CProgressCtrl m_progress3;
	afx_msg void OnBnClickedButton3();
	CStatic m_pic;
	afx_msg void OnBnClickedButtonBitblt();
	char* m_pData;
	int m_iHeight;
	int m_iWidth;
	BITMAPFILEHEADER m_bitFileHeader;
	BITMAPINFOHEADER m_bitInfoHeader;

	int m_iDrawMode;

	afx_msg void OnBnClickedButtonStretch();
	CStatic m_pic2;
	afx_msg void OnBnClickedButtonAnimal();
	CListBox m_List;
	afx_msg void OnBnClickedButtonDog();
	afx_msg void OnBnClickedButtonKorean();
};
