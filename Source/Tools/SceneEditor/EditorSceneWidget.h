#pragma once
#include <QWidget>
#include <QEvent>
#include <Windows.h>

//������Ⱦ����
class EditorSceneWidget : public QWidget
{
   Q_OBJECT

public:
   EditorSceneWidget(QWidget* parent = 0, Qt::WindowFlags flags = 0);

   void showObjectMenu();
private:
	bool m_Captured;
	POINT m_savedpos;	//CatureMouseʱ��������λ��
	int m_oldx;	//MouseMoveʱ�ɵ��������
	int m_oldy;

	bool MouseMovedSincePress;	//��갴�º��Ƿ��ƶ���
protected:
	void dragEnterEvent(QDragEnterEvent* event);
	void dropEvent(QDropEvent *event);

	void mouseMoveEvent(QMouseEvent *evt);
	void mousePressEvent(QMouseEvent* evt);
	void mouseReleaseEvent(QMouseEvent *evt);
	void leaveEvent(QEvent *evt);
	void wheelEvent(QWheelEvent *evt);

	void keyPressEvent(QKeyEvent *evt);

	//Qt����굽Urho��ת��
	unsigned int getMouseButton(Qt::MouseButtons buttons,Qt::MouseButton button);
};
