
// MFCTestDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCTest.h"
#include "MFCTestDlg.h"
#include "afxdialogex.h"
#include <thread>
#include <chrono>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <comdef.h>
#include <audioclient.h>
#include <Functiondiscoverykeys_devpkey.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define REFTIMES_PER_SEC		10000000  
#define SAMPLE_ADUIO_PATH		_T("audio.wav")		
#pragma comment(lib, "ole32.lib")

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
	m_bAudioPlaying = FALSE;
	m_bAudioStop = FALSE;
	m_bMakingEnforce = FALSE;
	m_bMakingTems = FALSE;
}

void CMFCTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);;
	DDX_Control(pDX, IDC_STATIC_Pic, m_pic);
	DDX_Control(pDX, IDC_STATIC_Pic2, m_pic2);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_COMBO_AUDIO_TYPE, m_cmbAudioType);
	DDX_Control(pDX, IDC_COMBO_AUDIO_TYPE2, m_cmbAudioType2);
	DDX_Control(pDX, IDC_EDIT_CODE, m_edCode);
	DDX_Control(pDX, IDC_COMBO_ENFOCE_BMT, m_cmbEnforceBmt);
}

BEGIN_MESSAGE_MAP(CMFCTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_KOREAN, &CMFCTestDlg::OnBnClickedButtonKorean)
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON_CREATE_ENFORCE_FILE, &CMFCTestDlg::OnBnClickedButtonCreateTemsFile)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_SET_SETUP_DIR, &CMFCTestDlg::OnBnClickedButtonSetSetupDir)
	ON_BN_CLICKED(IDC_BUTTON_SET_ITSENSE_DIR, &CMFCTestDlg::OnBnClickedButtonSetItsenseDir)
	ON_BN_CLICKED(IDC_BUTTON_MAKE_ITSENS_FOLDER, &CMFCTestDlg::OnBnClickedButtonMakeItsensFolder)
	ON_BN_CLICKED(IDC_BUTTON_ITAGENT, &CMFCTestDlg::OnBnClickedButtonItagent)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_TEMS_FILE, &CMFCTestDlg::OnBnClickedButtonCreateEnforceFileBoost)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_SERIAL, &CMFCTestDlg::OnBnClickedButtonOpenSerial)
	ON_BN_CLICKED(IDC_BUTTON_AUDIO_REFRESH, &CMFCTestDlg::OnBnClickedButtonAudioRefresh)
	ON_BN_CLICKED(IDC_BUTTON_PLAY_AUDIO_1, &CMFCTestDlg::OnBnClickedButtonPlayAudio1)
	ON_BN_CLICKED(IDC_BUTTON_PLAY_AUDIO_2, &CMFCTestDlg::OnBnClickedButtonPlayAudio2)
	ON_BN_CLICKED(IDC_BUTTON_PLAY_AUDIO_3, &CMFCTestDlg::OnBnClickedButtonPlayAudio3)
	ON_CBN_SELCHANGE(IDC_COMBO_AUDIO_TYPE, &CMFCTestDlg::OnCbnSelchangeComboAudioType)
	ON_CBN_SELCHANGE(IDC_COMBO_AUDIO_TYPE2, &CMFCTestDlg::OnCbnSelchangeComboAudioType2)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_AUDIO, &CMFCTestDlg::OnBnClickedButtonLoadAudio)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_AUDIO2, &CMFCTestDlg::OnBnClickedButtonLoadAudio2)
	ON_BN_CLICKED(IDC_BUTTON_MAKE_ENFORCE_FILE, &CMFCTestDlg::OnBnClickedButtonMakeEnforce)
	ON_BN_CLICKED(IDC_BUTTON_ICON, &CMFCTestDlg::OnBnClickedButtonIcon)
	ON_BN_CLICKED(IDC_BUTTON_ICON_TRANSPARENT, &CMFCTestDlg::OnBnClickedButtonIconTransparent)
	ON_BN_CLICKED(IDC_BUTTON_VALID_FORMAT, &CMFCTestDlg::OnBnClickedButtonValidFormat)
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
	Init();
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCTestDlg::Init()
{
	GetDlgItem(IDC_EDIT_STRING)->SetWindowText(_T("안녕하세요"));


	m_hStopTems = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hStopEnforce = CreateEvent(NULL, FALSE, FALSE, NULL);

	GetDlgItem(IDC_EDIT_FILE_CNT)->SetWindowText(_T("1"));
	GetDlgItem(IDC_EDIT_MIN)->SetWindowText(_T("1"));
	GetDlgItem(IDC_EDIT_MAX)->SetWindowText(_T("10"));

	m_cmbEnforceBmt.AddString(_T("단속파일 저장 경로: BMT"));
	m_cmbEnforceBmt.SetItemData(0, 0);
	m_cmbEnforceBmt.AddString(_T("단속파일 저장 경로: PASS"));
	m_cmbEnforceBmt.SetItemData(1, 1);

	GetAudioOutputDevice();

	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_iPixelSize = 128;
	m_pImageList.Create(m_iPixelSize, m_iPixelSize, ILC_COLOR32 | ILC_MASK, 0, 1);
	//m_pImageList.SetBkColor(RGB(255, 255, 255));
	//m_pImageList.SetBkColor(CLR_NONE);
	if (m_pImageList.GetImageCount() == 0)
	{
		m_pImageList.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	}
	//IDI_ICON_DIRECTION_STRAIGHT
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
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);  // dc는 원본 화면 DC 또는 다른 CDC

		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(&dc, m_iPixelSize, m_iPixelSize);  // 비트맵 크기 설정
		CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

		CRgn rgn;
		rgn.CreateEllipticRgn(0, 0, m_iPixelSize, m_iPixelSize);  // 타원 영역 (좌상단(50, 50), 우하단(200, 200))

		memDC.SelectClipRgn(&rgn);  // 타원 영역만 그리기
		memDC.FillSolidRect(0, 0, m_iPixelSize, m_iPixelSize, RGB(255, 0, 0));  // 빨간색으로 타원 영역 채우기

		dc.SelectClipRgn(&rgn);
		dc.BitBlt(0, 0, m_iPixelSize, m_iPixelSize, &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBitmap);  // 원본 비트맵 복원
	/*	CDC* pMemDC;
		CBitmap* pOldBitmap;
		pMemDC = new CDC;
		pMemDC->CreateCompatibleDC(&dc);

		CBitmap bmpDirection;
		bmpDirection.CreateCompatibleBitmap(&dc, m_iPixelSize, m_iPixelSize);
		pOldBitmap = pMemDC->SelectObject(&bmpDirection);
		
		CRgn rgn;
		rgn.*/


		//pMemDC->FillSolidRect(0, 0, m_iPixelSize, m_iPixelSize, RGB(255, 255, 255));
		// 
		//m_pImageList.Draw(&dc, 0, CPoint(0, 0), ILD_TRANSPARENT);						//dc에 직접 그리는게 젤 확실 (dc에 그리기전에 비트맵을 새로 다시 그려? 근데 그러면 리소스 낭비 그릴때마다 다시 그려야함, 아이콘 객체를 가져와서 투명도 조절 후 CImageList에 저장하는 것이 맞는듯?)
		//dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), pMemDC, 0, 0, SRCCOPY);
		//pMemDC->SelectObject(pOldBitmap);
		//delete pMemDC;
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
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

