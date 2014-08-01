#pragma once
#include "Ptr.h"
#include "Object.h"
using namespace Urho3D;
#include "../EditorInfo/BaseEditor.h"
#include "../EditorAssist/TerrainBrush.h"
#include <vector>
using namespace std;

enum EditMode
{
	EM_NONE = 0,	//û���κα༭
	EM_DEFORM,		//�������ߣ�����
	EM_SPLAT,		//ˢ�ر���ͼ
	EM_PAINT,		//����ɫ��ռ��һ����ͼ todo,��ʱ��֧��
	EM_SMOOTH,		//ƽ���ر�
	EM_SPLATGRASS	//ˢ��
};

namespace Urho3D
{
	class Context;
	class Engine;

	class Sprite;
	class Node;
	class Scene;
	class Terrain;
}

//���α༭��
class TerrainEditor : public BaseEditor,public Object
{
	OBJECT(TerrainEditor)
public:
	TerrainEditor(Context* context);

	virtual void OnMouseMove(float x,float y,unsigned int buttons);
	virtual void OnMouseLeftDown(float x,float y,unsigned int buttons);

	virtual void OnKeyDown(unsigned int key);
	virtual void OnKeyUp(unsigned int key);

	void startEdit();
	void stopEdit();

	//���õ�ǰ�༭ģʽ
	void setEditMode(unsigned int mode);

	TerrainBrush* GetBrush()
	{
		return brush_;
	}

	Node* add_mesh_test(const char* name,float x,float y,float z,const char* modelUrl);
private:
	EditMode mEditMode;		//��ǰ�༭ģʽ
	float mBrushIntensity;	//��ˢǿ��

	Node* brushNode_;
	TerrainBrush* brush_;

	//��
	void on_viewport_click(float x,float y);

	void on_terrain_click(Vector3& worldPos);

	void test_2d_scene();
	Node* tiledNode;

	int modIndex;
};
