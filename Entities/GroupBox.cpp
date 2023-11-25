#include "pch.h"
#include "GroupBox.h"
#include "Input.h"
#include "Gui.h"
#include "Drawing.h"
#include "Font.h"
#include "Animation.h"
#include "Graphics.h"
GroupBox::GroupBox(std::wstring name,std::wstring mandarin, float x, float y, float width, float height)
{
	GroupBox::Mandarin = mandarin;
	GroupBox::English = name;
	if (*Language == 0)
		GroupBox::Name = English;
	else if (*Language == 1)
		GroupBox::Name = Mandarin;

	GroupBox::Size = { width,height };
	GroupBox::SetVisible(true);
	GroupBox::Pos = { x,y };
	GroupBox::TextHeight = GetTextHeight(GroupBox::Name, 12, LIT("Verdana"));
	GroupBox::TextWidth = GetTextWidth(GroupBox::Name, 12, LIT("Verdana"));
	GroupBox::TextStartOffset = 5;
	GroupBox::CurrentColour.A = 255;
	GroupBox::LastUpdateTime = std::chrono::high_resolution_clock::now();
	HoverAnimationProgress = 0.0;
	IsHovered = false;

}
void GroupBox::Update()
{
	if (!GroupBox::Parent)	
		GroupBox::SetVisible(false);

	if (!GroupBox::IsVisible())
		return;
	GroupBox::TextStartPos = GroupBox::Parent->GetParent()->GetPos().x + GroupBox::Pos.x + GroupBox::TextStartOffset;
	GroupBox::ParentPos = { GroupBox::Parent->GetParent()->GetPos().x + GroupBox::Pos.x + 5, GroupBox::Parent->GetParent()->GetPos().y + GroupBox::Pos.y + 10};
	GroupBox::ParentSize = { GroupBox::Size };
	auto currenttime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> deltatime = currenttime - GroupBox::LastUpdateTime;
	GroupBox::LastUpdateTime = currenttime;
	double deltaseconds = deltatime.count();
	for (child& it : GroupBox::GetContainer())
	{
		// if it exceeds the position/size of this then dont draw it.
		bool inbox = (
			GroupBox::ParentPos.x + it->Pos.x >  GroupBox::Pos.x &&
			GroupBox::ParentPos.y + it->Pos.y >  GroupBox::Pos.y &&
			GroupBox::ParentPos.x + it->Pos.x + it->Size.x < GroupBox::ParentPos.x + GroupBox::Pos.x + GroupBox::Size.x &&
			GroupBox::ParentPos.y + it->Pos.y + it->Size.y < GroupBox::ParentPos.y+ GroupBox::Pos.y + GroupBox::Size.y
			);
		it->SetVisible(inbox);

	}
	bool mouseover = IsMouseInRectangle(
		GroupBox::Parent->GetParent()->GetPos().x + GroupBox::Pos.x,
		GroupBox::Parent->GetParent()->GetPos().y + GroupBox::Pos.y,
		GroupBox::Size.x,
		GroupBox::Size.y
	);

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
	CurrentColour = GroupBox::InterpolateColour(
		Colour(0,0,0,255), // The color when not hovering 
		Colour(100, 185, 255, 255), // The color when hovering 
		HoverAnimationProgress
	);

}
Color GroupBox::InterpolateColour(const Color& color1, const Color& color2, float progress)
{
	float r = color1.R + (color2.R - color1.R) * progress;
	float g = color1.G + (color2.G - color1.G) * progress;
	float b = color1.B + (color2.B - color1.B) * progress;

	return Colour(static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b), static_cast<unsigned char>(255));
}
void GroupBox::Draw()
{
	if (!GroupBox::IsVisible())
		return;

	FilledLine(GroupBox::Parent->GetParent()->GetPos().x + GroupBox::Pos.x, GroupBox::Parent->GetParent()->GetPos().y + GroupBox::Pos.y, TextStartPos, GroupBox::Parent->GetParent()->GetPos().y + GroupBox::Pos.y, 2, CurrentColour); // draw up to text
	FilledLine(TextStartPos + GroupBox::TextWidth, GroupBox::Parent->GetParent()->GetPos().y + GroupBox::Pos.y, GroupBox::Parent->GetParent()->GetPos().x + GroupBox::Pos.x + GroupBox::Size.x, GroupBox::Parent->GetParent()->GetPos().y + GroupBox::Pos.y, 2, CurrentColour); // draw past
	OutlineRectangle(GroupBox::Parent->GetParent()->GetPos().x + GroupBox::Pos.x, GroupBox::Parent->GetParent()->GetPos().y + GroupBox::Pos.y +1, GroupBox::Size.x, GroupBox::Size.y-1, 2, CurrentColour);
	FilledRectangle(GroupBox::Parent->GetParent()->GetPos().x + GroupBox::Pos.x, GroupBox::Parent->GetParent()->GetPos().y + GroupBox::Pos.y, GroupBox::Size.x, GroupBox::Size.y, Colour(50, 50, 50, 255));
	
	DrawText(TextStartPos + 1, GroupBox::Parent->GetParent()->GetPos().y + GroupBox::Pos.y - (TextHeight / 2), GroupBox::Name, LIT("Verdana"), 12, Colour(255, 255, 255, 255), None);
	Container::Draw();
	Container::Update();
	
}

