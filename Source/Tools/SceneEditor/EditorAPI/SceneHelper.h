#pragma once
#include "../Single.h"
#include <vector>
using namespace std;
#include "Vector3.h"
#include "Drawable.h"
#include "Object.h"
#include "Node.h"
using namespace Urho3D;

struct SceneHitResult 
{
	Drawable* object;
	Vector3 hitPos;
};

class SceneHelper : public Singleton<SceneHelper>,public Object
{
	OBJECT(SceneHelper)
public:
	SceneHelper();

	void Init2DScene();

	//���ص�ǰ�����ѯ�����Ķ����б�
	vector<SceneHitResult> QueryCurrentMousePosObjects(float maxDistance,IntVector2* screenPos = NULL);

	//��ѯ��Ļ��������Ķ�Ӧ��Node(��������п��ӻ�����)
	vector<Node*> QueryScreenRectNodes(const Vector2& vecStart,const Vector2& vecEnd);

	Scene* scene2D;

	void HandleUpdate(StringHash eventType, VariantMap& eventData);

	Vector2 GetScreenPos(const Vector2& viewPos);
	Vector2 GetViewPos(const Vector2& screenPos);
private:
	//�ݹ麯�����жϽڵ��Ƿ�����������
	void query_node_in_rect(Node* node,vector<Node*>& outNodes,const Vector2& vecStart,const Vector2& vecEnd);
};
