#pragma once
#include <QMainWindow.h>
#include <qlistwidget.h>
#include <qtabwidget.h>
#include <qdockwidget.h>
#include "EditorWindow/SceneView.h"
#include "EditorWindow/ObjectPropertiesView.h"
#include "EditorWindow/ResExplorerView.h"
#include "OtherTools/ModelTool.h"
#include "EditorWindow/LogView.h"
#include "Object.h"
using namespace Urho3D;

class QAction;
class QTreeWidget;
class EditorSceneWidget;

/// �����ر�������ڣ�������Ŀ������Դ������Ⱦ���ڣ�������ڵ�
class EditorMainWindow : public QMainWindow , public Object
{
    Q_OBJECT;
	OBJECT(EditorMainWindow);
public:
    EditorMainWindow(QWidget *parent=0, Qt::WindowFlags flags =0);
    virtual ~EditorMainWindow();

	EditorSceneWidget* GetSceneWidget()
	{
		return pSceneWidget;
	}

	SceneView* GetProjectView()
	{
		return treeProject;
	}

	void AddLog(int type,const char* logContent);

	//���°�ť����ʾ״̬
	void updateActions();
	void updateUndoRedoActions();

	QDockWidget* GetEditorPanel()
	{
		return dockRight;
	}

	ObjectPropertiesView* GetPropertiesView()
	{
		return propertyView;
	}

	void setApplicationObject(QObject* obj);

	//public�Ĳ˵�Action�������Ҽ��˵�
	QAction* renameAction_;
	QAction* copyAction_;
	QAction* cutAction_;
	QAction* pasteAction_;
	QAction* deleteAction_;
private:
	void CreateActions();
    void CreateMenuBar();

	void CreateToolBars();
    void OnNewScene();

	//�ļ�������
	QToolBar* fileToolBar;
	QToolBar* editToolBar;

    QAction* openAction_;
    QAction* saveAction_;
	QAction* exitAction_;

	QAction* undoAction_;
	QAction* redoAction_;
	QAction* screenshot_;

	QAction* selectAction_;
	QAction* moveAction_;
	QAction* rotateAction_;

	QAction* modelTransAction_;

	QAction* helpAction_;
	QAction* aboutAction_;


	//Dock����
	QDockWidget* dockLeft;
	QDockWidget* dockRight;
	QDockWidget* dockBottom;
	QDockWidget* dockCenter;

	QPushButton* btnAttachTerrain;

	// ��Ŀ��ԴĿ¼����ŵ�ǰ������������Դ��Ŀ¼��
	SceneView* treeProject;

	ResExplorerView* resTreeView;
	// 
	ObjectPropertiesView* propertyView;

	QTabWidget* rightTabView;

	LogView* logView;

	// ������Ⱦ����
	EditorSceneWidget* pSceneWidget;

	ModelTool* mModelTool;

	bool eventFilter(QObject *obj,  QEvent *event);

	void HandleSelectionChanged(StringHash eventType, VariantMap& eventData);
private Q_SLOTS:
	void onModeChanged(int index);
	void HandleScreenshotAction();

	void HandleUndoAction();
	void HandleRedoAction();

	//ģ�͹���
	void HandleModelToolAction();

	void HandleSelectTool(bool checked);
	void HandleMoveTool(bool checked);
	void HandleRotateTool(bool checked);

	void HandleOpenAction();
	void HandleSaveAction();

	void HandleCutAction();
	void HandleCopyAction();
	void HandlePasteAction();
	void HandleDeleteAction();
	void HandleAttachTerrainAction(bool);
	void HandleTestEffectAction(bool);

	//�򿪹��ڶԻ���
	void HandleAbout();
};
