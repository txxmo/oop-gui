#pragma once
#include "Entity.h"
class DropDown : public Entity
{
protected:
    float DropWidth;
    std::vector<std::wstring>Names;
    std::vector<std::wstring> MandarinNames;
    std::vector<std::wstring> EnglishNames;
    std::vector<std::wstring> CulledNames;
    int* Index;
    bool Active = false;
    std::wstring SelectedName;
    float TextWidth = 0;
    float LongestTextWidth = 0;
    float CutOffBuffer = 0;
    float SizeDifference;

    int MaxVisibleItems = 5;

    int PointerStart = 0;
    int PointerEnd = MaxVisibleItems;

    void ConvertSelectedName();
    void CalculateBuffer();
    void SetDropDownWidth();
    void ArrowNavigation();
    void UpdateScrollBar();

    bool ScrollBar = false;

    std::chrono::time_point<std::chrono::high_resolution_clock> LastUpdateTime;
    Color CurrentColour;
    const float HoverAnimationDuration = 0.3; // Duration of the color change animation in seconds
    float HoverAnimationProgress = 0.0;
    bool IsHovered = false;
    Color InterpolateColour(const Color& colour1, const Color& colour2, float progress);

    std::wstring English;
    std::wstring Mandarin;
public:
    DropDown(float x, float y, std::wstring text,std::wstring mandarin, int* items,std::vector<std::wstring>names,std::vector<std::wstring>mandarinnames,int maxitems);
    void Update();
    void Draw();
    std::wstring GetSelectedName();
};
