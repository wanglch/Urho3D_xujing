#pragma once
#include <QObject>
#include <QPushButton>

#include "../Controls/ButtonsPanel.h"

//���α༭��� Ӧ�ô�BaseEditor�̳У�
class TerrainEditorTool : public ButtonsPanel
{
	Q_OBJECT
public:
	TerrainEditorTool(QWidget* parent = 0);

};