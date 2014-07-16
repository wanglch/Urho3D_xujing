#pragma once
#include "CustomGeometry.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Vector2.h"
#include "Texture.h"
using namespace Urho3D;

namespace Urho3D
{
	class Terrain;
};

//��СΪ��λ1^��ͨ��Scale���ñ�ˢ��С
class TerrainBrush : public CustomGeometry
{
	OBJECT(TerrainBrush);
public:
	TerrainBrush(Context* context);
	~TerrainBrush();

	void SetOwner(Terrain* terrain)
	{
		owner_ = terrain;
	}

	void UpdatePos(const Vector3& worldPosition);

	void SetBrushSize(int brushSize);
	int GetBrushSize();

	void SetBrushTexture(Texture* texture);
private:
	float brushSize_;
	Vector3 currentPos;	//��ǰ�����������е�λ��
	Terrain* owner_;
	int pieces_;	//�����ж��ٸ����㣬Խ��Խϸ�壬���Ǽ���������
	float fPerSize;
	float fPerUv;
	int numVerts;
	float curPosHeight;

	void getVector(int x,int y,Vector3& position,Vector2& uv);
	void createGeometry();
};
