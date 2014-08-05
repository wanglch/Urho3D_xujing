#pragma once
#include <list>
using namespace std;
#include <QWidget>
#include <QPushButton>
#include "../Controls/LabelTextbox.h"

#include "../Controls/ShowHidePanel.h"
#include "../ComponentEditor/LightWidget.h"
#include "../ComponentEditor/SceneWidget.h"
#include "../ComponentEditor/SkyboxWidget.h"
#include "../ComponentEditor/StaticModelWidget.h"
#include "../ComponentEditor/AnimatedModelWidget.h"
#include "../ComponentEditor/TerrainWidget.h"
#include "../ComponentEditor/NodeWidget.h"

class ObjectPropertiesView : public QWidget
{
public:
	ObjectPropertiesView(QWidget* parent = NULL);

	void SetEditNode(Node* pNode);
private:
	QWidget* wContent;
	QVBoxLayout* vContentLayout;
	LabelTextBox* descWidget_;
	QVBoxLayout* layoutContent;

	//һЩ����༭���
	NodeWidget* positionWidget_;

	LightWidget* lightWidget_;
	SceneWidget* sceneWidget_;
	SkyboxWidget* skyboxWidget_;
	StaticModelWidget* staticModelWidget_;
	AnimatedModelWidget* animatedModelWidget_;

	TerrainWidget* terrainWidget_;
	QPushButton* btnAddComponent;

	list<ShowHidePanel*> lstWidges;

	QWidget* fixWidget;
};
