#include "stdafx.h"
#include "RectSelectionFrame.h"
#include "../EditorSceneWidget.h"

RectSelectionFrame::RectSelectionFrame()
{
	Context* context_ = EditorRoot::Instance()->context_;

	border_ = new BorderImage(context_);
	ResourceCache* cache = EditorRoot::Instance()->engine_->GetContext()->GetSubsystem<ResourceCache>();

	border_->SetTexture(cache->GetResource<Texture2D>("Textures/Brushes/Rect.png"));
	border_->SetBlendMode(BlendMode::BLEND_SUBTRACT);
	border_->SetPosition(100,100);

	EditorRoot::Instance()->UIRoot_->AddChild(border_);

	isBegin = false;
}

void RectSelectionFrame::Begin(int x,int y)
{
	vecStart.x_ = x;
	vecStart.y_ = y;

	vecEnd = vecStart;

	isBegin = true;

	border_->SetPosition(x,y);
}

void RectSelectionFrame::UpdateShow(int x,int y)
{
	vecEnd.x_ = x;
	vecEnd.y_ = y;
}

void RectSelectionFrame::Hide()
{
	border_->SetVisible(false);
	isBegin = false;
}