void CMFCTestDlg::OnBnClickedButtonCreateTemsFile()
{
	if (!m_bMakingTems)
	{
		std::thread createFile(&CMFCTestDlg::CreateTemsFile, this);
		createFile.detach();
	}
	else
	{
		SetEvent(m_hStopTems);
	}
}

void CMFCTestDlg::OnBnClickedButtonMakeEnforce()
{
	if (!m_bMakingEnforce)
	{
		std::thread createFile(&CMFCTestDlg::CreateEnforceFile, this);
		createFile.detach();
	}
	else
	{
		SetEvent(m_hStopEnforce);
	}
}


void CMFCTestDlg::CreateTemsFile(CMFCTestDlg* pDlg)
{
	if (pDlg)
		pDlg->CreateTemsFileProc();
}

void CMFCTestDlg::CreateTemsFileProc()
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
				DWORD dwResult = WaitForSingleObject(m_hStopTems, 1000);
				if (dwResult == WAIT_OBJECT_0)
				{
					m_List.AddString(_T("TEMS 제출 파일 생성 스레드 종료"));
					m_bMakingTems = FALSE;
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
					m_bMakingTems = TRUE;
				}
			}
			if (!m_bMakingTems)
				break;
		}
		m_bMakingTems = FALSE;
	}
}

void CMFCTestDlg::CreateTemsFileBoost(CMFCTestDlg* pDlg)
{
	if (pDlg)
		pDlg->CreateTemsFileBoostProc();
}

void CMFCTestDlg::CreateTemsFileBoostProc()
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
			DWORD dwResult = WaitForSingleObject(m_hStopTems, 1000);
			CString sTemp;
			for (int i = iMin; i <= iMax; i++)
			{
				if (dwResult == WAIT_OBJECT_0)
				{
					m_List.AddString(_T("TEMS 제출 파일 생성(Boost) 스레드 종료"));
					m_bMakingTems = FALSE;
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
					m_bMakingTems = TRUE;
				}
			}
			if (!m_bMakingTems)
				break;
		}
		m_bMakingTems = FALSE;
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
	if (!m_bMakingTems)
	{
		std::thread createFile(&CMFCTestDlg::CreateTemsFileBoost, this);
		createFile.detach();
	}
	else
	{
		SetEvent(m_hStopTems);
	}
}


