#pragma once
#include "../Controls/ShowHidePanel.h"
#include "Node.h"
#include <qwidget.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <QColor>
#include <QPalette>
using namespace Urho3D;

class BaseComponentEditor : public ShowHidePanel
{
public:
	virtual void InitComponent(Component* pComponent)
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