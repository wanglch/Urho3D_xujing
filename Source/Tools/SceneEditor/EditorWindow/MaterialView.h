#pragma once
#include <QWidget>
#include <QTreeWidget>
#include <QDialog>

//�����ĳ��Index ��Materialʱ��������壬ѡ��Material
class MaterialView : public QDialog
{
	Q_OBJECT;
public:
	MaterialView();
	~MaterialView();

	//��ʼ�������б�
	void prepareView();
private slots:
	//Tree��ѡ���˲�ͬ�Ĳ���
	void selectionChanged();
private:
	QTreeWidget* materialWidget_;
};
