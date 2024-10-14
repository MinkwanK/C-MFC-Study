#pragma once

enum  SEX
{
	MALE,
	WOMAN,
};

class CAnimal
{
public:
	CAnimal();
	~CAnimal();

	static int GetAnimalCnt() { return m_iAnimalCnt; }

	virtual void Introduce() { m_sSay = _T("I am Animal"); }
	virtual void Speak() { AfxMessageBox(_T("Hello")); }

public:
	static int m_iAnimalCnt;
	int m_iHeight;
	int m_iWeight;
	CString m_sSay;
	SEX m_eSex;
};

