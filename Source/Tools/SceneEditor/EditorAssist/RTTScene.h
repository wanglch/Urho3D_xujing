#pragma once
#include "Object.h"
#include "Image.h"

namespace Urho3D
{
	class Node;
	class Scene;
	class Context;
	class Image;
	class Texture2D;
	class Image;
}
using namespace Urho3D;

//һ���յĳ�������������RTT
class RTTScene : public Object
{
	OBJECT(RTTScene)
public:
	//����width,height��ʾ�����ͼƬ�ߴ�
	RTTScene(Context* context,int width = 1024,int height = 768);

	void CreateScene();

	void Clear();

	Node* CreateChildNode(const String& nodeName);

	Texture2D* GetResultTexture()
	{
		return renderTexture;
	}

	void SaveJpg(const char* pPath,int quality = 80);

	Node* GetCameraNode()
	{
		return rttCameraNode_;
	}
private:
	Scene* rttScene_;
	Node* rttCameraNode_;
	Texture2D* renderTexture;	//ʵʱ�õ���Texture
	
	Image* resultImage;

	int _width;
	int _height;
};
