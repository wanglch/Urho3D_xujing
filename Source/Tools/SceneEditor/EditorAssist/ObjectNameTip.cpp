#include "stdafx.h"
#include "ObjectNameTip.h"
#include "Color.h"

ObjectNameTip::ObjectNameTip()
{
	Context* context_ = EditorsRoot::Instance()->context_;
	text_ = new Text(context_);
	text_->SetStyle("ToolTipText");
	text_->SetColor(Color(1,1,1));
	//����߿�
	text_->SetTextEffect(TextEffect::TE_STROKE);
	text_->SetEffectColor(Color(0,0,0));

	EditorsRoot::Instance()->UIRoot_->AddChild(text_);
}

//������ʾλ��
void ObjectNameTip::UpdateShow(int x,int y)
{
	text_->SetVisible(true);
	text_->SetPosition(x + 30,y);
}

void ObjectNameTip::Hide()
{
	text_->SetVisible(false);
}

void ObjectNameTip::SetText(const char* pName)
{
	text_->SetText(pName);
}
