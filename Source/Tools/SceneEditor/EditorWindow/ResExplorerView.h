#pragma once
#include <QWidget>
#include <QTreeWidget>
#include <QEvent>

class ResExplorerView : public QTreeWidget
{
	Q_OBJECT
public:
	ResExplorerView(QWidget* parent = 0);

public Q_SLOTS:

private:
	QTreeWidgetItem* mModelsItem;	//ģ����Ŀ¼�ڵ�
};
