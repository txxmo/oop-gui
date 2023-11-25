#pragma once
#include "Entity.h"
class Tab : public Container
{
protected:
    int Index;
    int* Selected;
    float HoverTime;
    Color CurrentColour;
    Color OutlineColour;

    std::chrono::time_point<std::chrono::high_resolution_clock> LastUpdateTime;
    const float HoverAnimationDuration = 0.9; // Duration of the color change animation in seconds
    float HoverAnimationProgress = 0.0;
    bool IsHovered = false;
    Color InterpolateColour(const Color& colour1, const Color& colour2, float progress);
public:
    Tab(std::wstring name, float x, float y, float height, int* selectedtab);
    void Update();
    void Draw();
};


