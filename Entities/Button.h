#pragma once
#include "Entity.h"
class Button : public Entity
{
protected:

    std::function< void()> Action;
    void SetWidth();
    std::chrono::time_point<std::chrono::high_resolution_clock> LastUpdateTime;
    Color CurrentColour;
    const float HoverAnimationDuration = 0.5; // Duration of the color change animation in seconds
    float HoverAnimationProgress = 0.0;
    bool IsHovered = false;
    Color InterpolateColour(const Color& colour1, const Color& colour2, float progress);
    std::wstring English;
    std::wstring Mandarin;
public:
    Button(float x, float y, std::wstring text,std::wstring mandarin, std::function<void()> click);
    void Update();
    void Draw();
};
