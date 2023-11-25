#include "Pch.h"
#include "TabListBox.h"
#include "GUI.h"
#include "Input.h"
#include "Drawing.h"
#include "Font.h"
#include "Graphics.h"

TabListBox::TabListBox(std::wstring name,std::wstring mandarin)
{
	TabListBox::Mandarin = mandarin;
	TabListBox::English = name;

	if (*Language == 0)
		TabListBox::Name = English;
	else if (*Language == 1)
		TabListBox::Name = Mandarin;
	TabListBox::Index = TabCount;
	TabCount++;
}

void TabListBox::Update()
{
	TabListBox::ParentPos = TabListBox::Parent->GetParentPos();
	TabListBox::Pos = TabListBox::Parent->GetParentPos();
	Container::Update();
}
void TabListBox::Draw()
{
	Container::Draw();
}