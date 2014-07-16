#include "stdafx.h"
#include "ObjectPositionEditor.h"
#include "../Manager/SceneHelper.h"
#include "DebugRenderer.h"
#include "../EditorSceneWidget.h"
#include "Skybox.h"

ObjectPositionEditor::ObjectPositionEditor(Context* context) : Object(context)
{
	CurrentHoverObject = NULL;
	isRectSelectionMode = false;

	SubscribeToEvent(E_POSTRENDERUPDATE,HANDLER(ObjectPositionEditor,HandlePostRenderUpdate));
}

//��ʾѡ������İ�Χ��
void ObjectPositionEditor::HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData)
{
	EditorsRoot* pEditorRoot = EditorsRoot::Instance();
	DebugRenderer* debug = pEditorRoot->scene_->GetComponent<DebugRenderer>();
	if(debug == NULL)
		return;

	vector<Node*> nodes = pEditorRoot->GetUnionSelections();
	for(int i = 0;i < nodes.size();i ++)
	{
		Node* node = nodes[i];
		if(node->GetComponent<Skybox>() != NULL)
			continue;

		debug->AddNode(node,1.0f,false);

		const Vector<SharedPtr<Component> >& components = node->GetComponents();
		for(int j = 0;j < node->GetNumComponents();j ++)
		{
			Drawable* drawable = dynamic_cast<Drawable*>(components[j].Get());
			if(drawable != NULL)
			{
				debug->AddBoundingBox(drawable->GetWorldBoundingBox(),Color::WHITE,true);
			}
		}
	}

	//�����ܵ�
	if(nodes.size() > 1)
	{
		BoundingBox allBox;
		for(int i = 0;i < nodes.size();i ++)
		{
			Node* node = nodes[i];
			if(node->GetComponent<Skybox>() != NULL)
				continue;

			const Vector<SharedPtr<Component> >& components = node->GetComponents();
			for(int j = 0;j < node->GetNumComponents();j ++)
			{
				Drawable* drawable = dynamic_cast<Drawable*>(components[j].Get());
				if(drawable != NULL)
				{
					allBox.Merge(drawable->GetWorldBoundingBox());
				}
			}
		}

		debug->AddBoundingBox(allBox,Color::BLUE,true);
	}

	if(CurrentHoverObject != NULL)
	{
		CurrentHoverObject->DrawDebugGeometry(debug,false);
	}
}

void ObjectPositionEditor::OnMouseLeftDown(float x,float y,unsigned int buttons)
{
	EditorsRoot* pEditorRoot = EditorsRoot::Instance();
	Editor3dGizmo* pGizmo = pEditorRoot->GetGizmo();

	//�жϵ�������ݣ����������ж���û�а�סCtrl����ѡ��
	IntVector2 pos(x,y);
	vector<SceneHitResult> result = SceneHelper::Instance()->QueryCurrentMousePosObjects(250.0,&pos);

	//�ж��Ƿ���������
	if(is_axis_object(result) == true)
	{
		Graphics* graphics = GetSubsystem<Graphics>();
		Camera* camera = pEditorRoot->cameraNode_->GetComponent<Camera>();

		Ray cameraWorldRay = camera->GetScreenRay(x / graphics->GetWidth(),y / graphics->GetHeight());

		//�жϲ���¼��Щ�����ᱻѡ����
		bool bXSelect;
		bool bYSelect;
		bool bZSelect;
		pGizmo->QuerySelectedStateByWorldRay(cameraWorldRay,bXSelect,bYSelect,bZSelect);
		pGizmo->gizmoAxisX->selected = bXSelect;
		pGizmo->gizmoAxisY->selected = bYSelect;
		pGizmo->gizmoAxisZ->selected = bZSelect;

		if(pGizmo->IsSelected())
		{
			pGizmo->BeginDrag();
		}

		//�ж�Ctrl��û�а���
		pGizmo->update_model();
	}
	else
	{
		Drawable* selObject = get_edit_object(result);
		if(selObject == NULL)
		{
			if(pEditorRoot->IsCtrlPressed() == false)
			{
				pEditorRoot->CancelAllSelection();
			}

			pEditorRoot->RectSelectionFrame_->Begin(x,y);

			isRectSelectionMode = true;
			rectSelectionStart.x_ = x;
			rectSelectionStart.y_ = y;
		}
		else
		{
			//ѡ�еĽڵ�(����Ҫ�����Ƿ�סCtrl���ж����ӻ���ȥ��)
			Node* pSelNode = selObject->GetNode();
			pEditorRoot->OnNodeSelect(pSelNode);

			//
			pGizmo->gizmoAxisX->selected = true;
			pGizmo->gizmoAxisY->selected = true;
			pGizmo->gizmoAxisZ->selected = true;

			pGizmo->BeginDrag();
			pGizmo->update_model();
		}
	}
}

