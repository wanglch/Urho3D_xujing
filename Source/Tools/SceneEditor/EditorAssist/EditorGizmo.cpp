#include "stdafx.h"
#include "EditorGizmo.h"
#include "Plane.h"

const float axisMaxD = 0.1;	//��Զ������ѡ��
const float axisMaxT = 1.0;

bool GizmoAxis::QueryRaySelect(const Ray& worldRay)
{
	//������������������ϵ������
	Vector3 closest = worldRay.ClosestPoint(axisRay);
	//��������ϵ�closest�����ڶ�Ӧ�������ϵ�ͶӰ��
	Vector3 projected = axisRay.Project(closest);

	d = axisRay.Distance(closest);	//���߼�ļ��

	//�ж��������Ƿ�����Ұ�ĺ���
	Node* cameraNode = EditorRoot::Instance()->cameraNode_;
	Plane axisPlane(cameraNode->GetWorldPosition(),pOwnerGizmo->GetNode()->GetWorldPosition(),axisRay.origin_ + axisRay.direction_);
	if(axisPlane.Distance(closest) < 0.0)
		d = -d;

	//ͶӰ�㵽Axisԭ����룬�϶�������Ҫ��֤�þ��벻��
	t = (projected - axisRay.origin_).DotProduct(axisRay.direction_);

	//�Ƿ�ѡ�У�������������������һ�����룬����ԭ��ľ���
	float scale = pOwnerGizmo->GetNode()->GetScale().x_;
	bool bRayDistance = Abs(d) < axisMaxD * scale;
	bool bOriDistance = (t >= - axisMaxD * scale) && (t <= axisMaxT * scale);

	return bRayDistance && bOriDistance;
}

Editor3dGizmo::Editor3dGizmo(Context* context) : Object(context)
{
	lastGizmoMode = enEditMode_Move;

	gizmoAxisX = NULL;
	gizmoAxisY = NULL;
	gizmoAxisZ = NULL;

	gizmoNode = NULL;
	gizmo = NULL;

	IsDrag = false;
}

void Editor3dGizmo::BeginDrag()
{
	IsDrag = true;

	//��¼���ʱ�������ᣬΪ�˺�����϶�����
	gizmoAxisX->lastT = gizmoAxisX->t;
	gizmoAxisY->lastD = gizmoAxisY->d;
	gizmoAxisZ->lastD = gizmoAxisZ->d;

	//todo:
	posOnClick_ = EditorRoot::Instance()->SelectionNodes[0]->GetWorldPosition();
}

void Editor3dGizmo::EndDrag()
{
	IsDrag = false;

	if(EditorRoot::Instance()->SelectionNodes.size() > 0)
	{
		Node* pNode = EditorRoot::Instance()->SelectionNodes[0];
		gizmoNode->SetWorldPosition(pNode->GetWorldPosition());
	}

	gizmoNode->SetEnabled(true);
}

void Editor3dGizmo::UpdateMovePosition()
{
	Vector3 adjust(0,0,0);
	if(gizmoAxisX->selected)
		adjust += Vector3(1,0,0) * (gizmoAxisX->t - gizmoAxisX->lastT);
	if(gizmoAxisY->selected)
		adjust += Vector3(0,1,0) * (gizmoAxisY->t - gizmoAxisY->lastT);
	if(gizmoAxisZ->selected)
		adjust += Vector3(0,0,1) * (gizmoAxisZ->t - gizmoAxisZ->lastT);

	//todo:���Node�����
	Node* pNode = EditorRoot::Instance()->SelectionNodes[0];
	pNode->SetWorldPosition(posOnClick_ + adjust);

	gizmoNode->SetEnabled(false);
}

void Editor3dGizmo::CreateGizmo()
{
	ResourceCache* cache = GetSubsystem<ResourceCache>();

	gizmoNode = EditorRoot::Instance()->scene_->CreateChild("Axis");
	gizmo = gizmoNode->CreateComponent<StaticModel>();
	gizmo->SetTemporary(true);	//�������ᱻ����

	lastGizmoMode = enEditMode_Scale;
	
	//gizmo->SetMaterial(0,cache->GetResource<Material>("Materials/Editor/RedUnlit.xml"));
	//gizmo->SetMaterial(1,cache->GetResource<Material>("Materials/Editor/GreenUnlit.xml"));
	//gizmo->SetMaterial(2,cache->GetResource<Material>("Materials/Editor/BlueUnlit.xml"));
	gizmo->SetEnabled(false);

	EditorRoot::Instance()->SendNodeUpdateShowEvent(gizmoNode);

	//
	gizmoAxisX = new GizmoAxis(context_);
	gizmoAxisY = new GizmoAxis(context_);
	gizmoAxisZ = new GizmoAxis(context_);
	gizmoAxisX->pOwnerGizmo = this;
	gizmoAxisY->pOwnerGizmo = this;
	gizmoAxisZ->pOwnerGizmo = this;

	gizmoAxisX->selected = false;
	gizmoAxisY->selected = false;
	gizmoAxisZ->selected = false;

	update_model();

	SubscribeToEvent(E_UPDATE,HANDLER(Editor3dGizmo,HandleUpdate));
}

