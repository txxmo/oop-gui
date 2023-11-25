#pragma once
#include "Entity.h"
class TabListBox : public Container
{
protected:
    int* Selected;
    std::wstring English;
    std::wstring Mandarin;
public:
    TabListBox(std::wstring name,std::wstring mandarin);
    int Index;
    void Update();
    void Draw();
};


