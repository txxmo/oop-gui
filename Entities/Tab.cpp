#include "pch.h"
#include "Tab.h"
#include "Input.h"
#include "Gui.h"
#include "Drawing.h"
#include "Font.h"
Tab::Tab(std::wstring name, float x, float y, float height, int* selectedtab = nullptr)
{
	Tab::Size = { GetTextWidth(name,12,LIT("Verdana")) + 10,height};
	Tab::SetVisible(true);
	Tab::Index = TabCount; // this doesn't need to be changed for a different set of tabs or whatever, you dont need to import a value here you just need a nonce value so you never get a repeated one
	Tab::Pos = { x,y };
	Tab::Selected = selectedtab;
	Tab::Name = name;
	Tab::CurrentColour.A = 255;
	Tab::LastUpdateTime = std::chrono::high_resolution_clock::now();
	//	Tab::Size.x += 1;
	TabCount++;
	Tab::HoverAnimationProgress = 0.0;
	Tab::IsHovered = false;
}
void Tab::Update()
{
	if (!Tab::Parent)
		Tab::SetVisible(false);

	if (!Tab::IsVisible())
		return;
	Tab::ParentPos = Tab::GetParent()->GetParentPos();
	Tab::ParentSize = Tab::GetParent()->GetSize();

	size_t ignorecount = 0;

	for (child& it : Tab::Parent->GetContainer())
	{
		if (it == shared_from_this())
			break;

		if (!it->IsVisible())
			++ignorecount;
	}
	bool selected = Index == *Selected;
	if (!selected && !Tab::Blocked)
	{
		if (IsMouseInRectangle(Tab::ParentPos.x + Tab::Pos.x, Tab::ParentPos.y + Tab::Pos.y, Tab::Size.x, Tab::Size.y) && IsKeyClicked(VK_LBUTTON) && !selected && Tab::LastClick < (clock() * 0.00001f))
		{
		
			*Selected = Index;
			// ideally you just want to make a timer and use that but since this is just poc we do this
			Tab::LastClick = (clock() * 0.00001f) + 0.002f;
			Tab::ValueChangeEvent();
		}
	}
	auto currenttime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> deltatime = currenttime - Tab::LastUpdateTime;
	Tab::LastUpdateTime = currenttime;
	double deltaseconds = deltatime.count();
	if (*Selected != Index)
	{
		Tab::OutlineColour = Colour(255, 255, 255, 255);
		bool mouseover = IsMouseInRectangle(Tab::ParentPos.x + Tab::Pos.x, Tab::ParentPos.y + Tab::Pos.y, Tab::Size.x, Tab::Size.y);

		// Update the hover animation progress
		if (mouseover && !IsHovered)
		{
			Tab::IsHovered = true;
			Tab::HoverAnimationProgress = 0.0;
		}
		else if (!mouseover && IsHovered)
		{
			// Mouse stopped hovering, reset animation progress
			Tab::IsHovered = false;
			Tab::HoverAnimationProgress = 1.0;
		}

		if (IsHovered)
		{
			// Increase the progress when hovering
			Tab::HoverAnimationProgress += (1.0 / Tab::HoverAnimationDuration) * deltaseconds;
			Tab::HoverAnimationProgress = std::min(Tab::HoverAnimationProgress, 1.0f);
		}
		else
		{
			// Decrease the progress when not hovering
			Tab::HoverAnimationProgress -= (1.0 / Tab::HoverAnimationDuration) * deltaseconds;
			Tab::HoverAnimationProgress = std::max(Tab::HoverAnimationProgress, 0.0f);
		}

		Tab::CurrentColour = Tab::InterpolateColour(
			Colour(255, 255, 255, 255), 
			Colour(100, 185, 255, 255), 
			Tab::HoverAnimationProgress
		);
	}
	else
	{
		Tab::CurrentColour = Colour(255, 255, 255, 255);
		Tab::OutlineColour = Colour(100, 185, 255, 255);
	}
}
Color Tab::InterpolateColour(const Color& color1, const Color& color2, float progress)
{
	float r = color1.R + (color2.R - color1.R) * progress;
	float g = color1.G + (color2.G - color1.G) * progress;
	float b = color1.B + (color2.B - color1.B) * progress;

	return Colour(static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b), static_cast<unsigned char>(255));
}
void Tab::Draw()
{
	if (!Tab::IsVisible())
		return;

	Color textcolour = Colour(0, 0, 255, 255);
	bool selected = Index == *Selected;

	textcolour = Colour(255, 0, 0, 255);
	OutlineRectangle(ParentPos.x + Pos.x, ParentPos.y + Pos.y, Size.x, Size.y, 1, Tab::OutlineColour);
	FilledRectangle(Tab::ParentPos.x + Tab::Pos.x, Tab::ParentPos.y + Tab::Pos.y, Tab::Size.x, Tab::Size.y, Colour(80, 80, 80, 255));
	DrawText(Tab::ParentPos.x + Tab::Pos.x + (Tab::Size.x / 2), (Tab::ParentPos.y + Tab::Pos.y) + (Tab::Size.y / 2), Tab::Name, LIT("Verdana"), 12, Tab::CurrentColour, CentreCentre);

	if (selected)
	{
		Container::Draw();
		Container::Update();
	}
}

