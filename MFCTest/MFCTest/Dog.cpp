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
	m_sSay = _T("I am Dog");
}
