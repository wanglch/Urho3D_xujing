#pragma once
#include "../Controls/ShowHidePanel.h"
#include "Node.h"
#include <qwidget.h>
#include <qcombobox.h>
#include <qcheckbox.h>

class BaseEditorPage : public ShowHidePanel
{
public:
	virtual void Init(Node* pNode)
	{
		bEditNotify = true;
	}
protected:
	Node* pOwnerNode;

	//���⻥����µ��µĴ���
	bool bEditNotify;	//�Ƿ�ɱ༭

	void BeginInit()
	{
		bEditNotify = false;
	}

	void EndInit()
	{
		bEditNotify = true;
	}
};