void CMFCTestDlg::OnBnClickedButtonOpenSerial()
{
	CString sSerial, sMsg;
	int iSerial;
	GetDlgItem(IDC_EDIT_SERIAL)->GetWindowText(sSerial);
	iSerial = _ttoi(sSerial);
	sSerial.Format(_T("COM%d"), iSerial);

	if(m_hSerial == NULL)
	{ 
	m_hSerial = CreateFile(sSerial, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,0);
	if (m_hSerial == INVALID_HANDLE_VALUE)
	{
		sMsg.Format(_T("시리얼 포트 %s 를 열 수 없습니다."), sSerial);
		AddListBox(sMsg);
	}
	else
	{
		sMsg.Format(_T("시리얼 포트 %s 를 열었습니다."), sSerial);
		AddListBox(sMsg);

		//시리얼 포트 설정
		DCB dcbSerialParams = { 0 };
		dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

		if (!GetCommState(m_hSerial, &dcbSerialParams))
		{
			sMsg.Format(_T("시리얼 포트 % 상태 얻기 실패"), sSerial);
			AddListBox(sMsg);
		}

		dcbSerialParams.BaudRate = CBR_9600;
		dcbSerialParams.ByteSize = 8;
		dcbSerialParams.StopBits = ONESTOPBIT;
		dcbSerialParams.Parity = NOPARITY;

		if (!SetCommState(m_hSerial, &dcbSerialParams)) {
			sMsg.Format(_T("시리얼 포트 %s 상태 세팅 실패"), sSerial);
			AddListBox(sMsg);
		}
	}
		CString sInput;
		GetDlgItem(IDC_EDIT_STRING)->GetWindowText(sInput);
		DWORD bytes_written;
		if (!WriteFile(m_hSerial, sInput, sInput.GetLength(), &bytes_written, NULL))
		{
			sMsg.Format(_T("시리얼 포트 %s 쓰기 실패"), sSerial);
			AddListBox(sMsg);
		}
		else
		{
			sMsg.Format(_T("시리얼 포트 %s 쓰기"), sInput);
			AddListBox(sMsg);
		}
	}

	if (m_hSerial != NULL)
	{
		CloseHandle(m_hSerial);
		m_hSerial = NULL;
	}
}

void CMFCTestDlg::AddListBox(CString sMsg)
{
	int iSel = m_List.AddString(sMsg);
	m_List.SetCurSel(iSel);
}

void CMFCTestDlg::PlayAudio(CMFCTestDlg* pDlg)
{
	if(pDlg)
		pDlg->PlayAudioProc();
}

void CMFCTestDlg::PlayAudio2(CMFCTestDlg* pDlg)
{
	if (pDlg)
		pDlg->PlayAudioProc2();
}