void ObjectPositionEditor::OnMouseLeftUp(float x,float y,unsigned int buttons)
{
	EditorsRoot* pEditorRoot = EditorsRoot::Instance();

	if(isRectSelectionMode == true)
	{
		pEditorRoot->OnRectSelectionEnd();
		isRectSelectionMode = false;
	}
	else
	{
		Editor3dGizmo* pGizmo = pEditorRoot->GetGizmo();
		if(pGizmo == NULL)
			return;

		if(pGizmo->IsDrag == true)
		{
			pGizmo->EndDrag();
		}
	}
}

Drawable* ObjectPositionEditor::get_edit_object(const vector<SceneHitResult>& result)
{
	for(int i = 0;i < result.size();i ++)
	{
		const SceneHitResult& obj = result[i];
		const Urho3D::String& typeName = obj.object->GetTypeName();
		if(typeName == "StaticModel" || typeName == "AnimatedModel")
		{
			if(obj.object != EditorsRoot::Instance()->GetGizmo()->GetModel())
			{
				return obj.object;
			}
		}
	}

	return NULL;
}

//��һ������ģ�ͱ�����������
bool ObjectPositionEditor::is_axis_object(const vector<SceneHitResult>& result)
{
	for(int i = 0;i < result.size();i ++)
	{
		const SceneHitResult& obj = result[i];
		const Urho3D::String& typeName = obj.object->GetTypeName();
		if(typeName == "StaticModel" || typeName == "AnimatedModel")
		{
			if(obj.object == EditorsRoot::Instance()->GetGizmo()->GetModel())
			{
				return true;
			}
		}
	}

	return false;
}

//1.��ѡģʽ ���ǵ�ѡ
//2.��ѡ�����ƶ�
void ObjectPositionEditor::OnMouseMove(float x,float y,unsigned int buttons)
{
	EditorsRoot* pEditorRoot = EditorsRoot::Instance();
	//��ʾ����UI
	char szName[256];

	//���ο�ѡģʽ
	if(isRectSelectionMode == true)
	{
		pEditorRoot->RectSelectionFrame_->UpdateShow(x,y);

		//���������ҵ����ڿ��ڵĶ���
		pEditorRoot->RectSelectionNodes = SceneHelper::Instance()->QueryScreenRectNodes(pEditorRoot->RectSelectionFrame_->vecStart,pEditorRoot->RectSelectionFrame_->vecEnd);

		sprintf(szName,"Selected:%d",pEditorRoot->RectSelectionNodes.size());
		pEditorRoot->ObjectNameTip_->SetText(szName);
		pEditorRoot->ObjectNameTip_->UpdateShow(x,y);
	}
	else	//��ѡ�����ƶ�
	{
		Editor3dGizmo* pGizmo = pEditorRoot->GetGizmo();
		pEditorRoot->ObjectNameTip_->Hide();

		IntVector2 pos(x,y);
		vector<SceneHitResult> result = SceneHelper::Instance()->QueryCurrentMousePosObjects(250.0,&pos);

		Node* pHoverNode = NULL;
		Drawable* selObject = get_edit_object(result);
		if(selObject != NULL)
		{
			pHoverNode = selObject->GetNode();
		}

		//��������������������������(�Ƿ��϶�����Ҫ)
		Graphics* graphics = GetSubsystem<Graphics>();
		Camera* camera = pEditorRoot->cameraNode_->GetComponent<Camera>();

		Ray cameraWorldRay = camera->GetScreenRay(x / graphics->GetWidth(),y / graphics->GetHeight());

		//�жϲ���¼��Щ�����ᱻѡ����
		bool bXHover;
		bool bYHover;
		bool bZHover;
		pGizmo->QuerySelectedStateByWorldRay(cameraWorldRay,bXHover,bYHover,bZHover);
		if(pGizmo->IsDrag == true)
		{
			pGizmo->UpdateMovePosition();
		}
		else
		{
			//��ǰ������������
			if(is_axis_object(result) == true)
			{
				pGizmo->gizmoAxisX->hovered = bXHover;
				pGizmo->gizmoAxisY->hovered = bYHover;
				pGizmo->gizmoAxisZ->hovered = bZHover;
				pGizmo->update_model();

				if(bXHover || bYHover || bZHover)
				{
					EditorsRoot::Instance()->GetMainWindow()->setCursor(Qt::SizeAllCursor);
				}
				else
				{
					EditorsRoot::Instance()->GetMainWindow()->setCursor(Qt::ArrowCursor);
				}
			}
			else
			{
				//�ж��Ƿ���ѡ�������
				if(selObject == NULL)
				{
					//�ǵ�ǰ�ɱ༭�Ķ���ֱ���˳�
					EditorsRoot::Instance()->GetMainWindow()->setCursor(Qt::ArrowCursor);
					return;
				}

				sprintf(szName,"%s - %s",result[0].object->GetTypeName().CString(),result[0].object->GetNode()->GetName().CString());
				pEditorRoot->ObjectNameTip_->SetText(szName);
				pEditorRoot->ObjectNameTip_->UpdateShow(x,y);

				//��ǰ��������ѡ���
				bool isHoverInSel = pEditorRoot->IsNodeInSelections(selObject->GetNode());

				//�����ڷǵ�ǰѡ��Ľڵ���ʱ����ʾ+
				if(isHoverInSel == false)
				{
					EditorsRoot::Instance()->GetMainWindow()->setCursor(Qt::CrossCursor);
				}

				if(isHoverInSel == false && selObject == NULL)
				{
					EditorsRoot::Instance()->GetMainWindow()->setCursor(Qt::ArrowCursor);
				}
			}
		}
	}
}

