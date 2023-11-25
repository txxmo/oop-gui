#pragma once
#include "Entity.h"
class KeyBind : public Entity
{
protected:
    int* Key;
    void CollectInput();
    std::wstring KeyName;
    std::wstring Mandarin;
    std::wstring English;

    bool Active = false;
    void ConvertKeyToName();

    bool ContextActive = false;
    Vector2 ContextPos;
    Vector2 ContextSize;
    void ContextMenu();
    void Copy();
    void Paste();

    std::chrono::time_point<std::chrono::high_resolution_clock> LastUpdateTime;
    Color CurrentColour;
    const float HoverAnimationDuration = 0.4; // Duration of the color change animation in seconds
    float HoverAnimationProgress = 0.0;
    bool IsHovered = false;
    Color InterpolateColour(const Color& colour1, const Color& colour2, float progress);

    std::map<std::wstring, std::function<void()>> ContextNames = {
    { L"Copy", [this]() { Copy(); } },
    { L"Paste", [this]() { Paste(); } }
    };

    std::map<int, std::wstring> SymbolKeys = { {VK_LEFT,L"Left"},{VK_RIGHT,L"Right"},{VK_UP,L"Up"},{VK_DOWN,L"Down"},{VK_LWIN,L"Win"},{VK_RWIN,L"Win"}
    ,{VK_RWIN,L"Win"} ,{VK_DELETE,L"Delete"} ,{VK_INSERT,L"Insert"},{VK_END,L"End"},{VK_HOME,L"Home"},{VK_NEXT,L"Pg Up"},{VK_DOWN,L"Pg Down"}
    ,{VK_NUMLOCK,L"Num Lock"} ,{VK_DIVIDE,L"Num /"},{VK_LBUTTON,L"Mouse 1"},{VK_RBUTTON,L"Mouse 2"},{VK_MBUTTON,L"Mid Mouse"},{VK_XBUTTON1,L"Mouse 4"},{VK_XBUTTON2,L"Mouse 5"} 
    ,{VK_MENU,L"Alt"},{VK_RMENU,L"Right Alt"} ,{VK_LMENU,L"Left Alt"} };
public:
    KeyBind(float x, float y, std::wstring text,std::wstring mandarin, int* key);
    void Update();
    void Draw();
};
