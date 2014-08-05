#pragma once
#include <QWidget>
#include <QFrame>
#include <QVBoxLayout>

#include "StaticModel.h"
using namespace Urho3D;

//��ģ��(StaticModel,AnimationModel��)ָ�����ʵĴ���
class ModelMaterialWidget : public QFrame
{
	Q_OBJECT
public:
	ModelMaterialWidget(QWidget* parent = NULL);
	~ModelMaterialWidget();

	void InitComponent(Component* pComponent);

	void SetOwnerModel(StaticModel* pModel);

private Q_SLOTS:
	void onButtonSelectClicked(bool checked);
	void onColorFramesClicked(bool checked);
private:
	//�Ƴ����е���Ŀ������ģ�͵Ĳ���������̬����
	void ClearItems();
	QVBoxLayout* vLayout;
};
