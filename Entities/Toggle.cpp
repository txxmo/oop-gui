#include "pch.h"
#include "Toggle.h"
#include "Input.h"
#include "Drawing.h"
#include "Font.h"
#include "Graphics.h"

Toggle::Toggle(float x, float y, std::wstring text,std::wstring mandarin, bool* data = nullptr)
{
	Toggle::Mandarin = mandarin;
	Toggle::English = text;

	if (*Language == 0)
		Toggle::Name = English;
	else if (*Language == 1)
		Toggle::Name = Mandarin;

	Toggle::Pos = { x,y };
	Toggle::Size = { 11,11 };
	Toggle::TextHeight = GetTextHeight(text, 12, LIT("Verdana"));
	Toggle::TextWidth = GetTextWidth(text, 12, LIT("Verdana"));
	Data = data;


}
Color Toggle::InterpolateColour(const Color& colour1, const Color& colour2, float progress)
{
	float r = colour1.R + (colour2.R - colour1.R) * progress;
	float g = colour1.G + (colour2.G - colour1.G) * progress;
	float b = colour1.B + (colour2.B - colour1.B) * progress;

	return Colour(static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b), static_cast<unsigned char>(255));
}
void Toggle::Update()
{

	if (!Toggle::Parent)
		Toggle::SetVisible(false);
	if (!Toggle::IsVisible())
		return;
	auto currenttime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> deltatime = currenttime - Toggle::LastUpdateTime;
	Toggle::LastUpdateTime = currenttime;
	double deltaseconds = deltatime.count();
	Toggle::ParentPos = Toggle::Parent->GetParentPos();
	if (!Toggle::Blocked)
	{
		// square click check
		if (IsMouseInRectangle(Toggle::Pos + Toggle::ParentPos, Toggle::Size) && IsKeyClicked(VK_LBUTTON) && Toggle::LastClick < (clock() * 0.00001f))
		{

			*Toggle::Data = !(*Toggle::Data);
			Toggle::LastClick = (clock() * 0.00001f) + 0.002f;
			Toggle::ValueChangeEvent();

		}
		// text click check
		Vector2 textdimensions = { Toggle::TextWidth + 4,Toggle::TextHeight };
		if (IsMouseInRectangle(Toggle::ParentPos.x + Toggle::Pos.x + (Toggle::Size.x), Toggle::ParentPos.y + Toggle::Pos.y - 1, Toggle::TextWidth, Toggle::TextHeight) && IsKeyClicked(VK_LBUTTON) && Toggle::LastClick < (clock() * 0.00001f))
		{

			*Toggle::Data = !(*Toggle::Data);
			Toggle::LastClick = (clock() * 0.00001f) + 0.002f;
			Toggle::ValueChangeEvent();
		}
	}


	if (*Data)
	{
		Toggle::ClickedAnimationProgress += (1.0f / Toggle::ClickedAnimationDuration) * deltaseconds;
		Toggle::ClickedAnimationProgress = std::min(Toggle::ClickedAnimationProgress, 1.0f);
		Toggle::FadeAnimationProgress += (1.0f / Toggle::FadeAnimationDuration) * deltaseconds;
		Toggle::FadeAnimationProgress = std::min(Toggle::FadeAnimationProgress, 1.0f);
	}
	else {
		Toggle::ClickedAnimationProgress -= (1.0f / Toggle::ClickedAnimationDuration) * deltaseconds;
		Toggle::ClickedAnimationProgress = std::max(Toggle::ClickedAnimationProgress, 0.0f);
		Toggle::FadeAnimationProgress -= (1.0f / Toggle::FadeAnimationDuration) * deltaseconds;
		Toggle::FadeAnimationProgress = std::max(Toggle::FadeAnimationProgress, 0.0f);
	}

	int currentalpha = static_cast<int>(1 + (255 - 1) * Toggle::FadeAnimationProgress);
	int size = static_cast<int>(1 + (Size.x - 2 - 1) * Toggle::ClickedAnimationProgress);
	Toggle::RectangleColour = Colour(100, 185, 255, currentalpha);
	Toggle::ClickedSize = size;

	bool mouseover = IsMouseInRectangle(Toggle::ParentPos.x + Toggle::Pos.x + (Toggle::Size.x), Toggle::ParentPos.y + Toggle::Pos.y - 1, Toggle::TextWidth, Toggle::TextHeight) || IsMouseInRectangle(Toggle::Pos + Toggle::ParentPos, Toggle::Size) && !Toggle::Blocked;

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
	HoverColour = Toggle::InterpolateColour(
		Colour(255, 255, 255, 255), // The color when not hovering 
		Colour(100, 185, 255, 255), // The color when hovering 
		HoverAnimationProgress);
}
	


void Toggle::Draw()
{
	if (!Toggle::Parent)
		Toggle::SetVisible(false);
	if (!Toggle::IsVisible())
		return;

	OutlineRectangle(Toggle::ParentPos.x + Toggle::Pos.x, Toggle::ParentPos.y + Toggle::Pos.y, Toggle::Size.x, Toggle::Size.y, 1, Colour(20, 20, 20, 255));
	FilledRectangle(Toggle::ParentPos.x + Toggle::Pos.x, Toggle::ParentPos.y + Toggle::Pos.y, Toggle::Size.x, Toggle::Size.y, Colour(80, 80, 80, 255));
	if (*Toggle::Data == true)
	{
	
		FilledRectangle(Toggle::ParentPos.x + Toggle::Pos.x + 1, Toggle::ParentPos.y + Toggle::Pos.y + 1, Toggle::ClickedSize, Toggle::ClickedSize, Toggle::RectangleColour);
	}
	DrawText( Toggle::ParentPos.x + Toggle::Pos.x + (Toggle::Size.x) + 3, Toggle::ParentPos.y + Toggle::Pos.y - (Toggle::Size.y / 8), Toggle::Name, LIT("Verdana"), 12, HoverColour, None);

}