void CMFCTestDlg::PlayAudioProc2()
{
	do
	{
		CString sSelectedAudioDeviceID;
		GetDlgItem(IDC_EDIT_DEVICE_ID2)->GetWindowText(sSelectedAudioDeviceID);

		HRESULT hr = CoInitialize(nullptr);																				//COM 초기화
		if (FAILED(hr))
		{
			return;
		}

		const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);											//COM 객체를 얻기 위한 아이디
		const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);

		IMMDeviceEnumerator* pMMDeviceEnumerator = nullptr;																//컬렉션과 열거자 객체를 담기 위한 포인터 변수
		IMMDeviceCollection* pMMDeviceCollection = nullptr;

		hr = CoCreateInstance(
			CLSID_MMDeviceEnumerator, nullptr,
			CLSCTX_ALL, IID_IMMDeviceEnumerator,
			(void**)&pMMDeviceEnumerator);																				//열거자 객체 생성, 첫번째 com의 UUID

		if (FAILED(hr))
		{
			if (!pMMDeviceEnumerator)
			{
				pMMDeviceEnumerator->Release();
			}
			CoUninitialize();
			return;
		}

		hr = pMMDeviceEnumerator->EnumAudioEndpoints(EDataFlow::eRender, DEVICE_STATE_ACTIVE, &pMMDeviceCollection);	//오디오 Collection Get

		if (FAILED(hr))
		{
			if (!pMMDeviceEnumerator)
			{
				pMMDeviceEnumerator->Release();
			}
			if (!pMMDeviceCollection)
			{
				pMMDeviceCollection->Release();
			}
			CoUninitialize();
			return;
		}

		UINT uiCount = 0;
		pMMDeviceCollection->GetCount(&uiCount);																					//오디오 디바이스 개수 Get

		for (int i = 0; i < uiCount; ++i)
		{
			IMMDevice* pDevice = nullptr;
			pMMDeviceCollection->Item(i, &pDevice);

			if (pDevice)
			{
				LPWSTR pwszID = nullptr;
				hr = pDevice->GetId(&pwszID);
				if (SUCCEEDED(hr))
				{
					CString sDeviceID(pwszID);

					if (sSelectedAudioDeviceID == pwszID)
					{

						IAudioClient* pAudioClient = nullptr;
						hr = pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, (void**)&pAudioClient);				//오디오 클라이언트 Get

						if (SUCCEEDED(hr) && pAudioClient)
						{
							REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;												//요청할 오디오 지속시간
							hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE::AUDCLNT_SHAREMODE_EXCLUSIVE, 0, hnsRequestedDuration, 0, &m_wavFormat[1], nullptr);	//오디오 클라이언트 초기화
							if (SUCCEEDED(hr))
							{
								IAudioRenderClient* pAudioRenderClient = nullptr;												//오디오 출력장치에 데이터 전달하고 버퍼 관리 역할 인터페이스
								UINT32 bufferFrameCount;
								pAudioClient->GetBufferSize(&bufferFrameCount);													//버퍼 크기 가져오기
								hr = pAudioClient->GetService(__uuidof(IAudioRenderClient), (void**)&pAudioRenderClient);		//오디오 렌더링을 위한 인터페이스 요청

								if (SUCCEEDED(hr) && pAudioRenderClient)
								{
									UINT32 numFramesAvailable = 0;
									BYTE* pData;
									pAudioClient->Start();																		//오디오 클라이언트 시작
									size_t iOffset = 0;																			//오디오 데이터 오프셋 초기화


									while (iOffset < m_iAudioDataSize[1] && !m_bAudioStop)
									{
										pAudioClient->GetCurrentPadding(&numFramesAvailable);									//현재 패딩(아직 출력되지 않은 오디오 프레임 수)
										numFramesAvailable = bufferFrameCount - numFramesAvailable;								//사용 가능 프레임 수 계산

										pAudioRenderClient->GetBuffer(numFramesAvailable, &pData);								//렌더 클라이언트에서 버퍼 가져오기
										memcpy(pData, m_pAudioData[1] + iOffset, numFramesAvailable * m_wavFormat[1].nBlockAlign);    //사용 가능 프레임 * 한 오디오 프레임이 차지하는 바이트수 만큼을 복사
										iOffset += numFramesAvailable * m_wavFormat[1].nBlockAlign;								//오프셋 증가
										pAudioRenderClient->ReleaseBuffer(numFramesAvailable, 0);								//사용완료한 버퍼 release
									}
									pAudioClient->Stop();
									pAudioRenderClient->Release();
								}
							}
						}
						pAudioClient->Release();
					}
					pDevice->Release();
				}
			}
		}

		pMMDeviceCollection->Release();
		pMMDeviceEnumerator->Release();
		CoUninitialize();
	} while (FALSE);

	m_bAudioPlaying = FALSE;
}

void CMFCTestDlg::GetDeviceID(int iComboBox)
{
	int iSel = -1;
	if (iComboBox == 1)
	{
		iSel = m_cmbAudioType.GetCurSel();
	}
	else if (iComboBox == 2)
	{
		iSel = m_cmbAudioType2.GetCurSel();
	}

	HRESULT hr = CoInitialize(nullptr); //COM 초기화

	if (FAILED(hr))
	{
		return;
	}

	const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);	//COM 객체를 얻기 위한 아이디
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);

	IMMDeviceEnumerator* pMMDeviceEnumerator = nullptr;	//컬렉션과 열거자 객체를 담기 위한 포인터 변수
	IMMDeviceCollection* pMMDeviceCollection = nullptr;

	hr = CoCreateInstance(
		CLSID_MMDeviceEnumerator, nullptr,
		CLSCTX_ALL, IID_IMMDeviceEnumerator,
		(void**)&pMMDeviceEnumerator);	//열거자 객체 생성, 첫번째 com의 UUID

	if (FAILED(hr))
	{
		if (!pMMDeviceEnumerator)
		{
			pMMDeviceEnumerator->Release();
		}
		CoUninitialize();
		return;
	}

	hr = pMMDeviceEnumerator->EnumAudioEndpoints(EDataFlow::eRender, DEVICE_STATE_ACTIVE, &pMMDeviceCollection);	//오디오 Collection Get

	if (FAILED(hr))
	{
		if (!pMMDeviceEnumerator)
		{
			pMMDeviceEnumerator->Release();
		}
		if (!pMMDeviceCollection)
		{
			pMMDeviceCollection->Release();
		}
		CoUninitialize();
		return;
	}

	UINT uiCount = 0;
	pMMDeviceCollection->GetCount(&uiCount);	//오디오 디바이스 개수 Get

	if (uiCount > iSel)
	{
		IMMDevice* pDevice = nullptr;
		IPropertyStore* pProperty = nullptr;
		PROPVARIANT value{};
		pMMDeviceCollection->Item(iSel, &pDevice);

		if (pDevice)
		{
			LPWSTR pwszID = nullptr;
			hr = pDevice->GetId(&pwszID);
			if (SUCCEEDED(hr))
			{
				CString sDevice;
				sDevice.Format(_T("%S"), pwszID);
				if(iComboBox == 1)
					GetDlgItem(IDC_EDIT_DEVICE_ID)->SetWindowText(sDevice);
				else if(iComboBox == 2)
					GetDlgItem(IDC_EDIT_DEVICE_ID2)->SetWindowText(sDevice);
				pDevice->Release();
				CoTaskMemFree(pwszID);
			}
		}

		PropVariantClear(&value);
	}

	pMMDeviceCollection->Release();
	pMMDeviceEnumerator->Release();
	CoUninitialize();
}

