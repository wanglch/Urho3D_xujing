#pragma once
#include <QWidget>
#include <QTreeWidget>
#include <QEvent>
#include <QListWidget>
#include <QDirModel>

//Ŀ¼��Դ���
class ResExplorerView : public QWidget
{
	Q_OBJECT
public:
	ResExplorerView(QWidget* parent = 0);
	~ResExplorerView();

public Q_SLOTS:
	void onDirChanged();
private:
	QTreeWidget* mDirWidget;
	QListWidget* mFilesWidget;
};
