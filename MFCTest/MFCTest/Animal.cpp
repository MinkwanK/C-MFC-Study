#include "pch.h"
#include "Animal.h"

int CAnimal::m_iAnimalCnt = 0;
CAnimal::CAnimal()
{
	Introduce();
	++m_iAnimalCnt;
}

CAnimal::~CAnimal()
{
}