void CMFCTestDlg::CreateEnforceFile(CMFCTestDlg* pDlg)
{
	if (pDlg)
		pDlg->CreateEnforceFileProc();
}

void CMFCTestDlg::CreateEnforceFileProc()
{
	CString str = _T("단속 파일 (*.*)|*.*|"); // 모든 파일 표시
	CFileDialog dlg(TRUE, _T("*.*"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);
	dlg.GetOFN().lpstrTitle = _T("생성할 단속 파일 선택");
	CString sPath,sName,sExt,sCopyPath;
	if (dlg.DoModal() == IDOK)
	{
		sPath = dlg.GetPathName();
		sName = dlg.GetFileTitle();
		sExt = dlg.GetFileExt();
		while (TRUE)
		{	
			DWORD dwResult = WaitForSingleObject(m_hStopEnforce, 200);
			if (dwResult == WAIT_OBJECT_0)
			{
				m_List.AddString(_T("ENFORCE 제출 파일 생성 스레드 종료"));
				m_bMakingEnforce = FALSE;
				return;
			}
		
			CString sEnforceFileName;
			CString sLane;
			sLane = sName.Mid(17, 2);
			SYSTEMTIME st;
			GetLocalTime(&st);
			sEnforceFileName.Format(_T("%04d%02d%02d%02d%02d%02d%03d%s.%s"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,sLane, sExt);

			int iItem = m_cmbEnforceBmt.GetItemData(m_cmbEnforceBmt.GetCurSel());
			if(iItem == 0)
				sCopyPath.Format(_T("D:\\ITSens\\Bmt\\Pass\\%s"), sEnforceFileName);
			else
				sCopyPath.Format(_T("D:\\ITSens\\Pass\\%s"), sEnforceFileName);

			if(CopyFile(sPath, sCopyPath, 1))
			{
				
				CString sMsg;
				sMsg.Format(_T("ENFORCE 파일 생성(PASS 경로): %s"), sEnforceFileName);
				int iSel = m_List.AddString(sMsg);
				m_List.SetCurSel(iSel);
			}
			if (iItem == 0)
				sCopyPath.Format(_T("D:\\ITSens\\Bmt\\Enforce\\%s"), sEnforceFileName);
			else
				sCopyPath.Format(_T("D:\\ITSens\\Enforce\\%s"), sEnforceFileName);

			if (CopyFile(sPath, sCopyPath, 1))
			{

				CString sMsg;
				sMsg.Format(_T("ENFORCE 파일 생성(ENFORCE 경로): %s"), sEnforceFileName);
				int iSel = m_List.AddString(sMsg);
				m_List.SetCurSel(iSel);
			}
			m_bMakingEnforce = TRUE;
			
			
			if (!m_bMakingEnforce)
				break;
		}
		m_bMakingEnforce = FALSE;
	}
}

void CMFCTestDlg::PlayAudioProc()
{
	do
	{
		CString sSelectedAudioDeviceID;
		GetDlgItem(IDC_EDIT_DEVICE_ID)->GetWindowText(sSelectedAudioDeviceID);

		HRESULT hr = CoInitialize(nullptr);																				//COM 초기화
		if (FAILED(hr))
		{
			return;
		}

		const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);											//COM 객체를 얻기 위한 아이디
		const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);

		IMMDeviceEnumerator* pMMDeviceEnumerator = nullptr;																//컬렉션과 열거자 객체를 담기 위한 포인터 변수
		IMMDeviceCollection* pMMDeviceCollection = nullptr;

		hr = CoCreateInstance(
			CLSID_MMDeviceEnumerator, nullptr,
			CLSCTX_ALL, IID_IMMDeviceEnumerator,
			(void**)&pMMDeviceEnumerator);																				//열거자 객체 생성, 첫번째 com의 UUID

		if (FAILED(hr))
		{
			if (!pMMDeviceEnumerator)
			{
				pMMDeviceEnumerator->Release();
			}
			CoUninitialize();
			return;
		}

		hr = pMMDeviceEnumerator->EnumAudioEndpoints(EDataFlow::eRender, DEVICE_STATE_ACTIVE, &pMMDeviceCollection);	//오디오 Collection Get

		if (FAILED(hr))
		{
			if (!pMMDeviceEnumerator)
			{
				pMMDeviceEnumerator->Release();
			}
			if (!pMMDeviceCollection)
			{
				pMMDeviceCollection->Release();
			}
			CoUninitialize();
			return;
		}

		UINT uiCount = 0;
		pMMDeviceCollection->GetCount(&uiCount);																					//오디오 디바이스 개수 Get

		for (int i = 0; i < uiCount; ++i)
		{
			IMMDevice* pDevice = nullptr;
			pMMDeviceCollection->Item(i, &pDevice);

			if (pDevice)
			{
				LPWSTR pwszID = nullptr;
				hr = pDevice->GetId(&pwszID);
				if (SUCCEEDED(hr))
				{
					CString sDeviceID(pwszID);

					if (sSelectedAudioDeviceID == pwszID)
					{
						
						IAudioClient* pAudioClient = nullptr;
						hr = pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, (void**)&pAudioClient);				//오디오 클라이언트 Get

						if (SUCCEEDED(hr) && pAudioClient)
						{
							REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;												//요청할 오디오 지속시간
							hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE::AUDCLNT_SHAREMODE_EXCLUSIVE, 0, hnsRequestedDuration, 0, &m_wavFormat[0], nullptr);	//오디오 클라이언트 초기화
							if (SUCCEEDED(hr))
							{
								IAudioRenderClient* pAudioRenderClient = nullptr;												//오디오 출력장치에 데이터 전달하고 버퍼 관리 역할 인터페이스
								UINT32 bufferFrameCount;
								pAudioClient->GetBufferSize(&bufferFrameCount);													//버퍼 크기 가져오기
								hr = pAudioClient->GetService(__uuidof(IAudioRenderClient), (void**)&pAudioRenderClient);		//오디오 렌더링을 위한 인터페이스 요청

								if (SUCCEEDED(hr) && pAudioRenderClient)
								{
									UINT32 numFramesAvailable = 0;
									BYTE* pData;
									pAudioClient->Start();																		//오디오 클라이언트 시작
									size_t iOffset = 0;																			//오디오 데이터 오프셋 초기화

								
									while (iOffset < m_iAudioDataSize[0] && !m_bAudioStop)
									{
										pAudioClient->GetCurrentPadding(&numFramesAvailable);									//현재 패딩(아직 출력되지 않은 오디오 프레임 수)
										numFramesAvailable = bufferFrameCount - numFramesAvailable;								//사용 가능 프레임 수 계산

										pAudioRenderClient->GetBuffer(numFramesAvailable, &pData);								//렌더 클라이언트에서 버퍼 가져오기
										memcpy(pData, m_pAudioData[0] + iOffset, numFramesAvailable * m_wavFormat[0].nBlockAlign);    //사용 가능 프레임 * 한 오디오 프레임이 차지하는 바이트수 만큼을 복사
										iOffset += numFramesAvailable * m_wavFormat[0].nBlockAlign;								//오프셋 증가
										pAudioRenderClient->ReleaseBuffer(numFramesAvailable, 0);								//사용완료한 버퍼 release
									}
									pAudioClient->Stop();
									pAudioRenderClient->Release();
								}
							}
						}
						pAudioClient->Release();
					}
					pDevice->Release();
				}
			}
		}

		pMMDeviceCollection->Release();
		pMMDeviceEnumerator->Release();
		CoUninitialize();
	} while (FALSE);

	m_bAudioPlaying = FALSE;
}



