
// MFCTestDlg.h: 헤더 파일
//

#pragma once
#include "Animal.h"
#include "Dog.h"
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")


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
	void Init();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonKorean();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnBnClickedButtonCreateTemsFile();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonSetSetupDir();
	afx_msg void OnBnClickedButtonSetItsenseDir();
	afx_msg void OnBnClickedButtonMakeItsensFolder();
	afx_msg void OnBnClickedButtonItagent();
	afx_msg void OnBnClickedButtonCreateEnforceFileBoost();
	afx_msg void OnBnClickedButtonOpenSerial();

	static void CreateTemsFile(CMFCTestDlg* pDlg);
	void CreateTemsFileProc();

	static void CreateTemsFileBoost(CMFCTestDlg* pDlg);
	void CreateTemsFileBoostProc();

	int CopyFolder(CString szFrom, CString szTo);

	void AddListBox(CString sMsg);

	static void PlayAudio(CMFCTestDlg* pDlg);
	void PlayAudioProc();

	static void PlayAudio2(CMFCTestDlg* pDlg);
	void PlayAudioProc2();

	void GetDeviceID(int iComboBox);


	static void CreateEnforceFile(CMFCTestDlg* pDlg);
	void CreateEnforceFileProc();

public:
	CStatic m_pic;
	char* m_pData;
	int m_iHeight;
	int m_iWidth;
	BITMAPFILEHEADER m_bitFileHeader;
	BITMAPINFOHEADER m_bitInfoHeader;

	int m_iDrawMode;
	int m_iScroll;
	BOOL m_bMakingTems;
	BOOL m_bMakingEnforce;
	HANDLE m_hStopTems;
	HANDLE m_hStopEnforce;
	CString m_sSetupPath;
	CString m_sITSensPath;

	WAVEFORMATEX m_wavFormat[2];
	BYTE* m_pAudioData[2];
	int m_iAudioDataSize[2];
	BOOL m_bAudioStop;			//오디오 중지 
	BOOL m_bAudioPlaying;		//오디오 플레이 중?

	CStatic m_pic2;
	CListBox m_List;
   HANDLE m_hSerial;
   
   	CComboBox m_cmbAudioType;
	afx_msg void OnBnClickedButtonAudioRefresh();
	void GetAudioOutputDevice();
	BOOL LoadWavFile(CString sFileName,int iOrder);
	afx_msg void OnBnClickedButtonPlayAudio1();
	afx_msg void OnBnClickedButtonPlayAudio2();
	afx_msg void OnBnClickedButtonPlayAudio3();
	CComboBox m_cmbAudioType2;
	afx_msg void OnCbnSelchangeComboAudioType();
	afx_msg void OnCbnSelchangeComboAudioType2();
	afx_msg void OnBnClickedButtonLoadAudio();
	afx_msg void OnBnClickedButtonLoadAudio2();
	afx_msg void OnBnClickedButtonMakeEnforce();
	CEdit m_edCode;
	CComboBox m_cmbEnforceBmt;
	afx_msg void OnBnClickedButtonIcon();

	CImageList m_pImageList;
	int m_iPixelSize;
	afx_msg void OnBnClickedButtonIconTransparent();
	afx_msg void OnBnClickedButtonValidFormat();
	bool ReadWavFormatFromFile(const CString& filePath, WAVEFORMATEX& outWf);

};
