
// MFCTestDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCTest.h"
#include "MFCTestDlg.h"
#include "afxdialogex.h"
#include <thread>
#include <chrono>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CPerson m_Person;
extern int m_iAge = 0;

const CString SETUP_INI_PATH = _T("Setup.ini");
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCTestDlg 대화 상자



CMFCTestDlg::CMFCTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	m_pData = nullptr;
}

void CMFCTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_PROGRESS2, m_progress2);
	DDX_Control(pDX, IDC_PROGRESS3, m_progress3);
	DDX_Control(pDX, IDC_STATIC_Pic, m_pic);
	DDX_Control(pDX, IDC_STATIC_Pic2, m_pic2);
	DDX_Control(pDX, IDC_LIST1, m_List);
}

BEGIN_MESSAGE_MAP(CMFCTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCTestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_BitBlt, &CMFCTestDlg::OnBnClickedButtonBitblt)
	ON_BN_CLICKED(IDC_BUTTON_ANIMAL, &CMFCTestDlg::OnBnClickedButtonAnimal)
	ON_BN_CLICKED(IDC_BUTTON_DOG, &CMFCTestDlg::OnBnClickedButtonDog)
	ON_BN_CLICKED(IDC_BUTTON_KOREAN, &CMFCTestDlg::OnBnClickedButtonKorean)
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON_NOW, &CMFCTestDlg::OnBnClickedButtonNow)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_ENFORCE_FILE, &CMFCTestDlg::OnBnClickedButtonCreateEnforceFile)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_SET_SETUP_DIR, &CMFCTestDlg::OnBnClickedButtonSetSetupDir)
	ON_BN_CLICKED(IDC_BUTTON_SET_ITSENSE_DIR, &CMFCTestDlg::OnBnClickedButtonSetItsenseDir)
	ON_BN_CLICKED(IDC_BUTTON_MAKE_ITSENS_FOLDER, &CMFCTestDlg::OnBnClickedButtonMakeItsensFolder)
	ON_BN_CLICKED(IDC_BUTTON_ITAGENT, &CMFCTestDlg::OnBnClickedButtonItagent)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_ENFORCE_FILE_BOOST, &CMFCTestDlg::OnBnClickedButtonCreateEnforceFileBoost)
END_MESSAGE_MAP()


// CMFCTestDlg 메시지 처리기

