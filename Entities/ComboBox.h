#pragma once
#include "Entity.h"
class ComboBox : public Entity
{
protected:
    float DropWidth;
    std::vector<std::wstring>Names;
    std::vector<bool*>Items;
    std::vector<std::wstring>CulledNames;
    bool Active = false;
    std::wstring SelectedName;
    float TextWidth = 0;
    float CutOffBuffer = 0;
    float SizeDifference;

    int MaxVisibleItems = 5;

    int PointerStart = 0;
    int PointerEnd = MaxVisibleItems;

    void ConvertSelectedName();
    void CalculateBuffer();
    void SetComboBoxWidth();
    void ArrowNavigation();
    void UpdateScrollBar();

    bool ScrollBarHeld = false;

    std::chrono::time_point<std::chrono::high_resolution_clock> LastUpdateTime;
    Color CurrentColour;
    const float HoverAnimationDuration = 0.3; // Duration of the color change animation in seconds
    float HoverAnimationProgress = 0.0;
    bool IsHovered = false;
    Color InterpolateColour(const Color& colour1, const Color& colour2, float progress);
public:
    ComboBox(float x, float y, std::wstring text, std::vector<bool*>items, std::vector<std::wstring>names);
    void Update();
    void Draw();
};
