#include "pch.h"
#include "TabController.h"
#include "Drawing.h"
TabController::TabController()
{
	TabController::SetVisible(true);
}

void TabController::Update()
{
	if (!TabController::Parent)
		TabController::SetVisible(false);

	// The tab control is NOT valid, or disabled.
	if (!TabController::IsVisible())
		return;
	TabController::ParentPos = TabController::Parent->GetParentPos();
	TabController::Pos.x = TabController::Parent->GetPos().x;
	TabController::Pos.y = TabController::Parent->GetPos().y;
	TabController::Size = TabController::Parent->GetSize();
	TabController::Container::Update();
}

void TabController::Draw()
{
	if (!TabController::IsVisible())
		return;
	
	  FilledLine(TabController::Pos.x + 10, TabController::Pos.y + 60, TabController::Pos.x + TabController::Size.x - 10, TabController::Pos.y + 60, 1, Colour(100, 185, 255, 255));

	TabController::Container::Draw();
}