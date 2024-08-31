#include "pch.h"
#include "Dog.h"

CDog::CDog()
{
	Introduce();
}

CDog::~CDog()
{
}

void CDog::Introduce()
{
	m_sLine = _T("I am Dog");
}
