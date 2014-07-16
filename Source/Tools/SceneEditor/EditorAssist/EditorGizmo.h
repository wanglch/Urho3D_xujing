#pragma once
#include "Object.h"
#include "StaticModel.h"
#include "Ray.h"
#include "Context.h"
#include <vector>
using namespace std;

namespace Urho3D
{
	class Node;
	class Context;
}

using namespace Urho3D;

enum enEditMode
{
	enEditMode_Move,	//�ƶ�
	enEditMode_Rotate,	//��ת
	enEditMode_Scale,	//����
};

class Editor3dGizmo;

//����������
class GizmoAxis : public Object
{
	OBJECT(GizmoAxis)
public:
	GizmoAxis(Context* context) : Object(context)
	{
		selected = false;
		hovered = false;

		t = d = lastT = lastD = 0;
	}

	//�ж���������Ƿ�ѡ��
	bool QueryRaySelect(const Ray& worldRay);

	bool IsHighShow()
	{
		return selected || hovered;
	}

	Ray axisRay;	//�������е�ָ��(Node��ƫ�Ʒ�����Ǹ������)
	bool selected;	//�Ƿ�ѡ�У������������ԭ�㸽��ʱ���������ᶼ�ᱻѡ��(���ڵ��ʱ���¸�ֵ���϶�ʱ���ݸ�ֵ�ж��Ƿ�Ӧ���϶�)
	bool hovered;	//��ǰ�Ƿ�����

	float t;
	float d;
	float lastT;
	float lastD;

	Editor3dGizmo* pOwnerGizmo;
};

//���Nodeʱ��ʾ������������
class Editor3dGizmo : public Object
{
	OBJECT(Editor3dGizmo)
public:
	Editor3dGizmo(Context* context);

	void CreateGizmo();
	void ShowGizmo();
	void HideGizmo();

	void SetMode(enEditMode mode);
public:
	GizmoAxis* gizmoAxisX;
	GizmoAxis* gizmoAxisY;
	GizmoAxis* gizmoAxisZ;

	bool IsSelected()
	{
		return gizmoAxisX->selected || gizmoAxisY->selected || gizmoAxisZ->selected;
	}

	Node* GetNode()
	{
		return gizmoNode;
	}

	StaticModel* GetModel()
	{
		return gizmo;
	}

	//����ѡ��״̬������
	void Update();

	//����world���ߣ�����������������ཻ���
	void QuerySelectedStateByWorldRay(const Ray& worldRay,bool& xSelect,bool& ySelect,bool& zSelect);
	//���ݵ�ǰ�༭������ʾ��ͬ��ģ��
	void update_model();

	void BeginDrag();
	void EndDrag();

	void UpdateMovePosition();

	Vector3 posOnClick_;	//��¼�������ʱ��������ԭ��λ��
	bool IsDrag;
private:

	void HandleUpdate(StringHash eventType, VariantMap& eventData);

	//���������������������е�ָ��
	void CalculateGizmoAxes();

	enEditMode lastGizmoMode;

	Node* gizmoNode;
	StaticModel* gizmo;
};
