#pragma once
#include "Text.h"
using namespace Urho3D;

//�������ʾ������Tip����UIʵ��
class ObjectNameTip
{
public:
	ObjectNameTip();

	//������ʾλ��
	void UpdateShow(int x,int y);
	void Hide();

	void SetText(const char* pName);
private:
	Text* text_;
};
