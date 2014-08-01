#include "stdafx.h"
#include "ObjectPropertiesView.h"
#include "Skybox.h"

ObjectPropertiesView::ObjectPropertiesView(QWidget* parent)
{
	QVBoxLayout* layout = new QVBoxLayout();
	layout->setAlignment(Qt::AlignTop);
	layout->setSpacing(0);
	layout->setMargin(0);
	setLayout(layout);

	setMinimumWidth(270);

	//HeaderһЩ����
	descWidget_ = new LabelTextBox();
	descWidget_->txtValue->setFixedWidth(150);

	layout->addWidget(descWidget_);
	
	wContent = new QWidget();
	vContentLayout = new QVBoxLayout();
	wContent->setLayout(vContentLayout);

	QScrollArea* scroll = new QScrollArea();
	scroll->setWidgetResizable(true);
	//scroll->setBackgroundRole(QPalette::Dark);// ���ù�������ı��� �ᵼ��������ı���ɫ�޷�����
	scroll->setWidget(wContent);

	//�������
	positionWidget_ = new PositonEditorComponent();
	positionWidget_->Show();	//Ĭ�ϴ�
	lstWidges.push_back(positionWidget_);

	lightWidget_ = new LightWidget();
	lstWidges.push_back(lightWidget_);

	sceneWidget_ = new SceneWidget();
	lstWidges.push_back(sceneWidget_);

	skyboxWidget_ = new SkyboxWidget();
	lstWidges.push_back(skyboxWidget_);

	staticModelWidget_ = new StaticModelWidget();
	lstWidges.push_back(staticModelWidget_);

	terrainWidget_ = new TerrainWidget();
	lstWidges.push_back(terrainWidget_);

	btnAddComponent = new QPushButton("Add Component");
	//lstWidges.push_back(btnAddComponent);

	layout->addWidget(scroll);

	//ʼ�ջ�ռ������Ŀռ䣬�����пؼ������ú���Ӹÿؼ���ʵ�ִ��ϵ��½ڴղ���
	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	fixWidget = new QWidget();
	fixWidget->setSizePolicy(sizePolicy);

	showMinimized();
}

void ObjectPropertiesView::SetTarget(Node* pTarget)
{
	fixWidget->setParent(NULL);

	descWidget_->SetText(pTarget->GetName().CString());
	list<ShowHidePanel*>::iterator it = lstWidges.begin();
	while(it != lstWidges.end())
	{
		(*it)->setParent(NULL);
		++ it;
	}

	if(pTarget == EditorRoot::Instance()->scene_)
	{
		sceneWidget_->Init(pTarget);
		vContentLayout->addWidget(sceneWidget_);
		vContentLayout->addWidget(fixWidget);
		return;
	}
	
	//Position ÿ��Node����
	positionWidget_->Init(pTarget);
	vContentLayout->addWidget(positionWidget_);

	if(pTarget->GetComponent<Light>() != NULL)
	{
		lightWidget_->Init(pTarget);
		vContentLayout->addWidget(lightWidget_);
	}

	if(pTarget->GetComponent<StaticModel>() != NULL)
	{
		staticModelWidget_->Init(pTarget);
		vContentLayout->addWidget(staticModelWidget_);
	}

	if(pTarget->GetComponent<TerrainPatch>() != NULL)
	{
		terrainWidget_->Init(pTarget);
		vContentLayout->addWidget(terrainWidget_);
	}

	if(pTarget->GetComponent<Skybox>() != NULL)
	{
		skyboxWidget_->Init(pTarget);
		vContentLayout->addWidget(skyboxWidget_);
	}

	vContentLayout->addWidget(btnAddComponent);

	vContentLayout->addWidget(fixWidget);
}
