#pragma once
#include "BaseEditorPage.h"
#include <QCheckBox>

//SceneRoot�ڵ���Ϣ��ʾ
class SceneWidget : public BaseEditorPage
{
	Q_OBJECT
public:
	SceneWidget();

	void Init(Node* pNode);
private Q_SLOTS:
	void on_physics_click(int state);
	void on_debugrender_click(int state);
private:
	QCheckBox* chkOctree;
	QCheckBox* chkPhysics;
	QCheckBox* chkDebugRender;
};
