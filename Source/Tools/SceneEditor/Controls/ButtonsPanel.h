#pragma once
#include <vector>
#include <string>
using namespace std;
#include <qframe.h>
#include <qpushbutton.h>
#include <qscrollarea.h>
#include "EditTool.h"
#include <qboxlayout.h>
#include <QtGui/qsplitter.h>

class ButtonsPanel : public QWidget
{
	Q_OBJECT
public:
	ButtonsPanel(QWidget* parent = 0);

	void AddButton(const char* buttonName,EditTool* pPanel);

	void OnInitDialog();

	struct SButton
	{
		SButton()
		{
			pPanel = NULL;
			pButton = NULL;
		}

		string Name;
		EditTool* pPanel;
		QPushButton* pButton;
	};

private slots:
	void buttonClicked(bool checked = false);
private:
	vector<SButton> m_buttons;
	EditTool* m_pCurrentTool;	//当前展开的Panel
	QPushButton* m_pCurrentButton;
	QSplitter* vSplitter;
	QVBoxLayout* vBoxLayout;

	QFrame* m_ButtonPanel;
	QFrame* m_ContentPanel;

	void RecalcButtonsLayout();
};