void CMFCTestDlg::OnBnClickedButtonAudioRefresh()
{
	m_cmbAudioType.ResetContent();
	m_cmbAudioType2.ResetContent();
	GetAudioOutputDevice();
}

void CMFCTestDlg::GetAudioOutputDevice()
{
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED); //com 초기화

	if (FAILED(hr))
	{
		return;
	}

	const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);	//COM 객체를 얻기 위한 아이디
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);

	IMMDeviceEnumerator* pMMDeviceEnumerator = nullptr;	//컬렉션과 열거자 객체를 담기 위한 포인터 변수
	IMMDeviceCollection* pMMDeviceCollection = nullptr;

	hr = CoCreateInstance(
		CLSID_MMDeviceEnumerator, nullptr,
		CLSCTX_ALL, IID_IMMDeviceEnumerator,
		(void**)&pMMDeviceEnumerator);	//열거자 객체 생성, 첫번째 com의 UUID

	if (FAILED(hr))
	{
		if (!pMMDeviceEnumerator)
		{
			pMMDeviceEnumerator->Release();
		}
		CoUninitialize();
		return;
	}

	hr = pMMDeviceEnumerator->EnumAudioEndpoints(EDataFlow::eRender, DEVICE_STATE_ACTIVE, &pMMDeviceCollection);	//오디오 Collection Get

	if (FAILED(hr))
	{
		if (!pMMDeviceEnumerator)
		{
			pMMDeviceEnumerator->Release();
		}
		if (!pMMDeviceCollection)
		{
			pMMDeviceCollection->Release();
		}
		CoUninitialize();
		return;
	}

	UINT uiCount = 0;
	pMMDeviceCollection->GetCount(&uiCount);	//오디오 디바이스 개수 Get

	for (int i = 0; i < uiCount; ++i)
	{
		IMMDevice* pDevice = nullptr;
		IPropertyStore* pProperty = nullptr;
		PROPVARIANT value{};
		pMMDeviceCollection->Item(i, &pDevice);

		if (pDevice)
		{
			pDevice->OpenPropertyStore(STGM_READ, &pProperty);
			pProperty->GetValue(PKEY_Device_FriendlyName, &value);
			CString sDeviceName(value.pwszVal);
			IAudioEndpointVolume* pEndpointVolume = nullptr;	//볼륨컨트롤 인터페이스 Get
			pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, (void**)&pEndpointVolume);

			if (pEndpointVolume)
			{
				float fVolume = 0.0f;
				int iVolume = 0;
				hr = pEndpointVolume->GetMasterVolumeLevelScalar(&fVolume);	//볼륨 값 Get
				if (SUCCEEDED(hr))
				{
					iVolume = fVolume * 100.0f;
				}
			


				LPWSTR pwszID = nullptr;
				hr = pDevice->GetId(&pwszID);								//디바이스 ID Get

				if (SUCCEEDED(hr))
				{
					CString sDeviceID(pwszID);
					CoTaskMemFree(pwszID);
		

					//모두 성공했기 때문에 반영
					int iSel = m_cmbAudioType.AddString(sDeviceName);
					m_cmbAudioType2.AddString(sDeviceName);
					//m_cmbAudioType.SetItemData(iSel, sDeviceID);

				}
				pEndpointVolume->Release();
			}

			PropVariantClear(&value);
			pDevice->Release();
		}
	}

	pMMDeviceCollection->Release();
	pMMDeviceEnumerator->Release();
	CoUninitialize();
}

