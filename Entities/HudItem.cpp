#include "pch.h"
#include "HudItem.h"
#include "Input.h"
#include "Gui.h"
#include "Drawing.h"
#include "Font.h"

HudItem::HudItem(int* x, int* y,bool* enable, std::wstring name, std::vector<std::wstring>* items = nullptr)
{
	HudItem::Size = { 0,0 };
	HudItem::SetVisible(true);
	HudItem::Name = name;
	HudItem::Items = items;
	X = x;
	Y = y;
	Enable = enable;
	HudItem::Dragging = false;
	HudItem::Drag = { 0,0 };
	HudItem::Header = {  GetTextWidth(name,12,"Verdana") + 20,GetTextHeight(name,12,"Verdana") + 10};
	HudItem::LongestString = GetTextWidth(name, 12, "Verdana");
}
void HudItem::DragAction()
{
    if (HudItem::Dragging && !IsKeyDown(VK_LBUTTON))
		HudItem::Dragging = false;

    if (HudItem::Dragging)
    {

        *X = MousePos.x - HudItem::Drag.x;
       *Y = MousePos.y - HudItem::Drag.y;
    }
    if (IsMouseInRectangle(*X,*Y, HudItem::Size.x, HudItem::Header.y))
    {

        if (IsKeyClicked(VK_LBUTTON)) // This prevents a user holding down and hovering over the title bar to drag it. You need to actually click down.
			HudItem::Dragging = true;

		HudItem::Drag.x = MousePos.x - *X;
		HudItem::Drag.y = MousePos.y - *Y;
    }
}
void HudItem::Update()
{
	if (!HudItem::Parent)
		HudItem::SetVisible(false);

	if (!HudItem::IsVisible())
		return;
	if(!*Enable)
		return;
	HudItem::DragAction();
//	HudItem::Header.x = LongestString + 20;
	HudItem::Size.x = LongestString + 20;
	HudItem::Size.y = 20;

	
}

void HudItem::Draw()
{
	if (!HudItem::IsVisible())
		return;
	if (!*Enable)
		return;
	if (HudItem::Items == nullptr)
		return;
	// update later
	if (IsMouseInRectangle(*X, *Y, HudItem::Header.x, HudItem::Header.y) && IsKeyClicked(VK_LBUTTON))
	{
		HudItem::SetLastClick(clock() * 0.00001f); // we multiple it for accuracy purposes to turn it into a float
	}

	FilledRectangle(*X, *Y,  HudItem::LongestString + 20, HudItem::Size.y + 3, Colour(50, 50, 50, 200));
	DrawText(*X + (HudItem::LongestString / 2) +10, *Y + 5, HudItem::Name, LIT("Verdana"), 12, Colour(255, 255, 255, 255), Centre);

	int i = 0;
	int highestwidth = 0;
	for (std::wstring str : *HudItem::Items)
	{
		int width = GetTextWidth(str, 12, "Verdana");
		if (width > highestwidth)
			highestwidth = width;

		if (i % 2 == 0)
		{
			FilledRectangle(*X, *Y + (i* HudItem::Size.y) + HudItem::Header.y, HudItem::LongestString + 20, HudItem::Size.y, Colour(30, 30, 30, 200));
		}
		else
			FilledRectangle(*X, *Y + (i * HudItem::Size.y) + HudItem::Header.y, HudItem::LongestString + 20, HudItem::Size.y, Colour(50, 50, 50, 200));
		DrawText(*X + (HudItem::LongestString / 2) + 10, *Y +(i * HudItem::Size.y) + HudItem::Header.y, str, LIT("Verdana"), 12, Colour(255, 255, 255, 255), Centre);
		i++;
	}
	if(HudItem::Header.x > highestwidth)
		highestwidth = HudItem::Header.x;
	HudItem::LongestString = highestwidth + 20;
	FilledLine(*X -1, *Y, *X+ HudItem::LongestString + 20 + 1, *Y, 1, Colour(100, 185, 255));
	FilledLine(*X - 1, *Y + HudItem::Header.y, *X + HudItem::LongestString + 20 + 1, *Y + HudItem::Header.y, 1, Colour(100, 185, 255));
	FilledLine(*X, *Y, *X, *Y + (HudItem::Items->size() * HudItem::Size.y) + HudItem::Header.y, 1, Colour(100, 185, 255));
	FilledLine(*X + HudItem::LongestString + 20, *Y, *X + HudItem::LongestString + 20, *Y + (HudItem::Items->size() * HudItem::Size.y) + HudItem::Header.y, 1, Colour(100, 185, 255));
	FilledLine(*X -1, *Y + (HudItem::Items->size() * HudItem::Size.y) + HudItem::Header.y, *X + HudItem::LongestString + 20 + 1, *Y + (i * HudItem::Size.y) + HudItem::Header.y, 1, Colour(100, 185, 255));
}