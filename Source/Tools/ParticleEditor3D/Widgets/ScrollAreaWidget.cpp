#include "stdafx.h"
#include "ScrollAreaWidget.h"
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>

ScrollAreaWidget::ScrollAreaWidget()
{
    setMinimumWidth(320);
    setWidgetResizable(true);

    QWidget* widget = new QWidget();
    setWidget(widget);

    vBoxLayout_ = new QVBoxLayout();
    widget->setLayout(vBoxLayout_);
}

ScrollAreaWidget::~ScrollAreaWidget()
{
}

QHBoxLayout* ScrollAreaWidget::AddHBoxLayout()
{
	QHBoxLayout* hBoxLayout = new QHBoxLayout();
	vBoxLayout_->addLayout(hBoxLayout);

    hBoxLayout->setContentsMargins(0, 0, 0, 0);
	return hBoxLayout;
}
