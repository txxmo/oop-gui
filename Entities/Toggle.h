#pragma once
#include "Entity.h"
class Toggle : public Entity
{
protected:
    std::chrono::time_point<std::chrono::high_resolution_clock> LastUpdateTime;
    Color RectangleColour;
    const float ClickedAnimationDuration = 0.09; // Duration of the size change animation in seconds
    float ClickedAnimationProgress = 0.0;
    const float FadeAnimationDuration = 1.8; // Duration of the color alpha change animation in seconds
    float FadeAnimationProgress = 0.0;
    float ClickedSize = 0;
    Color HoverColour;
    const float HoverAnimationDuration = 0.3; // Duration of the color change animation in seconds
    float HoverAnimationProgress = 0.0;
    bool IsHovered = false;
    Color InterpolateColour(const Color& colour1, const Color& colour2, float progress);

    bool* Data;
    bool Tabbed = false;

    float TextWidth = 0;
    float TextHeight = 0;

    std::wstring English;
    std::wstring Mandarin;

public:
    Toggle(float x, float y, std::wstring text,std::wstring mandarin, bool* data);
    void Update();
    void Draw();
};
