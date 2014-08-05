#pragma once
#include <QWidget>
#include <QTreeWidget>
#include <QMenu>

#include "Object.h"

namespace Urho3D
{
	class Node;
}
using namespace Urho3D;

//�������ṹ�ؼ�
class SceneView : public QTreeWidget , public Object
{
	Q_OBJECT;
	OBJECT(SceneView);
public:
	SceneView(QWidget* parent,Context* context = NULL);
	void Init(Node* pSceneNode);
	void ClearAllSelection();
public Q_SLOTS:
	void selectionChanged();
	void itemDoubleClicked(QTreeWidgetItem *item, int column);
private:
	//��¼�����̶��Ľڵ�Node
	QTreeWidgetItem* sceneRootNode_;

	void on_node_select(Node* pNode);

	//�����ڵ��ҵ�Node
	Node* get_node_by_item(QTreeWidgetItem* item);
	//����Node�ҵ���Ӧ�����ڵ�
	QTreeWidgetItem* get_item_by_node(Node* pNode);

	void add_node_to_tree(QTreeWidgetItem* parent,Node* pNode);

	//����Node��ͼ����ʾ�����ر༭�����Ƿ�༭���ӽڵ㣬�綯����һ�����壬�ڲ��ڵ㳡���༭��������༭
	bool update_node_icon(QTreeWidgetItem* parent,Node* pNode);

	void HandleNodeAdded(StringHash eventType, VariantMap& eventData);
	void HandleNodeRemoved(StringHash eventType, VariantMap& eventData);
	void HandleNodeSelectionChange(StringHash eventType, VariantMap& eventData);
	void HandleNodeUpdateShow(StringHash eventType, VariantMap& eventData);
	void HandleSceneReset(StringHash eventType, VariantMap& eventData);

	bool bSelectTrigger;	//�������ѡ��״̬�Ƿ񴥷�����
protected:
	void mouseReleaseEvent(QMouseEvent *evt);
};
