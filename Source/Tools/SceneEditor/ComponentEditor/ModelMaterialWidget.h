#pragma once
#include "BaseComponentEditor.h"
#include "StaticModel.h"
using namespace Urho3D;

//��ģ��(StaticModel,AnimationModel��)ָ�����ʵĴ���
class ModelMaterialWidget : public BaseComponentEditor
{
	Q_OBJECT
public:
	ModelMaterialWidget();
	~ModelMaterialWidget();

	void Init(Node* pNode);

	void SetOwnerModel(StaticModel* pModel);

private Q_SLOTS:
	void onButtonSelectClicked(bool checked);
private:
	//�Ƴ����е���Ŀ������ģ�͵Ĳ���������̬����
	void ClearItems();
};