BOOL CMFCTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_progress.SetRange(0, 100);
	m_progress2.SetRange(0, 100);
	m_progress3.SetRange(0, 100);
	InitializeCriticalSection(&m_cs);
	
	GetDlgItem(IDC_EDIT_STRING)->SetWindowText(_T("안녕하세요"));


	m_hStopEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	GetDlgItem(IDC_EDIT_FILE_CNT)->SetWindowText(_T("1"));
	GetDlgItem(IDC_EDIT_MIN)->SetWindowText(_T("1"));
	GetDlgItem(IDC_EDIT_MAX)->SetWindowText(_T("10"));
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRect rcClient;
		GetClientRect(&rcClient);

		CRect rcPic;
		m_pic.GetWindowRect(&rcPic);
		ScreenToClient(&rcPic);

		CPaintDC dc(this);
		CDC* pMemDC;
		CBitmap* pOldBitmap;
		pMemDC = new CDC;
		pMemDC->CreateCompatibleDC(&dc);
		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
		pOldBitmap = pMemDC->SelectObject(&bitmap);
		bitmap.DeleteObject();
		pMemDC->SetBkMode(TRANSPARENT);
		pMemDC->SetStretchBltMode(COLORONCOLOR);
		pMemDC->FillSolidRect(&rcClient, RGB(255, 255, 255));

		if (m_pData)
		{
			BITMAPINFO bitInfo;
			bitInfo.bmiHeader = m_bitInfoHeader;

			SetStretchBltMode(pMemDC->GetSafeHdc(), COLORONCOLOR);  // set iMode.
			StretchDIBits(pMemDC->GetSafeHdc(), rcPic.left, rcPic.top, rcPic.Width(), rcPic.Height(),
				0, 0, bitInfo.bmiHeader.biWidth, bitInfo.bmiHeader.biHeight, m_pData, &bitInfo, DIB_RGB_COLORS, SRCCOPY);

			//원본 이미지에서 이만큼을 자르고 싶다는 의미. 
			CRect rcCrop(250, 250, 500, 500);

			float fWidthRatio = (float)rcPic.Width() / (float)m_iWidth;
			float fHeightRatio = (float)rcPic.Height() / (float)m_iHeight;

			CRect rcCrop2;
			rcCrop2.left = rcPic.left + rcCrop.left * fWidthRatio;
			rcCrop2.right = rcPic.left + rcCrop.right * fWidthRatio;
			rcCrop2.top = rcPic.top + rcCrop.top * fHeightRatio;
			rcCrop2.bottom = rcPic.top + rcCrop.bottom * fHeightRatio;
			pMemDC->Rectangle(rcCrop2);

			int iWidth = rcCrop.Width();
			int iHeight = rcCrop.Height();

			int iPadding = ((iWidth * 3) % 4) == 0 ? 0 : 4 - ((iWidth * 3) % 4);
			char* pData2 = nullptr;
			pData2 = new char[(iWidth + iPadding) * iHeight * 3];

			bitInfo.bmiHeader.biWidth = iWidth;
			bitInfo.bmiHeader.biHeight = iHeight;
			int iPos = 0;

			if (iPadding > 0)
			{
				for (int i = rcCrop.top; i < rcCrop.bottom; i++)
				{
					memcpy(&pData2[iPos], &m_pData[(i * m_iWidth * 3) + rcCrop.left * 3], iWidth * 3);
					iPos += iWidth * 3;
					memset(&pData2[iPos], 0x00, iPadding);
					iPos += iPadding;
				}
			}
			else
			{
				for (int i = rcCrop.bottom; i < rcCrop.bottom; i++)
				{
					memcpy(&pData2[iPos], &m_pData[(i * m_iWidth * 3) + rcCrop.left * 3], iWidth * 3);
					iPos += iWidth * 3;
				}
			}


			CRect rcPic2;
			m_pic2.GetWindowRect(&rcPic2);
			ScreenToClient(&rcPic2);
			SetStretchBltMode(pMemDC->GetSafeHdc(), COLORONCOLOR);  // set iMode.
			StretchDIBits(pMemDC->GetSafeHdc(), rcPic2.left, rcPic2.top, rcPic2.Width(), rcPic2.Height(),
				0, 0, bitInfo.bmiHeader.biWidth, bitInfo.bmiHeader.biHeight, pData2, &bitInfo, DIB_RGB_COLORS, SRCCOPY);
			
			CString sImageInfo;
			sImageInfo.Format(_T("이미지 넓이: %d 높이: %d, 컨트롤 넓이: %d, 높이: %d"), m_iWidth, m_iHeight, rcPic.Width(), rcPic.Height());
			pMemDC->DrawText(sImageInfo, rcClient, DT_BOTTOM| DT_CENTER | DT_SINGLELINE);
		}

		if (m_iDrawMode == 0)
		{
			dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), pMemDC, rcClient.left, rcClient.top, SRCCOPY);
		}

		delete pMemDC;
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCTestDlg::OnBnClickedButton1()
{
	std::thread t1(&CMFCTestDlg::Thread1, this);
	t1.detach();

}


void CMFCTestDlg::OnBnClickedButton2()
{
	std::thread t2(&CMFCTestDlg::Thread2, this);
	t2.detach();
}

void CMFCTestDlg::OnBnClickedButton3()
{
	std::thread t3(&CMFCTestDlg::Thread3, this);
	t3.detach();

}


void CMFCTestDlg::Thread1()
{
	TRACE(_T("스레드1 시작\n"));
	EnterCriticalSection(&m_cs);
	for (int i = 0; i <= 100; i++)
	{
		m_progress.SetPos(i);
		TRACE(_T("%d\n"),i);
		Sleep(1);
	}
	LeaveCriticalSection(&m_cs);
	TRACE(_T("스레드1 종료\n"));
}

void CMFCTestDlg::Thread2()
{
	TRACE(_T("스레드2 시작\n"));
	for (int i = 0; i <= 100; i++)
	{
		m_progress2.SetPos(i);
		TRACE(_T("%d\n"), i);
		Sleep(1);
	}
	TRACE(_T("스레드2 종료\n"));
}

