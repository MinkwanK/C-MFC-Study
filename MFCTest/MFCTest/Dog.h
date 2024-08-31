#pragma once
#include "Animal.h"
class CDog :
    public CAnimal
{
public:
    CDog();
    ~CDog();
    void Introduce() override;
    
};