bool ObjectPositionEditor::IsEditObject(Drawable* obj)
{
	const Urho3D::String& typeName = obj->GetTypeName();
	if(typeName == "StaticModel" || typeName == "AnimatedModel")
	{
		if(obj != EditorsRoot::Instance()->GetGizmo()->GetModel())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool ObjectPositionEditor::OnKeyDown(unsigned short key)
{
	return true;
}

bool ObjectPositionEditor::OnKeyUp(unsigned short key)
{
	if(key == 46)
	{
		vector<Node*>& editNodes = EditorsRoot::Instance()->SelectionNodes;
		for(int i = 0;i < editNodes.size();i ++)
		{
			Node* pNode = editNodes[i];
			if(pNode->GetParent())
			{
				pNode->GetParent()->RemoveChild(pNode);
			}
		}
	}

	return true;
}

void ObjectPositionEditor::OnObjectSelect(Node* pNode)
{
	EditorsRoot* pEditorsRoot = EditorsRoot::Instance();
	if(pEditorsRoot->IsCtrlPressed() == false)
	{
		pEditorsRoot->CancelAllSelection();
	}
	
	//�ж���û��
	vector<Node*>& editNodes = pEditorsRoot->SelectionNodes;
	vector<Node*>::iterator it = editNodes.begin();
	while(it != editNodes.end())
	{
		if(*it == pNode)
		{
			editNodes.erase(it);
			return;
		}
		++ it;
	}

	editNodes.push_back(pNode);
}

void ObjectPositionEditor::OnNodesCut()
{
	OnNodesCopy();

	//todo:ɾ���ڵ�

}

void ObjectPositionEditor::OnNodesCopy()
{
	nodes_clipboard_data.clear();

	EditorsRoot* pEditorsRoot = EditorsRoot::Instance();
	const vector<Node*>& nodes = pEditorsRoot->SelectionNodes;
	for(int i = 0;i < nodes.size();i ++)
	{
		const Node* pNode = nodes[i];

		NodeData nodeData;
		nodeData.NodeID = pNode->GetID();
		nodeData.ParentNodeID = pNode->GetParent()->GetID();
		nodeData.xmlData = new XMLFile(pEditorsRoot->context_);
		pNode->SaveXML(nodeData.xmlData->CreateRoot("Node"));

		nodes_clipboard_data.push_back(nodeData);
	}
}

//ճ���Ժ�����岻�ͷ�
void ObjectPositionEditor::OnNodesPaste()
{
	EditorsRoot* pEditorsRoot = EditorsRoot::Instance();

	list<NodeData>::iterator it = nodes_clipboard_data.begin();
	while(it != nodes_clipboard_data.end())
	{
		NodeData& data = *it;

		Node* pParentNode = pEditorsRoot->scene_->GetNode(data.ParentNodeID);
		if(pParentNode != NULL)
		{
			Node* pNode = pParentNode->CreateChild();
			pNode->LoadXML(data.xmlData->GetRoot("Node"));
			pEditorsRoot->SendNodeUpdateShowEvent(pNode);
			//pNode->SetID(pEditorsRoot->scene_->GetFreeNodeID(CreateMode::REPLICATED));
			//pParentNode->AddChild(pNode);
			//pNode->SetName(String::EMPTY);

			//��Դ��һ��λ��
			Vector3 vOldPos = pNode->GetWorldPosition();
			vOldPos += Vector3(1,0,1);
			pNode->SetWorldPosition(vOldPos);
		}

		++ it;
	}
}