void CMFCTestDlg::Thread3()
{

	TRACE(_T("스레드3 시작\n"));
	EnterCriticalSection(&m_cs);
	for (int i = 0; i <= 100; i++)
	{
		m_progress.SetPos(i);
		m_progress2.SetPos(i);
		m_progress3.SetPos(i);
		Sleep(1);
		TRACE(_T("%d\n"), i);
	}
	LeaveCriticalSection(&m_cs);
	TRACE(_T("스레드3 종료\n"));
}




void CMFCTestDlg::OnBnClickedButtonBitblt()
{
	CFile file;
	if (file.Open(_T("test.bmp"), CFile::modeRead))
	{
		if (m_pData != nullptr)
		{
			delete[] m_pData;
			m_pData = nullptr;
		}
		
		file.Read(&m_bitFileHeader, sizeof(BITMAPFILEHEADER));
		file.Read(&m_bitInfoHeader, sizeof(BITMAPINFOHEADER));

		m_iWidth = m_bitInfoHeader.biWidth;
		m_iHeight = m_bitInfoHeader.biHeight;

		int iBmpSize = m_bitFileHeader.bfSize - m_bitFileHeader.bfOffBits;
		m_pData = new char[iBmpSize];
		file.Seek(m_bitFileHeader.bfOffBits, CFile::begin);
		file.Read(m_pData, iBmpSize);
		file.Close();
		Invalidate();
	}

	m_iDrawMode = 0;
}



void CMFCTestDlg::OnBnClickedButtonAnimal()
{
	CAnimal animal;
	CString str;
	str.Format(_T("만들어진 Animal 개수: %d"),animal.GetAnimalCnt());
	m_List.AddString(animal.m_sSay);
	m_List.AddString(str);
}


void CMFCTestDlg::OnBnClickedButtonDog()
{
	CDog dog;
	int iSel = m_List.AddString(dog.m_sSay);
	m_List.SetCurSel(iSel);
}




void CMFCTestDlg::OnBnClickedButtonKorean()
{
	CString sEdit;
	GetDlgItem(IDC_EDIT_STRING)->GetWindowText(sEdit);

	if (sEdit.GetLength() > 2)
	{
		char cFirst = sEdit[0];
		
		if (cFirst & 0x80)
		{
			int iSel = m_List.AddString(_T("첫 글자가 한글입니다."));
			m_List.SetCurSel(iSel);
		}
		char cSecond = sEdit[1];

		TRACE(_T(""));
	}
}


BOOL CMFCTestDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CMFCTestDlg::OnBnClickedButtonNow()
{
	std::chrono::system_clock::time_point now =  std::chrono::system_clock::now();
	std::time_t timeT = std::chrono::system_clock::to_time_t(now);

	FILETIME fileTime;
	SYSTEMTIME stTime;

	//__int64 time = static_cast<__int64>(timeT) * 10000000 + 116444736000000000LL; // 1970-01-01 to 1601-01-01
	//fileTime.dwLowDateTime = (DWORD)(time & 0xFFFFFFFF);
	//fileTime.dwHighDateTime = (DWORD)(time >> 32);

	//FileTimeToSystemTime(&fileTime, &stTime);
	SetLocalTime(&stTime);
	CString sTime;
	sTime.Format(_T("형변환 없이 GetLocalTime으로 편하게 시간을 구할 수 있다. %04d.%02d.%02d %02d:%02d:%02d>"), stTime.wYear, stTime.wMonth, stTime.wDay,stTime.wHour,stTime.wMinute,stTime.wSecond);
	int iSel = m_List.AddString(sTime);
	m_List.SetCurSel(iSel);

}


void CMFCTestDlg::OnBnClickedButtonCreateEnforceFile()
{
	if (!m_bMakingFile)
	{
		std::thread createFile(&CMFCTestDlg::CreateEnforceFile, this);
		createFile.detach();
	}
	else
	{
		SetEvent(m_hStopEvent);
	}
}

void CMFCTestDlg::CreateEnforceFile(CMFCTestDlg* pDlg)
{
	if (pDlg)
		pDlg->CreateEnforceFileProc();
}

