#include "pch.h"
#include "Input.h"
#include "Drawing.h"
#include "DropDown.h"
#include "Font.h"
#include "Graphics.h"
DropDown::DropDown(float x, float y, std::wstring text,std::wstring mandarin, int* index, std::vector<std::wstring>names,std::vector<std::wstring> mandarinnames, int maxitems)
{
	DropDown::English = text;
	DropDown::Mandarin = mandarin;
	if (*Language == 0)
		DropDown::Name = English;
	else if (*Language == 1)
		DropDown::Name = Mandarin;

	DropDown::EnglishNames = names;
	DropDown::MandarinNames = mandarinnames;
	if (*Language == 0)
		DropDown::Names = EnglishNames;
	else if (*Language == 1)
		DropDown::Names = MandarinNames;

	DropDown::MaxVisibleItems = maxitems;
	DropDown::Pos = { x,y };
	DropDown::Index = index;
	DropDown::Size = { 160,20 };
	DropDown::CalculateBuffer();
	DropDown::ConvertSelectedName();
	if (DropDown::MaxVisibleItems > DropDown::Names.size())
	{
		DropDown::MaxVisibleItems = DropDown::Names.size();
	}
	DropDown::PointerStart = 0;
	DropDown::PointerEnd = DropDown::MaxVisibleItems;
}
void DropDown::CalculateBuffer()
{
	if (!DropDown::Active)
		DropDown::CutOffBuffer = 15;
	else
		DropDown::CutOffBuffer = 0;
}
void DropDown::SetDropDownWidth()
{
	float width = 0;
	for (std::wstring str : DropDown::Names)
	{
		float wdth = GetTextWidth(str, 11, LIT("Verdana"));
		if (wdth > width)
			width = wdth;
	}
	DropDown::DropWidth = width;

}
void DropDown::ConvertSelectedName()
{
	auto it = DropDown::Names.begin();
	std::advance(it, *Index);
	float originalwidth = GetTextWidth(*it, 11, LIT("Verdana"));

	if (originalwidth < DropDown::Size.x - DropDown::CutOffBuffer)
	{
		DropDown::SelectedName = *it;
		TextWidth = originalwidth;
		return;
	}
	else
	{
		std::wstring str = *it;
		for (int i = str.length(); i > 0; i--)
		{
			str.erase(std::prev((str).end()));
			float width = GetTextWidth(str + L"..", 11, LIT("Verdana"));
			if (width < DropDown::Size.x - DropDown::CutOffBuffer)
			{
				DropDown::SelectedName = str + L"..";
				TextWidth = width;
				return;
			}
		}
		DropDown::SelectedName = str + L"..";
		TextWidth = GetTextWidth(str + L"..", 11, LIT("Verdana"));
	}

}
void DropDown::ArrowNavigation()
{
	if (!DropDown::Active)
		return;
	if (IsKeyClicked(VK_DOWN) && DropDown::LastClick < (clock() * 0.00001f))
	{
		if (DropDown::Names.size()-1> DropDown::PointerEnd)
		{
			DropDown::PointerEnd++;
			DropDown::PointerStart++;
			DropDown::LastClick = (clock() * 0.00001f) + 0.002f;
		}
	}
	if (IsKeyClicked(VK_UP) && DropDown::LastClick < (clock() * 0.00001f))
	{
		if (DropDown::PointerStart > 0)
		{
			DropDown::PointerEnd--;
			DropDown::PointerStart--;
			DropDown::LastClick = (clock() * 0.00001f) + 0.002f;
		}
	}
}
Color DropDown::InterpolateColour(const Color& colour1, const Color& colour2, float progress)
{
	float r = colour1.R + (colour2.R - colour1.R) * progress;
	float g = colour1.G + (colour2.G - colour1.G) * progress;
	float b = colour1.B + (colour2.B - colour1.B) * progress;

	return Colour(static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b), static_cast<unsigned char>(255));
}
std::wstring DropDown::GetSelectedName()
{
	return DropDown::SelectedName;
}

