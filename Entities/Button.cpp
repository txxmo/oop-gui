#include "pch.h"
#include "Input.h"
#include "Drawing.h"
#include "Button.h"
#include "Font.h";
#include "Graphics.h"
Button::Button(float x, float y, std::wstring text,std::wstring mandarin, std::function<void()> click)
{
	Button::Mandarin = mandarin;
	Button::English = text;
	if (*Language == 0)
		Button::Name = English;
	else if (*Language == 1)
		Button::Name = Mandarin;

	Button::Pos = { x,y };
	Button::Size = { 70,20 };
	Button::Action = click;
	Button::SetWidth();
	Button::LastUpdateTime = std::chrono::high_resolution_clock::now();
	Button::HoverAnimationProgress = 0.0;
	Button::IsHovered = false;
}
void Button::SetWidth()
{
	Button::Size.x = GetTextWidth(Button::Name, 11, LIT("Verdana")) + 20;

}
Color Button::InterpolateColour(const Color& color1, const Color& color2, float progress)
{
	float r = color1.R + (color2.R - color1.R) * progress;
	float g = color1.G + (color2.G - color1.G) * progress;
	float b = color1.B + (color2.B - color1.B) * progress;

	return Colour(static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b), static_cast<unsigned char>(255));
}
void Button::Update()
{
	if (!Button::Parent)
		Button::SetVisible(false);
	if (!Button::IsVisible())
		return;
	auto currenttime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> deltatime = currenttime - Button::LastUpdateTime;
	Button::LastUpdateTime = currenttime;
	double deltaseconds = deltatime.count();
	bool mouseover = IsMouseInRectangle(ParentPos.x + Button::Pos.x, ParentPos.y + Button::Pos.y, Button::Size.x, Button::Size.y) && !Button::Blocked;

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
	CurrentColour = Button::InterpolateColour(
		Colour(255, 255, 255, 255), // The color when not hovering 
		Colour(100, 185, 255, 255), // The color when hovering
		HoverAnimationProgress);

	Button::ParentPos = Button::Parent->GetParentPos();
	if (!Button::Blocked)
	{
		if (IsMouseInRectangle(Button::Pos + ParentPos, Button::Size) && IsKeyClicked(VK_LBUTTON) && Button::LastClick < (clock() * 0.00001f))
		{
			Button::Action();
			Button::LastClick = (clock() * 0.00001f) + 0.002f;
		}
	}
}

void Button::Draw()
{
	//Button::Parent->GetParent()->GetParent()->GetPos();
	if (!Button::Parent)
		Button::SetVisible(false);
	if (!Button::IsVisible())
		return;
	OutlineRectangle(ParentPos.x + Pos.x, ParentPos.y + Pos.y, Size.x, Size.y, 1, Colour(20, 20, 20, 255));
	FilledRectangle(ParentPos.x + Button::Pos.x, ParentPos.y + Button::Pos.y, Button::Size.x, Button::Size.y, Colour(80, 80, 80, 255));
	DrawText( ParentPos.x + Button::Pos.x + (Button::Size.x / 2), ParentPos.y + Button::Pos.y + (Button::Size.y / 2), Button::Name, LIT("Verdana"), 12, CurrentColour, CentreCentre);
	
}