#pragma once
#include "Entity.h"
class GroupBox : public Container
{
protected:
    float TextWidth;
    float TextHeight;
    float TextStartPos;
    int TextStartOffset;

    float YBuffer;
    std::chrono::time_point<std::chrono::high_resolution_clock> LastUpdateTime;
    Color CurrentColour;
    const float HoverAnimationDuration = 0.5; // Duration of the color change animation in seconds
    float HoverAnimationProgress = 0.0;
    bool IsHovered = false;
    Color InterpolateColour(const Color& colour1, const Color& colour2, float progress);
    std::wstring Mandarin;
    std::wstring English;
public:
    GroupBox(std::wstring name,std::wstring mandarin, float x, float y, float width, float height);
    void Update();
    void Draw();
};


