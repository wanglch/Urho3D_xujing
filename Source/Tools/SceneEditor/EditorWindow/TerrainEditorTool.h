#pragma once
#include <qobject.h>
#include <qpushbutton.h>
#include "../Controls/ButtonsPanel.h"

//���α༭��� Ӧ�ô�BaseEditor�̳У�
class TerrainEditorTool : public ButtonsPanel
{
	Q_OBJECT
public:
	TerrainEditorTool(QWidget* parent = 0);

};