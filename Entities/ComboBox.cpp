#include "pch.h"
#include "Input.h"
#include "Drawing.h"
#include "ComboBox.h"
#include "Font.h"
ComboBox::ComboBox(float x, float y, std::wstring text, std::vector<bool*>items, std::vector<std::wstring>names)
{
	ComboBox::Pos = { x,y };
	ComboBox::Name = text;
	ComboBox::Items = items;
	ComboBox::Names = names;
	ComboBox::Size = { 160,20 };
	ComboBox::CalculateBuffer();
	ComboBox::ConvertSelectedName();

	if (ComboBox::MaxVisibleItems > items.size())
	{
		ComboBox::MaxVisibleItems = items.size();
	}
	ComboBox::PointerStart = 0;
	ComboBox::PointerEnd = ComboBox::MaxVisibleItems;
}
void ComboBox::CalculateBuffer()
{
	if (!ComboBox::Active)
		ComboBox::CutOffBuffer = 15;
	else
		ComboBox::CutOffBuffer = 0;
}
void ComboBox::SetComboBoxWidth()
{
	float width = 0;
	for (std::wstring str : ComboBox::Names)
	{
		float wdth = GetTextWidth(str, 11, LIT("Verdana"));
		if (wdth > width)
			width = wdth;
	}
	ComboBox::DropWidth = width;
}
void ComboBox::ConvertSelectedName()
{

	std::wstring combinedstr = L"";
	for (int i = 0; i < ComboBox::Names.size(); i++)
	{
		auto it = ComboBox::Names.begin();
		std::advance(it, i);
		std::advance(std::begin(ComboBox::Items), i);
		auto itbool = ComboBox::Items.begin();
		std::advance(itbool, i);

		if (**itbool == true)
			combinedstr += *it + L",";
	}
	if (combinedstr.length() == 0)
	{
		combinedstr = L"Unselected";
		float originalwidth = GetTextWidth(combinedstr, 11, LIT("Verdana"));
		if (originalwidth < ComboBox::Size.x - ComboBox::CutOffBuffer)
		{
			ComboBox::TextWidth = originalwidth;
			ComboBox::SelectedName = combinedstr;
			return;
		}
		else
		{
			for (int i = combinedstr.length(); i > 0; i--)
			{
				combinedstr.erase(std::prev((combinedstr).end()));
				float width = GetTextWidth(combinedstr + L"..", 11, LIT("Verdana"));
				if (width < ComboBox::Size.x - ComboBox::CutOffBuffer)
				{
					ComboBox::SelectedName = combinedstr + L"..";
					ComboBox::TextWidth = width;
					return;
				}
			}
		}
		
	}

	combinedstr.erase(std::prev((combinedstr).end())); // last character will be "," and we dont need that
	float originalwidth = GetTextWidth(combinedstr, 11, LIT("Verdana"));

	if (originalwidth < ComboBox::Size.x - ComboBox::CutOffBuffer)
	{
		ComboBox::SelectedName = combinedstr;
		ComboBox::TextWidth = originalwidth;
		return;
	}
	else
	{
		std::wstring str = combinedstr;
		for (int i = str.length(); i > 0; i--)
		{
			str.erase(std::prev((str).end()));
			float width = GetTextWidth(str + L"..", 11, LIT("Verdana"));
			if (width < ComboBox::Size.x - ComboBox::CutOffBuffer)
			{
				ComboBox::SelectedName = str + L"..";
				ComboBox::TextWidth = width;
				return;
			}
		}
		ComboBox::SelectedName = str + L"..";
		ComboBox::TextWidth = GetTextWidth(str + L"..", 11, LIT("Verdana"));
	}

}
void ComboBox::ArrowNavigation()
{
	if (!ComboBox::Active)
		return;
	if (IsKeyClicked(VK_DOWN) && ComboBox::LastClick < (clock() * 0.00001f))
	{
		if (ComboBox::Names.size() - 1 > ComboBox::PointerEnd)
		{
			ComboBox::PointerEnd++;
			ComboBox::PointerStart++;
			ComboBox::LastClick = (clock() * 0.00001f) + 0.002f;
		}
	}
	if (IsKeyClicked(VK_UP) && ComboBox::LastClick < (clock() * 0.00001f))
	{
		if (ComboBox::PointerStart > 0)
		{
			ComboBox::PointerEnd--;
			ComboBox::PointerStart--;
			ComboBox::LastClick = (clock() * 0.00001f) + 0.002f;
		}
	}
}
Color ComboBox::InterpolateColour(const Color& colour1, const Color& colour2, float progress)
{
	float r = colour1.R + (colour2.R - colour1.R) * progress;
	float g = colour1.G + (colour2.G - colour1.G) * progress;
	float b = colour1.B + (colour2.B - colour1.B) * progress;

	return Colour(static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b), static_cast<unsigned char>(255));
}
void ComboBox::Update()
{
	if (!ComboBox::Parent)
		ComboBox::SetVisible(false);
	if (!ComboBox::IsVisible())
		return;
	ComboBox::ArrowNavigation();
	ComboBox::ParentPos = ComboBox::Parent->GetParentPos();
	ComboBox::CalculateBuffer();
	ComboBox::UpdateScrollBar();
	auto currenttime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> deltatime = currenttime - ComboBox::LastUpdateTime;
	ComboBox::LastUpdateTime = currenttime;
	double deltaseconds = deltatime.count();
	if (!ComboBox::Blocked)
	{
		if (IsMouseInRectangle(ComboBox::Pos + ParentPos, ComboBox::Size) && IsKeyClicked(VK_LBUTTON) && ComboBox::LastClick < (clock() * 0.00001f))
		{
			if (!ComboBox::Active)
			{
				ComboBox::Active = true;
				ComboBox::SetBlockedSiblings(true);
				ComboBox::LastClick = (clock() * 0.00001f) + 0.002f;
				ComboBox::CalculateBuffer();
				ComboBox::ConvertSelectedName();

				int i = 0;
				for (const std::wstring& name : ComboBox::Names)
				{
					std::wstring culledname = L"";
					float width = GetTextWidth(name, 11, LIT("Verdana"));
					if (width < ComboBox::Size.x + 2)
					{
						ComboBox::CulledNames.push_back(name);

					}
					else
					{
						culledname = name;
						for (int i = culledname.length(); i > 0; i--)
						{
							culledname.erase(std::prev((culledname).end()));
							float width = GetTextWidth(culledname + L"..", 11, LIT("Verdana"));
							if (width < ComboBox::Size.x + 2)
							{
								ComboBox::CulledNames.push_back(culledname + L"..");
								break;
							}
						}

					}
				}
			}
			else
			{
				ComboBox::Active = false;
				ComboBox::SetBlockedSiblings(false);
				ComboBox::CalculateBuffer();
				ComboBox::ConvertSelectedName();
				ComboBox::LastClick = (clock() * 0.00001f) + 0.002f;
			}
		}
	}
	if (IsKeyClicked(VK_LBUTTON) && ComboBox::Active && !(IsMouseInRectangle(ComboBox::Pos + ParentPos, ComboBox::Size) || IsMouseInRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x , ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y + 5, ComboBox::Size.x, ComboBox::Names.size() * ComboBox::Size.y) || (IsMouseInRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x + ComboBox::Size.x, ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y + 4, 6, (ComboBox::PointerEnd - ComboBox::PointerStart) * ComboBox::Size.y)&& ComboBox::Names.size() > ComboBox::MaxVisibleItems)))
	{
		ComboBox::Active = false;
		ComboBox::SetBlockedSiblings(false);
		ComboBox::CalculateBuffer();
		ComboBox::ConvertSelectedName();
	}
	if ((IsKeyClicked(VK_RETURN) || IsKeyClicked(VK_ESCAPE)) && ComboBox::Active)
	{
		ComboBox::Active = false;
		ComboBox::SetBlockedSiblings(false);
		ComboBox::CalculateBuffer();
		ComboBox::ConvertSelectedName();
	}
	if (ComboBox::Active)
	{
		ComboBox::SizeDifference = ComboBox::DropWidth - ComboBox::TextWidth;
		ComboBox::SetComboBoxWidth();
		int i = 0;
		for (const std::wstring& name : ComboBox::Names)
		{
			if (i < ComboBox::PointerStart)
			{
				i++;
				continue;
			}
			if (i > ComboBox::PointerEnd-1)
			{
				i++;
				continue;
			}
			float itemposy = ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y + 5 + ((i - ComboBox::PointerStart) * ComboBox::Size.y);

			if (IsMouseInRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x, itemposy, ComboBox::ParentPos.x + ComboBox::Pos.x + ComboBox::Size.x, ComboBox::Size.y) && IsKeyClicked(VK_LBUTTON) && ComboBox::LastClick < (clock() * 0.00001f))
			{
				auto it = ComboBox::Items.begin();
				std::advance(it, i);
				ComboBox::ValueChangeEvent();
				**it = !**it;
				ComboBox::LastClick = (clock() * 0.00001f) + 0.002f;
				ComboBox::ConvertSelectedName();
				Sleep(50); // bandage click through fix
			}
			i++;
		}
	}
	bool mouseover = IsMouseInRectangle(
		ComboBox::ParentPos.x + ComboBox::Pos.x, ComboBox::ParentPos.y + ComboBox::Pos.y, ComboBox::Size.x, ComboBox::Size.y) && !ComboBox::Blocked
		|| (ComboBox::Active &&
			IsMouseInRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x, ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y + 5, ComboBox::Size.x, (ComboBox::PointerEnd - ComboBox::PointerStart) * ComboBox::Size.y)
			&& !ComboBox::Blocked);

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
		ComboBox::HoverAnimationProgress += (1.0 / ComboBox::HoverAnimationDuration) * deltaseconds;
		ComboBox::HoverAnimationProgress = std::min(ComboBox::HoverAnimationProgress, 1.0f);
	}
	else
	{
		// Decrease the progress when not hovering
		ComboBox::HoverAnimationProgress -= (1.0 / ComboBox::HoverAnimationDuration) * deltaseconds;
		ComboBox::HoverAnimationProgress = std::max(ComboBox::HoverAnimationProgress, 0.0f);
	}

	// Update the current color based on the animation progress
	CurrentColour = ComboBox::InterpolateColour(
		Colour(255, 255, 255, 255), // The color when not hovering 
		Colour(100, 185, 255, 255), // The color when hovering 
		ComboBox::HoverAnimationProgress);
}
void ComboBox::UpdateScrollBar()
{
	if (!IsKeyDown(VK_LBUTTON))
		ComboBox::ScrollBarHeld = false;
	if (IsMouseInRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x, ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y + 4, 6, (ComboBox::PointerEnd - ComboBox::PointerStart) * ComboBox::Size.y) && IsKeyClicked(VK_LBUTTON))
		ComboBox::ScrollBarHeld = true;
	if (ComboBox::ScrollBarHeld)
	{
		float ratio = (MousePos.y - (float)(ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y + 4)) / (float)((ComboBox::MaxVisibleItems - 1) * ComboBox::Size.y);
		ratio = std::clamp(ratio, 0.0f, 1.0f);
		ComboBox::PointerEnd = (int)(ComboBox::MaxVisibleItems + (ComboBox::Names.size() - ComboBox::MaxVisibleItems) * ratio);

	}
	ComboBox::PointerStart = ComboBox::PointerEnd - ComboBox::MaxVisibleItems;
}

