#pragma once
#include "../../Controls/EditTool.h"

//ˢ�ر���ͼ
class TerrainTexturePanel : public EditTool
{
	Q_OBJECT
public:
	TerrainTexturePanel(QWidget* parent = 0);

	virtual void BeginEditParams(Node* pNode);
	virtual void EndEditParams();

};