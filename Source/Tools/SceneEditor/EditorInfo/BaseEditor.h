/*
���б༭���Ļ���
*/
#pragma once
#include <string>
#include "../EditorManager/EventsDefinitions.h"

//Ϊ�˼���ƣ���������Ϣ���ᷢ�͵���ǰEditor������ñ༭�����������Ϣ�����ظú�������
class BaseEditor : public MouseListener , public KeyboardListener
{
public:
	//ÿһ֡����(���ע���˵Ļ�)
	virtual bool update(float timePassed)
	{
		return false;
	}

	virtual void OnObjectSelect(Node* pNode)
	{

	}
};