void Editor3dGizmo::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
	if(gizmo->IsEnabled() == false)
		return;

	Node* cameraNode_ = EditorRoot::Instance()->cameraNode_;
	Camera* pCamera = cameraNode_->GetComponent<Camera>();
	if(pCamera != NULL)
	{
		float c = 0.1;
		if(pCamera->IsOrthographic())
		{
			gizmoNode->SetScale(c);
		}
		else
		{
			Vector3 vWorldRay = gizmoNode->GetWorldPosition() - cameraNode_->GetWorldPosition();
			//ת�������������ľ��룬Ҳ���Ǵ�ֱ����Ļ�ľ���
			float z = (cameraNode_->GetWorldRotation().Inverse() * vWorldRay).z_;
			gizmoNode->SetScale(c * z);
		}
	}

	CalculateGizmoAxes();
}

void Editor3dGizmo::ShowGizmo()
{
	if(gizmo != NULL)
	{
		gizmo->SetEnabled(true);

		//ѡ������ʱĬ�������ᶼѡ��
		gizmoAxisX->selected = true;
		gizmoAxisY->selected = true;
		gizmoAxisZ->selected = true;

		update_model();
	}
}

void Editor3dGizmo::SetMode(enEditMode mode)
{
	lastGizmoMode = mode;
	update_model();
}

void Editor3dGizmo::HideGizmo()
{
	if(gizmo != NULL)
		gizmo->SetEnabled(false);
}

void Editor3dGizmo::update_model()
{
	ResourceCache* cache = GetSubsystem<ResourceCache>();

	switch(lastGizmoMode)
	{
	case enEditMode_Move:
		gizmo->SetModel(cache->GetResource<Model>("Models/Editor/Axes.mdl"));
		break;
	case enEditMode_Rotate:
		gizmo->SetModel(cache->GetResource<Model>("Models/Editor/RotateAxes.mdl"));
		break;
	case enEditMode_Scale:
		gizmo->SetModel(cache->GetResource<Model>("Models/Editor/ScaleAxes.mdl"));
		break;
	}

	//���²��� x,y,z��ֱ����ڲ���[0],[1],[2]
	gizmo->SetMaterial(0,cache->GetResource<Material>(gizmoAxisX->IsHighShow() ? "Materials/Editor/BrightRedUnlit.xml" : "Materials/Editor/RedUnlit.xml"));
	gizmo->SetMaterial(1,cache->GetResource<Material>(gizmoAxisY->IsHighShow() ? "Materials/Editor/BrightGreenUnlit.xml" : "Materials/Editor/GreenUnlit.xml"));
	gizmo->SetMaterial(2,cache->GetResource<Material>(gizmoAxisZ->IsHighShow() ? "Materials/Editor/BrightBlueUnlit.xml" : "Materials/Editor/BlueUnlit.xml"));
}

void Editor3dGizmo::CalculateGizmoAxes()
{
	gizmoAxisX->axisRay = Ray(gizmoNode->GetWorldPosition(),gizmoNode->GetWorldRotation() * Vector3(1,0,0));
	gizmoAxisY->axisRay = Ray(gizmoNode->GetWorldPosition(),gizmoNode->GetWorldRotation() * Vector3(0,1,0));
	gizmoAxisZ->axisRay = Ray(gizmoNode->GetWorldPosition(),gizmoNode->GetWorldRotation() * Vector3(0,0,1));
}

//����
void Editor3dGizmo::QuerySelectedStateByWorldRay(const Ray& worldRay,bool& xSelect,bool& ySelect,bool& zSelect)
{
	xSelect = gizmoAxisX->QueryRaySelect(worldRay);
	ySelect = gizmoAxisY->QueryRaySelect(worldRay);
	zSelect = gizmoAxisZ->QueryRaySelect(worldRay);
}

void Editor3dGizmo::Update()
{

}