void ComboBox::Draw()
{
	if (!ComboBox::Parent)
		ComboBox::SetVisible(false);
	if (!ComboBox::IsVisible())
		return;
	DrawText(ComboBox::ParentPos.x + ComboBox::Pos.x, (ComboBox::ParentPos.y + ComboBox::Pos.y) - 17, ComboBox::Name + L": ", LIT("Verdana"), 11, Colour(255, 255, 255, 255), None);

	OutlineRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x - 1, ComboBox::ParentPos.y + ComboBox::Pos.y - 1, ComboBox::Size.x + 1, ComboBox::Size.y + 1, 1, Colour(130, 130, 130, 255));
	FilledRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x, ComboBox::ParentPos.y + ComboBox::Pos.y, ComboBox::Size.x, ComboBox::Size.y, Colour(80, 80, 80, 255));
	float trianglex1 = ComboBox::ParentPos.x + ComboBox::Pos.x + ComboBox::Size.x - 12;
	float triangley1 = ComboBox::ParentPos.y + ComboBox::Pos.y + 3;
	float trianglex2 = ComboBox::ParentPos.x + ComboBox::Pos.x + ComboBox::Size.x - 3;
	float triangley2 = ComboBox::Pos.y + ComboBox::ParentPos.y + 3;
	float trianglex3 = ComboBox::ParentPos.x + ComboBox::Pos.x + ComboBox::Size.x - 7;
	float triangley3 = ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y - 3;
	if (!ComboBox::Active)
		FilledTriangle(trianglex1, triangley1, trianglex2, triangley2, trianglex3, triangley3, Colour(0, 180, 255, 255));
	DrawText(ComboBox::ParentPos.x + ComboBox::Pos.x + 5, ComboBox::ParentPos.y + ComboBox::Pos.y + (ComboBox::Size.y / 8), ComboBox::SelectedName, LIT("Verdana"), 11, ComboBox::CurrentColour, None);
	if (ComboBox::DropWidth < ComboBox::Size.x)
	{
		ComboBox::DropWidth = ComboBox::Size.x;
		ComboBox::SizeDifference = 0;
	}
	if (ComboBox::Active)
	{
		OutlineRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x, ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y + 5, ComboBox::Size.x, (ComboBox::PointerEnd - ComboBox::PointerStart) * ComboBox::Size.y, 1, Colour(130, 130, 130, 255));
	
		int i = 0;
		for (const std::wstring& name : ComboBox::Names)
		{
			if (i < ComboBox::PointerStart)
			{
				i++;
				continue;
			}
			if (i > ComboBox::PointerEnd-1)
			{
				i++;
				continue;
			}
			float itemposy = ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y + 5 + ((i - ComboBox::PointerStart) * ComboBox::Size.y);
			auto it = ComboBox::Items.begin();
			std::advance(it, i);
			if (i % 2 == 0)
				FilledRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x , itemposy,ComboBox::Size.x, ComboBox::Size.y, Colour(30, 30, 30, 255));
			else
				FilledRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x , itemposy, ComboBox::Size.x, ComboBox::Size.y, Colour(50, 50, 50, 255));
			if (IsMouseInRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x, itemposy, ComboBox::Size.x, ComboBox::Size.y))
			{
				float width = GetTextWidth(name, 11, LIT("Verdana"));
				if (ComboBox::CulledNames[i] != ComboBox::Names[i])
				{
					FilledRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x, itemposy, width + 5, ComboBox::Size.y, Colour(150, 150, 150, 120));
					DrawText(ComboBox::ParentPos.x + ComboBox::Pos.x + 5, itemposy + (ComboBox::Size.y / 8), name, LIT("Verdana"), 11, Colour(100, 185, 255, 255), None);
				}
				else if (**it == true)
				{
					FilledRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x, itemposy, ComboBox::Size.x, ComboBox::Size.y, Colour(150, 150, 150, 200));
					DrawText(ComboBox::ParentPos.x + ComboBox::Pos.x + 5, itemposy + (ComboBox::Size.y / 8), ComboBox::CulledNames[i], LIT("Verdana"), 11, Colour(100, 185, 255, 255), None);
				}
				else
				{
					FilledRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x, itemposy, ComboBox::Size.x, ComboBox::Size.y, Colour(150, 150, 150, 120));
					DrawText(ComboBox::ParentPos.x + ComboBox::Pos.x + 5, itemposy + (ComboBox::Size.y / 8), ComboBox::CulledNames[i], LIT("Verdana"), 11, Colour(240, 240, 240, 255), None);

				}

			}
			else
			{
				if (**it == true)
					DrawText(ComboBox::ParentPos.x + ComboBox::Pos.x + 5, itemposy + (ComboBox::Size.y / 8), ComboBox::CulledNames[i], LIT("Verdana"), 11, Colour(100, 185, 255, 255), None);
				else
					DrawText(ComboBox::ParentPos.x + ComboBox::Pos.x + 5, itemposy + (ComboBox::Size.y / 8), ComboBox::CulledNames[i], LIT("Verdana"), 11, Colour(240, 240, 240, 255), None);
			}
			i++;
		}
		if (ComboBox::Names.size() > ComboBox::MaxVisibleItems)
		{
			OutlineRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x , ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y + 4, 6, (ComboBox::PointerEnd - ComboBox::PointerStart) * ComboBox::Size.y + 1, 1, Colour(130, 130, 130, 255));
			int unselectedelements = Names.size() - MaxVisibleItems;
			float unselectedclamp = std::clamp(unselectedelements, 1, (int)Names.size());
			float scrollheight = ((ComboBox::PointerEnd - ComboBox::PointerStart) * ComboBox::Size.y) / (unselectedclamp);
			float scrolly = ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y + 5 + (((PointerEnd - MaxVisibleItems) * Size.y));
			float scrollyclamp = std::clamp(scrolly, ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y + 5, ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y + 5 + ((ComboBox::PointerEnd - ComboBox::PointerStart) * ComboBox::Size.y) - scrollheight);

			FilledRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x, scrollyclamp, 6, scrollheight, Colour(100, 185, 255, 255));
		}
	}
}