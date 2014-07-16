#pragma once
#include "Misc/Single.h"
#include "../EditorMainWindow.h"
#include "../Terrain/TerrainEditor.h"
#include "Context.h"
#include <string>
#include <map>

using namespace std;
#include "../EditorAssist/RTTScene.h"
#include "../EditorMainWindow.h"
#include "../EditorAssist/EditorGizmo.h"
#include "../ObjectEditor/ObjectPositionEditor.h"
#include "../EditorAssist/ObjectNameTip.h"
#include "../EditorAssist/RectSelectionFrame.h"

namespace Urho3D
{
	class Engine;
	class Node;
	class Scene;
	class Terrain;
	class IntVector2;
}

enum enEditorTools
{
	TOOL_SELECT = 0,
	TOOL_MOVE,
	TOOL_ROTATE,
	TOOL_SCALE,
	TOOL_DEFORM,
	TOOL_SMOOTH,
	TOOL_PAINT,	//ˢ�ر�
};

//�༭���ײ�����Ľӿڣ����������ʹ��
//�༭�����ʹ�ö�Ӧ���¼���������ע��
class EditorsRoot : public Singleton<EditorsRoot>
{
public:
	EditorsRoot();

	//����µ�ToolBar
	void RegisterToolBar(void* plugin,void * toolbar);

	//ע����Ӧ���¼�
	void RegisterMouseHandler(void* source);

	void InitEditors(Context* context);

	//��ʼ���༭��������Ҫ��UI
	void InitEditorUI(Context* context);

	Context* context_;

	TerrainEditor* GetTerrainEditor()
	{
		return terrainEditor_;
	}

	ObjectPositionEditor* GetObjectPositionEditor()
	{
		return objectEditor_;
	}

	IntVector2 GetScreenMousePos();

	//�����һЩȫ�ֱ���
	Engine* engine_;
	Scene* scene_;
	Node* cameraNode_;
	/// Camera yaw angle.
	float yaw_;
	/// Camera pitch angle.
	float pitch_;

	Node* terrainNode;
	Terrain* terrain_;
	UIElement* UIRoot_;
	ObjectNameTip* ObjectNameTip_;
	RectSelectionFrame* RectSelectionFrame_;

	//��ǰ��Ծ�ı༭��ʽ
	enEditorTools ActiveTool;

	EditorMainWindow* GetMainWindow()
	{
		return mainWindow_;
	}

	void SetMainWindow(EditorMainWindow* pWindow)
	{
		mainWindow_ = pWindow;
	}

	Editor3dGizmo* GetGizmo()
	{
		if(gizmo_ == NULL)
		{
			gizmo_ = new Editor3dGizmo(context_);
			gizmo_->CreateGizmo();
		}

		return gizmo_;
	}

	//����Node��������С����Camera��λ�۲�ʹ��
	float GetNodeSize(Node* pNode);

	void CancelAllSelection();

	//���ͽڵ�ѡ��仯��Ϣ
	void SendNodeSelectionChangeEvent();

	//ĳ�ڵ���ʾ����
	void SendNodeUpdateShowEvent(const Node* pNode);

	//����ѡ����
	void OnRectSelectionEnd();

	//Node�Ƿ���ѡ��Χ��
	bool IsNodeInSelections(const Node* pNode);

	//ɾ������ѡ��Ľڵ�
	void DeleteAllSelectionNodes();

	//����ѡ����������ر�
	void AttachSelectionsToTerrain();

	//���������Ч
	void AddEffetToSelectionNodes();

	vector<Node*> GetUnionSelections();

	RTTScene* GetRTTScene();
public:
	// �¼��ӿ�
	//1.�϶�
	bool OnDragEnter(void* source);
	bool OnDragMove(void* source,unsigned int modifier,int x,int y);
	void OnDragLeave();

	//2.����
	void OnKeyDown(unsigned int key);
	void OnKeyUp(unsigned int key);
	
	//3.Mouse
	void OnMouseMove(float x,float y,unsigned int buttons);
	void OnMouseLeftDown(float x,float y,unsigned int buttons);
	void OnMouseLeftUp(float x,float y,unsigned int buttons);
	void OnMouseRightDown(float x,float y,unsigned int buttons);
	void OnMouseRightUp(float x,float y,unsigned int buttons);
	void OnMouseMiddleDown(float x,float y,unsigned int buttons);
	void OnMouseMiddleUp(float x,float y,unsigned int buttons);
	void OnMouseWheel(float x,float y,unsigned int buttons);

	void OnNodeSelect(Node* pNode);

	//��ǰCtrl�Ƿ���
	bool IsCtrlPressed();

	enEditorTools mSelectedTool;

	vector<Node*> SelectionNodes;		//��ǰѡ�еĽڵ�
	vector<Node*> RectSelectionNodes;	//��ѡ�ڵ㼯��
private:
	BaseEditor* pCurrentEditor;
	//todo:�Ƿ����ͨ�û���
	TerrainEditor* terrainEditor_;
	ObjectPositionEditor* objectEditor_;

	Editor3dGizmo* gizmo_;
	EditorMainWindow* mainWindow_;

	RTTScene* rttScene_;
};