BOOL CMFCTestDlg::LoadWavFile(CString sFileName, int iOrder)
{
	CFile file;
	BYTE header[44];
	if (file.Open(sFileName, CFile::modeRead))
	{
		file.Read(header, 44);
		/*
			*reinterpret_cast<WORD*> 주어진 포인터를 WORD 타입 포인터로 형변환 후 역참조하여 값을 가져옴.
			reinterpret_cast는 포인터를 다른 타입으로 변환할 때

		*/
		m_wavFormat[iOrder].wFormatTag = *reinterpret_cast<WORD*>(header + 20);			//PCM(비압축 오디오)의 경우 1
		m_wavFormat[iOrder].nChannels = *reinterpret_cast<WORD*>(header + 22);			//오디어 데이터 채널 (Mono: 1 Stereo: 2)
		m_wavFormat[iOrder].nSamplesPerSec = *reinterpret_cast<DWORD*>(header + 24);		//오디오 샘플링 주기 (초당 샘플 수, 높을수록 재생되는 오디오 주파수가 넓어지고 품질 향상
		m_wavFormat[iOrder].nAvgBytesPerSec = *reinterpret_cast<DWORD*>(header + 28);	//초당 평균 바이트 수, 데이터 전송률
		m_wavFormat[iOrder].nBlockAlign = *reinterpret_cast<WORD*>(header + 32);		//하나의 오디오 블록이 차지하는 바이트 수
		m_wavFormat[iOrder].wBitsPerSample = *reinterpret_cast<WORD*>(header + 34);		//샘플당 비트 수 (스트레오 16비트)
		m_wavFormat[iOrder].cbSize = 0;													//추가적인 사용자 정의 데이터

		m_pAudioData[iOrder] = new BYTE[file.GetLength() - 44];
		file.Read(m_pAudioData[iOrder], file.GetLength() - 44);
		m_iAudioDataSize[iOrder] = file.GetLength() - 44;
		file.Close();
		return TRUE;
	}
	return FALSE;
}

void CMFCTestDlg::OnBnClickedButtonPlayAudio1()
{
	std::thread t1(&CMFCTestDlg::PlayAudio, this);
	t1.detach();
}


void CMFCTestDlg::OnBnClickedButtonPlayAudio2()
{
	std::thread t2(&CMFCTestDlg::PlayAudio2, this);
	t2.detach();
}


void CMFCTestDlg::OnBnClickedButtonPlayAudio3()
{
	std::thread t1(&CMFCTestDlg::PlayAudio, this);
	t1.detach();

	std::thread t2(&CMFCTestDlg::PlayAudio2, this);
	t2.detach();
}


void CMFCTestDlg::OnCbnSelchangeComboAudioType()
{
	GetDeviceID(1);
}


void CMFCTestDlg::OnCbnSelchangeComboAudioType2()
{
	GetDeviceID(2);
}


void CMFCTestDlg::OnBnClickedButtonLoadAudio()
{
	CString sFilter = _T("오디오 파일 (*.wav) | *.wav;||");
	CFileDialog dlg(TRUE, _T(""), _T(""), OFN_HIDEREADONLY, sFilter, NULL, 0, 0);

	if (dlg.DoModal())
	{
		CString sPath = dlg.GetPathName();
		LoadWavFile(sPath,0);
	}
}


