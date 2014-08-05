#pragma once
#include "Object.h"
#include "Node.h"
using namespace Urho3D;
#include "../EditorInfo/BaseEditor.h"
#include <vector>
using namespace std;
#include "../EditorAPI/SceneHelper.h"
#include "../EditorAPI/OperationData.h"

/*
���������ʵ���϶�������㷨��
���ʱ����¼���Ray��Axis��ͶӰ�㵽Axisԭ��ľ��룬֮�����Ray���ϱ仯�������������Ҳ�ڱ仯�����������������������ἰ��ӦObject������
*/

//����ƶ������ŵȲ���
class ObjectPositionEditor : public BaseEditor,public Object
{
	OBJECT(ObjectPositionEditor)
public:
	ObjectPositionEditor(Context* context);

	void OnMouseLeftDown(float x,float y,unsigned int buttons);
	void OnMouseLeftUp(float x,float y,unsigned int buttons);

	void OnMouseMove(float x,float y,unsigned int buttons);

	bool OnKeyDown(unsigned short key);
	bool OnKeyUp(unsigned short key);

	void OnObjectSelect(Node* pNode);

	void OnNodesCut();

	//����
	void OnNodesCopy();

	//ճ��
	void OnNodesPaste();

	//�Ƿ�Ϊ���༭���༭�Ķ���
	static bool IsEditObject(Drawable* obj);
private:
	void HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData);

	Drawable* get_edit_object(const vector<SceneHitResult>& result);
	bool is_axis_object(const vector<SceneHitResult>& result);

	bool isRectSelectionMode;
	Vector2 rectSelectionStart;

	Drawable* CurrentHoverObject;
	list<NodeData> nodes_clipboard_data;	//���ƵĽڵ�����
};
