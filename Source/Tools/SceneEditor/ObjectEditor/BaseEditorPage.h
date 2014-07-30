#pragma once
#include "../Controls/ShowHidePanel.h"
#include "Node.h"
#include <qwidget.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <QColor>
#include <QPalette>

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

	void SetWidgetFontColor(QWidget* widget,const QColor& qcolor)
	{
		QPalette pe;
		pe.setColor(QPalette::WindowText,qcolor);
		widget->setPalette(pe);
	}

	void BeginInit()
	{
		bEditNotify = false;
	}

	void EndInit()
	{
		bEditNotify = true;
	}
};