void DropDown::Update()
{
	if (!DropDown::Parent)
		DropDown::SetVisible(false);
	if (!DropDown::IsVisible())
		return;
	DropDown::ArrowNavigation();
	DropDown::ParentPos = DropDown::Parent->GetParentPos();
	DropDown::CalculateBuffer();
	DropDown::UpdateScrollBar();
	auto currenttime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> deltatime = currenttime - DropDown::LastUpdateTime;
	DropDown::LastUpdateTime = currenttime;
	double deltaseconds = deltatime.count();
	if (!DropDown::Blocked)
	{
		if (IsMouseInRectangle(DropDown::Pos + ParentPos, DropDown::Size) && IsKeyClicked(VK_LBUTTON) && DropDown::LastClick < (clock() * 0.00001f))
		{
			if (!DropDown::Active)
			{
				DropDown::Active = true;
				DropDown::SetBlockedSiblings(true);
				DropDown::LastClick = (clock() * 0.00001f) + 0.002f;
				DropDown::CalculateBuffer();
				DropDown::ConvertSelectedName();
				int i = 0;
				for (const std::wstring& name : DropDown::Names)
				{
					std::wstring culledname = L"";
					float width = GetTextWidth(name, 11, LIT("Verdana"));
					if (width < DropDown::Size.x + 2)
					{
						CulledNames.push_back(name);

					}
					else
					{
						culledname = name;
						for (int i = culledname.length(); i > 0; i--)
						{
							culledname.erase(std::prev((culledname).end()));
							float width = GetTextWidth(culledname + L"..", 11, LIT("Verdana"));
							if (width < DropDown::Size.x + 2)
							{
								CulledNames.push_back(culledname + L"..");
								break;
							}
						}

					}
				}
			}
			else
			{
				DropDown::Active = false;
				DropDown::SetBlockedSiblings(false);
				DropDown::CalculateBuffer();
				DropDown::ConvertSelectedName();
				DropDown::LastClick = (clock() * 0.00001f) + 0.002f;
			}
		}
	}
	// not in box, not in element, not in scroll bar
	if ( IsKeyClicked(VK_LBUTTON) && DropDown::Active && !(IsMouseInRectangle(DropDown::Pos + ParentPos, DropDown::Size) || IsMouseInRectangle(DropDown::ParentPos.x + DropDown::Pos.x, DropDown::ParentPos.y + DropDown::Pos.y + DropDown::Size.y +5, DropDown::Size.x, DropDown::Names.size() * DropDown::Size.y) || IsMouseInRectangle(DropDown::ParentPos.x + DropDown::Pos.x + DropDown::Size.x, DropDown::ParentPos.y + DropDown::Pos.y + DropDown::Size.y + 4, 6, (DropDown::PointerEnd - DropDown::PointerStart) * DropDown::Size.y) && DropDown::Names.size() > DropDown::MaxVisibleItems))
	{
		DropDown::Active = false;
		DropDown::SetBlockedSiblings(false);
		DropDown::CalculateBuffer();
		DropDown::ConvertSelectedName();
	}
	if ((IsKeyClicked(VK_RETURN) || IsKeyClicked(VK_ESCAPE)) && DropDown::Active)
	{
		DropDown::Active = false;
		DropDown::SetBlockedSiblings(false);
		DropDown::CalculateBuffer();
		DropDown::ConvertSelectedName();
	}
	if (DropDown::Active)
	{
		DropDown::SizeDifference = DropDown::Size.x - DropDown::TextWidth;
		DropDown::SetDropDownWidth();
		int i = 0;
		for (const std::wstring& name : DropDown::Names)
		{
			if (i < DropDown::PointerStart)
			{
				i++;
				continue;
			}
			if (i > DropDown::PointerEnd-1)
			{
				i++;
				continue;
			}
			float itemposy = DropDown::ParentPos.y + DropDown::Pos.y + DropDown::Size.y + 5 + ((i - DropDown::PointerStart) * DropDown::Size.y);
			
			if (IsMouseInRectangle(DropDown::ParentPos.x + DropDown::Pos.x , itemposy, DropDown::Size.x, DropDown::Size.y) && IsKeyClicked(VK_LBUTTON))
			{
				*DropDown::Index = i;
				DropDown::Active = false;
				DropDown::SetBlockedSiblings(false);
				DropDown::CalculateBuffer();
				DropDown::ConvertSelectedName();
				DropDown::ValueChangeEvent();
				Sleep(50); // bandage fix to stop click through
			}
			i++;
		}
	}
	bool mouseover = IsMouseInRectangle(
		DropDown::ParentPos.x + DropDown::Pos.x, DropDown::ParentPos.y + DropDown::Pos.y, DropDown::Size.x, DropDown::Size.y) && !DropDown::Blocked
		|| (DropDown::Active && 
		IsMouseInRectangle(DropDown::ParentPos.x + DropDown::Pos.x - (DropDown::DropWidth / 4), DropDown::ParentPos.y + DropDown::Pos.y + DropDown::Size.y + 5, DropDown::Size.x, (DropDown::PointerEnd - DropDown::PointerStart) * DropDown::Size.y)
			&& !DropDown::Blocked);

	// Update the hover animation progress
	if (mouseover && !IsHovered)
	{
		// Mouse just started hovering, reset animation progress
		IsHovered = true;
		HoverAnimationProgress = 0.0;
	}
	else if (!mouseover && IsHovered)
	{
		// Mouse stopped hovering, reset animation progress
		IsHovered = false;
		HoverAnimationProgress = 1.0;
	}

	// Update the animation progress
	if (IsHovered)
	{
		// Increase the progress when hovering
		HoverAnimationProgress += (1.0 / HoverAnimationDuration) * deltaseconds;
		HoverAnimationProgress = std::min(HoverAnimationProgress, 1.0f);
	}
	else
	{
		// Decrease the progress when not hovering
		HoverAnimationProgress -= (1.0 / HoverAnimationDuration) * deltaseconds;
		HoverAnimationProgress = std::max(HoverAnimationProgress, 0.0f);
	}
	
		// Update the current color based on the animation progress
		CurrentColour = DropDown::InterpolateColour(
			Colour(255, 255, 255, 255), // The color when not hovering 
			Colour(100, 185, 255, 255), // The color when hovering 
			HoverAnimationProgress);
	
	
}
void DropDown::UpdateScrollBar()
{
	if (!IsKeyDown(VK_LBUTTON))
		DropDown::ScrollBar = false;
	if (IsMouseInRectangle(DropDown::ParentPos.x + DropDown::Pos.x + DropDown::Size.x, DropDown::ParentPos.y + DropDown::Pos.y + DropDown::Size.y + 4, 6, (DropDown::PointerEnd - DropDown::PointerStart) * DropDown::Size.y) && IsKeyClicked(VK_LBUTTON))
		DropDown::ScrollBar = true;
	if (DropDown::ScrollBar)
	{
		float ratio = (MousePos.y - (float)(DropDown::ParentPos.y + DropDown::Pos.y + DropDown::Size.y + 4)) / (float)((DropDown::MaxVisibleItems - 1) * DropDown::Size.y);
		ratio = std::clamp(ratio, 0.0f, 1.0f);
		DropDown::PointerEnd = (int)(DropDown::MaxVisibleItems + (DropDown::Names.size() - DropDown::MaxVisibleItems) * ratio);
	
	}
	DropDown::PointerStart = DropDown::PointerEnd - DropDown::MaxVisibleItems;
}