void CMFCTestDlg::OnBnClickedButtonLoadAudio2()
{
	CString sFilter = _T("오디오 파일 (*.wav) | *.wav;||");
	CFileDialog dlg(TRUE, _T(""), _T(""), OFN_HIDEREADONLY, sFilter, NULL, 0, 0);

	if (dlg.DoModal())
	{
		CString sPath = dlg.GetPathName();
		LoadWavFile(sPath,1);
	}
}



void CMFCTestDlg::OnBnClickedButtonIcon()
{

}


void CMFCTestDlg::OnBnClickedButtonIconTransparent()
{
	//ICONINFO iconInfo;
	//HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DIRECTION_STRAIGHT);
	//if (!GetIconInfo(hIcon, &iconInfo))
	//	return;

	//if (hIcon)
	//{
	//	CDC hDC = 
	//	HDC hMemDC = CreateCompatibleDC(hDC);

	//	// CImage를 아이콘 크기로 생성 (32비트 RGBA 지원)
	//	image.Create(width, height, 32);
	//	HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, image);
	//}
}

bool CMFCTestDlg::ReadWavFormatFromFile(const CString& filePath, WAVEFORMATEX& outWf)
{
	CFile file;
	if (!file.Open(filePath, CFile::modeRead | CFile::typeBinary))
		return false;

	DWORD chunkId = 0, chunkSize = 0, format = 0;
	file.Read(&chunkId, 4);
	file.Read(&chunkSize, 4);
	file.Read(&format, 4);

	if (chunkId != 'FFIR' || format != 'EVAW') // "RIFF", "WAVE"
		return false;

	while (file.GetPosition() < file.GetLength())
	{
		DWORD subChunkId = 0;
		DWORD subChunkSize = 0;
		file.Read(&subChunkId, 4);
		file.Read(&subChunkSize, 4);

		if (subChunkId == ' tmf') // "fmt "
		{
			if (subChunkSize < 16) return false; // 최소 WAVEFORMAT 크기

			BYTE* pFmtBuffer = new BYTE[subChunkSize];
			file.Read(pFmtBuffer, subChunkSize);

			memcpy(&outWf, pFmtBuffer, min(subChunkSize, sizeof(WAVEFORMATEX)));

			delete[] pFmtBuffer;
			return true;
		}
		else
		{
			// 다른 chunk는 건너뜀
			file.Seek(subChunkSize, CFile::current);
		}
	}

	return false;
}

void CMFCTestDlg::OnBnClickedButtonValidFormat()
{
	CFileDialog dlg(TRUE, _T("wav"), nullptr, OFN_FILEMUSTEXIST, _T("WAV Files (*.wav)|*.wav||"));
	if (dlg.DoModal() != IDOK)
		return;

	CString filePath = dlg.GetPathName();
	WAVEFORMATEX wf = {};

	if (!ReadWavFormatFromFile(filePath, wf))
	{
		AfxMessageBox(_T("WAV 파일 포맷을 읽을 수 없습니다."));
		return;
	}

	// WASAPI 디바이스 초기화
	CoInitialize(nullptr);

	CComPtr<IMMDeviceEnumerator> pEnumerator;
	CComPtr<IMMDevice> pDevice;
	CComPtr<IAudioClient> pAudioClient;

	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, IID_PPV_ARGS(&pEnumerator));
	if (FAILED(hr)) { AfxMessageBox(_T("Enumerator 실패")); return; }

	hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
	if (FAILED(hr)) { AfxMessageBox(_T("오디오 디바이스 실패")); return; }

	hr = pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, (void**)&pAudioClient);
	if (FAILED(hr)) { AfxMessageBox(_T("IAudioClient 실패")); return; }

	// 지원 여부 체크
	WAVEFORMATEX* pClosest = nullptr;
	hr = pAudioClient->IsFormatSupported(AUDCLNT_SHAREMODE_EXCLUSIVE, &wf, &pClosest);

	CString sResult;
	if (pClosest) CoTaskMemFree(pClosest);

	if (hr == S_OK)
	{
		sResult.Format(_T("지원됨\nSampleRate: %d Hz\nBitDepth: %d\nChannels: %d"),
			wf.nSamplesPerSec, wf.wBitsPerSample, wf.nChannels);
	}
	else if (hr == S_FALSE)
	{
		sResult.Format(_T("유사 포맷만 지원\nSampleRate: %d Hz\nBitDepth: %d\nChannels: %d"),
			wf.nSamplesPerSec, wf.wBitsPerSample, wf.nChannels);
	}
	else
	{
		sResult.Format(_T("지원되지 않음\nHRESULT: 0x%08X"), hr);
	}

	AfxMessageBox(sResult);
}
