#include "stdafx.h"
#include "ShowHidePanel.h"

ShowHidePanel::ShowHidePanel(QWidget* parent)
{
	vLayout = new QVBoxLayout();
	setLayout(vLayout);

	vLayout->setSpacing(0);
	vLayout->setMargin(0);

	//Header
	QWidget* wHeader = new QWidget();
	QHBoxLayout* hHeader = new QHBoxLayout();
	wHeader->setLayout(hHeader);
	vLayout->addWidget(wHeader);

	lblButton_ = new QPushButton();
	lblButton_->setMaximumSize(20,20);
	lblButton_->setIcon(QIcon(":/Images/Show.png"));
	hHeader->addWidget(lblButton_);

	lblIcon_ = new QLabel();
	lblIcon_->setScaledContents(true);
	lblIcon_->setMaximumSize(20,20);
	hHeader->addWidget(lblIcon_);

	lblName_ = new QLabel();
	lblName_->setFont(QFont("Times",10,QFont::Bold));
	lblName_->setMaximumHeight(20);
	hHeader->addWidget(lblName_);

	//Frame
	frame_ = new QGroupBox();
	//frame_->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
	//frame_->setLineWidth(1);
	frameLayout_ = new QVBoxLayout();
	frame_->setLayout(frameLayout_);
	//frame_->setContentsMargins(0,0,0,0);
	vLayout->addWidget(frame_);

	Show();

	connect(lblButton_,SIGNAL(clicked(bool)),this,SLOT(buttonClicked(bool)));
}

QHBoxLayout* ShowHidePanel::getNewRow()
{
	QWidget* wRow = new QWidget();
	QHBoxLayout* layout = new QHBoxLayout();
	layout->setStretch(0, 2);
	layout->setSpacing(0);
	layout->setMargin(0);
	wRow->setLayout(layout);

	frameLayout_->addWidget(wRow);

	return layout;
}

void ShowHidePanel::buttonClicked(bool checked)
{
	if(frame_->parentWidget() != NULL)
	{
		Hide();
	}
	else
	{
		Show();
	}
}

void ShowHidePanel::SetIcon(const char* iconSrc)
{
	lblIcon_->setPixmap(QPixmap(iconSrc));
}

void ShowHidePanel::SetName(const char* name)
{
	lblName_->setText(name);
}

void ShowHidePanel::Show()
{
	lblButton_->setIcon(QIcon(":/Images/Hide.png"));
	vLayout->addWidget(frame_);
}

void ShowHidePanel::Hide()
{
	lblButton_->setIcon(QIcon(":/Images/Show.png"));
	frame_->setParent(NULL);
}