void DropDown::Draw()
{
	if (!DropDown::Parent)
		DropDown::SetVisible(false);
	if (!DropDown::IsVisible())
		return;
	DrawText(DropDown::ParentPos.x + DropDown::Pos.x, (DropDown::ParentPos.y + DropDown::Pos.y) - 17, DropDown::Name + L": ", LIT("Verdana"), 11, Colour(255, 255, 255, 255), None);
	OutlineRectangle(DropDown::ParentPos.x + DropDown::Pos.x - 1, DropDown::ParentPos.y + DropDown::Pos.y - 1, DropDown::Size.x + 1, DropDown::Size.y + 1, 1, Colour(130, 130, 130, 255));
	FilledRectangle(DropDown::ParentPos.x + DropDown::Pos.x, DropDown::ParentPos.y +DropDown::Pos.y, DropDown::Size.x, DropDown::Size.y, Colour(80, 80, 80, 255));
	float trianglex1 = DropDown::ParentPos.x + DropDown::Pos.x + DropDown::Size.x - 12;
	float triangley1 = DropDown::ParentPos.y + DropDown::Pos.y +3;
	float trianglex2 = DropDown::ParentPos.x + DropDown::Pos.x + DropDown::Size.x - 3;
	float triangley2 = DropDown::Pos.y + DropDown::ParentPos.y + 3;
	float trianglex3 = DropDown::ParentPos.x + DropDown::Pos.x + DropDown::Size.x - 7;
	float triangley3 = DropDown::ParentPos.y + DropDown::Pos.y + DropDown::Size.y - 3;
	if(!DropDown::Active)
	FilledTriangle(trianglex1, triangley1, trianglex2, triangley2, trianglex3, triangley3, Colour(0, 185, 255, 255));
	DrawText(DropDown::ParentPos.x + DropDown::Pos.x + 5, DropDown::ParentPos.y + DropDown::Pos.y + (DropDown::Size.y /8),DropDown::SelectedName, LIT("Verdana"), 11, DropDown::CurrentColour, None);

	if (DropDown::DropWidth < DropDown::Size.x)
	{
		DropDown::DropWidth = DropDown::Size.x;
		DropDown::SizeDifference = 0;
	}
	if (DropDown::Active)
	{
		OutlineRectangle(DropDown::ParentPos.x + DropDown::Pos.x , DropDown::ParentPos.y + DropDown::Pos.y + DropDown::Size.y +5, DropDown::Size.x, (DropDown::PointerEnd - DropDown::PointerStart) * DropDown::Size.y, 1, Colour(130, 130, 130, 255));

		int i = 0;
		for (const std::wstring& name : DropDown::Names)
		{
			if (i < DropDown::PointerStart)
			{
				i++;
				continue;
			}
			if (i > DropDown::PointerEnd-1)
			{
				i++;
				continue;
			}
		
			float itemposy = DropDown::ParentPos.y + DropDown::Pos.y + DropDown::Size.y + 5 + ((i- DropDown::PointerStart) * DropDown::Size.y);
			if (i % 2 == 0)
				FilledRectangle(DropDown::ParentPos.x + DropDown::Pos.x, itemposy, DropDown::Size.x, DropDown::Size.y, Colour(30, 30, 30, 255));
			else
				FilledRectangle(DropDown::ParentPos.x + DropDown::Pos.x , itemposy, DropDown::Size.x, DropDown::Size.y, Colour(50, 50, 50, 255));

			if (IsMouseInRectangle(DropDown::ParentPos.x + DropDown::Pos.x, itemposy, DropDown::Size.x, DropDown::Size.y))
			{
			
				float width = GetTextWidth(name, 11, LIT("Verdana"));
				if (DropDown::CulledNames[i] != DropDown::Names[i])
				{
					FilledRectangle(DropDown::ParentPos.x + DropDown::Pos.x, itemposy, width + 5, DropDown::Size.y, Colour(150, 150, 150, 120));
					DrawText(DropDown::ParentPos.x + DropDown::Pos.x + 5, itemposy + (DropDown::Size.y / 8),name, LIT("Verdana"), 11, Colour(100, 185, 255, 255), None);
				}
				else if (i == *DropDown::Index)
				{
					FilledRectangle(DropDown::ParentPos.x + DropDown::Pos.x, itemposy, DropDown::Size.x, DropDown::Size.y, Colour(150, 150, 150, 200));
					DrawText(DropDown::ParentPos.x + DropDown::Pos.x + 5, itemposy + (DropDown::Size.y / 8), DropDown::CulledNames[i], LIT("Verdana"), 11, Colour(100, 185, 255, 255), None);
				}
				else
				{
					FilledRectangle(DropDown::ParentPos.x + DropDown::Pos.x, itemposy, DropDown::Size.x, DropDown::Size.y, Colour(150, 150, 150, 120));
					DrawText(DropDown::ParentPos.x + DropDown::Pos.x + 5, itemposy + (DropDown::Size.y / 8), DropDown::CulledNames[i], LIT("Verdana"), 11, Colour(240, 240, 240, 255), None);
					
				}
				
			}
			else
			{
				if (i == *DropDown::Index)
				{

					DrawText(DropDown::ParentPos.x + DropDown::Pos.x + 5, itemposy + (DropDown::Size.y / 8), DropDown::CulledNames[i], LIT("Verdana"), 11, Colour(100, 185, 255, 255), None);
				}
				else
				{
					DrawText(DropDown::ParentPos.x + DropDown::Pos.x + 5, itemposy + (DropDown::Size.y / 8), DropDown::CulledNames[i], LIT("Verdana"), 11, Colour(240, 240, 240, 255), None);
				}
			}
			i++;
		}
		if (DropDown::Names.size() > DropDown::MaxVisibleItems)
		{
			OutlineRectangle(DropDown::ParentPos.x + DropDown::Pos.x + DropDown::Size.x, DropDown::ParentPos.y + DropDown::Pos.y + DropDown::Size.y + 4, 6, (DropDown::PointerEnd - DropDown::PointerStart) * DropDown::Size.y + 1, 1, Colour(130, 130, 130, 255));
			int unselectedelements = Names.size() - MaxVisibleItems;
			float unselectedclamp = std::clamp(unselectedelements, 1, (int)Names.size());
			float scrollheight = ((DropDown::PointerEnd - DropDown::PointerStart) * DropDown::Size.y) / (unselectedclamp);
			float scrolly = DropDown::ParentPos.y + DropDown::Pos.y + DropDown::Size.y + 5 + (((PointerEnd - MaxVisibleItems) * Size.y));
			float scrollyclamp = std::clamp(scrolly, DropDown::ParentPos.y + DropDown::Pos.y + DropDown::Size.y + 5, DropDown::ParentPos.y + DropDown::Pos.y + DropDown::Size.y + 5 + ((DropDown::PointerEnd - DropDown::PointerStart) * DropDown::Size.y) - scrollheight);


			FilledRectangle(DropDown::ParentPos.x + DropDown::Pos.x + DropDown::Size.x, scrollyclamp, 6, scrollheight, Colour(100, 185, 255, 255));
		}
	}
}