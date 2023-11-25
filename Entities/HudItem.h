#pragma once
#include "Entity.h"
class HudItem : public Container
{
	protected:
		std::vector<std::wstring>* Items;
		int* X;
		int* Y;
		bool* Enable;

		Vector2 Header;
		bool Dragging = false;
		Vector2	Drag;
		int LongestString = 0;
		void DragAction();

public:
	HudItem(int* x, int* y, bool* enable,std::wstring name,std::vector<std::wstring>*items);
	void Draw();
	void Update();
};