void CMFCTestDlg::CreateEnforceFileProc()
{
	CString str = _T("단속 파일 (*.*)|*.*|"); // 모든 파일 표시
	CFileDialog dlg(TRUE, _T("*.DAT"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);
	dlg.GetOFN().lpstrTitle = _T("생성할 TEMS 단속 파일 선택");
	CString sPath, sFileName, sCopyPath;
	CString sMin, sMax, sFileCnt;
	int iMin, iMax, iCnt, iFileCnt = 0;
	GetDlgItem(IDC_EDIT_MIN)->GetWindowText(sMin);
	GetDlgItem(IDC_EDIT_MAX)->GetWindowText(sMax);
	GetDlgItem(IDC_EDIT_FILE_CNT)->GetWindowText(sFileCnt);
	iMin = _ttoi(sMin);
	iMax = _ttoi(sMax);
	iFileCnt = _ttoi(sFileCnt);

	if (dlg.DoModal() == IDOK)
	{
		sPath = dlg.GetPathName();
		sFileName = dlg.GetFileTitle();
		while (TRUE)
		{
			CString sTemp;
			for (int i = iMin; i <= iMax; i++)
			{
				DWORD dwResult = WaitForSingleObject(m_hStopEvent, 1000);
				if (dwResult == WAIT_OBJECT_0)
				{
					m_List.AddString(_T("TEMS 제출 파일 생성 스레드 종료"));
					m_bMakingFile = FALSE;
					return;
				}
				for (int j = 0; j < iFileCnt; j++)
				{
					CString sCode, sDate;
					SYSTEMTIME st;
					sCode.Format(_T("G%04d"), i);
					sTemp = sFileName.Left(3);
					sTemp += sCode + _T("_");
					GetLocalTime(&st);
					sDate.Format(_T("%04d%02d%02d%02d%02d%02d%03d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
					sTemp += sDate + _T("_");
					sTemp += sFileName.Mid(27, 2) + _T("_");
					sTemp += sFileName.Right(2) + _T(".DAT");

					sCopyPath.Format(_T("D:\\ITSens\\Enforce\\%s"), sTemp);
					if (CopyFile(sPath, sCopyPath, 1))
					{
						CFile file;
						if (file.Open(sCopyPath, CFile::modeWrite))
						{
							file.Seek(25, CFile::begin);
							file.Write(sDate, sDate.GetLength());
							file.Close();
						}

						CString sMsg;
						sMsg.Format(_T("TEMS 제출 파일 생성 %d의 %d번째 : %s"), i, j, sTemp);
						int iSel = m_List.AddString(sMsg);
						m_List.SetCurSel(iSel);
					}
					m_bMakingFile = TRUE;
				}
			}
			if (!m_bMakingFile)
				break;
		}
		m_bMakingFile = FALSE;
	}
}

void CMFCTestDlg::CreateEnforceFileBoost(CMFCTestDlg* pDlg)
{
	if (pDlg)
		pDlg->CreateEnforceFileBoostProc();
}

void CMFCTestDlg::CreateEnforceFileBoostProc()
{
	CString str = _T("단속 파일 (*.*)|*.*|"); // 모든 파일 표시
	CFileDialog dlg(TRUE, _T("*.DAT"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);
	dlg.GetOFN().lpstrTitle = _T("생성할 TEMS 단속 파일 선택");
	CString sPath, sFileName, sCopyPath;
	CString sMin, sMax,sFileCnt;
	int iMin, iMax, iCnt, iFileCnt = 0;
	GetDlgItem(IDC_EDIT_MIN)->GetWindowText(sMin);
	GetDlgItem(IDC_EDIT_MAX)->GetWindowText(sMax);
	GetDlgItem(IDC_EDIT_FILE_CNT)->GetWindowText(sFileCnt);
	iMin = _ttoi(sMin);
	iMax = _ttoi(sMax);
	iFileCnt = _ttoi(sFileCnt);

	if (dlg.DoModal() == IDOK)
	{
		sPath = dlg.GetPathName();
		sFileName = dlg.GetFileTitle();
		while (TRUE)
		{
			DWORD dwResult = WaitForSingleObject(m_hStopEvent, 1000);
			CString sTemp;
			for (int i = iMin; i <= iMax; i++)
			{
				if (dwResult == WAIT_OBJECT_0)
				{
					m_List.AddString(_T("TEMS 제출 파일 생성(Boost) 스레드 종료"));
					m_bMakingFile = FALSE;
					return;
				}

				for (int j = 1; j <= iFileCnt; j++)
				{
					CString sCode, sDate;
					SYSTEMTIME st;
					sCode.Format(_T("G%04d"), i);
					sTemp = sFileName.Left(3);
					sTemp += sCode + _T("_");
					GetLocalTime(&st);
					sDate.Format(_T("%04d%02d%02d%02d%02d%02d%03d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
					sTemp += sDate + _T("_");
					sTemp += sFileName.Mid(27, 2) + _T("_");
					sTemp += sFileName.Right(2) + _T(".DAT");

					sCopyPath.Format(_T("D:\\ITSens\\Enforce\\%s"), sTemp);
					if (CopyFile(sPath, sCopyPath, 1))
					{
						CFile file;
						if (file.Open(sCopyPath, CFile::modeWrite))
						{
							file.Seek(25, CFile::begin);
							file.Write(sDate, sDate.GetLength());
							file.Close();
						}

						CString sMsg;
						sMsg.Format(_T("TEMS 제출 파일 생성 %d의 %d번째 : %s"), i,j,sTemp);
						int iSel = m_List.AddString(sMsg);
						m_List.SetCurSel(iSel);
					}
					m_bMakingFile = TRUE;
				}
			}
			if (!m_bMakingFile)
				break;
		}
		m_bMakingFile = FALSE;
	}
}


void CMFCTestDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnClose();
}


void CMFCTestDlg::OnBnClickedButtonSetSetupDir()
{
	CFolderPickerDialog folderDlg(NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR, NULL, NULL, NULL);
	folderDlg.GetOFN().lpstrTitle = _T("Setup 폴더 선택하세요.");
	if (folderDlg.DoModal() == IDOK) // 대화 상자를 표시하고 사용자가 확인을 선택했는지 확인
	{
		m_sSetupPath = folderDlg.GetPathName();
	}
}


void CMFCTestDlg::OnBnClickedButtonSetItsenseDir()
{
	CString str = _T("exe 파일 (*.exe)|*.exe|"); // 모든 파일 표시

	CFileDialog dlg(TRUE, _T("*.exe"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);

	if (dlg.DoModal() == IDOK) // 대화 상자를 표시하고 사용자가 확인을 선택했는지 확인
	{
		m_sITSensPath = dlg.GetPathName();
	}
}


void CMFCTestDlg::OnBnClickedButtonMakeItsensFolder()
{
	CString sMin, sMax, sPath, sSaveFolder, sName;
	int iMin, iMax, iCnt = 0;
	GetDlgItem(IDC_EDIT_MIN)->GetWindowText(sMin);
	GetDlgItem(IDC_EDIT_MAX)->GetWindowText(sMax);
	iMin = _ttoi(sMin);
	iMax = _ttoi(sMax);

	CFolderPickerDialog folderDlg(NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR, NULL, NULL, NULL);
	folderDlg.GetOFN().lpstrTitle = _T("복사할 ITSens 폴더를 선택하세요.");
	if (folderDlg.DoModal() == IDOK)
	{
		sPath = folderDlg.GetPathName();
		sName = folderDlg.GetFileName();
		CFolderPickerDialog folderDlg2(NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR, NULL, NULL, NULL);

		if (folderDlg2.DoModal() == IDOK)
		{
			folderDlg2.GetOFN().lpstrTitle = _T("복사한 ITSens를 저장할 폴더를 선택하세요.");
			sSaveFolder = folderDlg2.GetPathName();

			for (iCnt = iMin; iCnt <= iMax; ++iCnt)
			{
				CString sNewPath;
				sNewPath.Format(_T("%s\\%s%d"), sSaveFolder, sName, iCnt);

				CopyFolder(sPath, sNewPath);

				CString sModifySetupPath;
				sModifySetupPath.Format(_T("%s\\Setup.ini"),sNewPath);

				CString sOldExeName;
				CString sNewExeName;
				sOldExeName.Format(_T("%s\\ITSensViewer.exe"),sPath,iCnt);
				sNewExeName.Format(_T("%s\\ITSensViewer%d.exe"), sNewPath,iCnt);
				CopyFile(sOldExeName, sNewExeName,TRUE);

				CString sSection = _T("enforcement");
				CString sKey = _T("devid");
				CString sValue;
				sValue.Format(_T("G%04d"), iCnt);
				WritePrivateProfileString(sSection, sKey, sValue, sModifySetupPath);

			}

		}
	}

}

//https://jangjy.tistory.com/378
int CMFCTestDlg::CopyFolder(CString szFrom, CString szTo)
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	BOOL bResult = TRUE;
	TCHAR WildCard[MAX_PATH];
	TCHAR SrcFile[MAX_PATH];
	TCHAR DestFile[MAX_PATH];

	wsprintf(WildCard, "%s\\*.*", szFrom);
	CreateDirectory(szTo, NULL);
	hSrch = FindFirstFile(WildCard, &wfd);
	if (hSrch == INVALID_HANDLE_VALUE)
		return FALSE;
	while (bResult) {
		wsprintf(SrcFile, "%s\\%s", szFrom, wfd.cFileName);
		wsprintf(DestFile, "%s\\%s", szTo, wfd.cFileName);
		// 서브 디렉토리가 발견되면 서브 디렉토리를 복사한다.
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (lstrcmp(wfd.cFileName, ".") && lstrcmp(wfd.cFileName, "..")) {
				CopyFolder(SrcFile, DestFile);
			}
		}
		else {
			CopyFile(SrcFile, DestFile, FALSE);
		}
		bResult = FindNextFile(hSrch, &wfd);
	}
	FindClose(hSrch);
	return TRUE;
}

void CMFCTestDlg::OnBnClickedButtonItagent()
{
	CString sMin, sMax, sPath, sAgentProccessPath, sName;
	int iMin, iMax, iCnt = 0;
	GetDlgItem(IDC_EDIT_MIN)->GetWindowText(sMin);
	GetDlgItem(IDC_EDIT_MAX)->GetWindowText(sMax);
	iMin = _ttoi(sMin);
	iMax = _ttoi(sMax);


	CFolderPickerDialog folderDlg(NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR, NULL, NULL, NULL);
	folderDlg.GetOFN().lpstrTitle = _T("참조할 ITSens 폴더를 선택하세요.");
	if (folderDlg.DoModal() == IDOK)
	{
		sPath = folderDlg.GetPathName();	//ITSens 폴더 경로
		CString str = _T("ini 파일 (*.ini)|*.ini|"); // 모든 파일 표시
		CFileDialog fileDlg(TRUE, _T("*.ini"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);
		fileDlg.GetOFN().lpstrTitle = _T("참조한 폴더의 ITAgent 내부 process ini 파일을 선택하세요");
		if (fileDlg.DoModal() == IDOK)	//ITAgent 프로세스 폴더 경로
		{
			CString sProccessFolderPath = fileDlg.GetFolderPath();
			CString sPrecessName = fileDlg.GetFileName();

			for (iCnt = iMin; iCnt <= iMax; ++iCnt)
			{
				CString sProccessPath;
				sProccessPath.Format(_T("%s%d\\\ITSensViewer%d.exe"), sPath, iCnt,iCnt);

				CString sNewName;
				sNewName.Format(_T("%s\\ITSensViewer%d.ini"), sProccessFolderPath, iCnt);
				CopyFile(fileDlg.GetPathName(), sNewName, TRUE);

				CString sSection = _T("PROCESS");
				CString sKey = _T("PROCESS");
				CString sValue = sProccessPath;
				WritePrivateProfileString(sSection, sKey, sValue, sNewName	);
				sKey = _T("ERROR SEC");
				sValue = _T("30");
				WritePrivateProfileString(sSection, sKey, sValue, sNewName);

			}

		}
	}
}


void CMFCTestDlg::OnBnClickedButtonCreateEnforceFileBoost()
{
	if (!m_bMakingFile)
	{
		std::thread createFile(&CMFCTestDlg::CreateEnforceFileBoost, this);
		createFile.detach();
	}
	else
	{
		SetEvent(m_hStopEvent);
	}
}
