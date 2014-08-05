#include "stdafx.h"
#include "ObjectPropertiesView.h"
#include "Skybox.h"

ObjectPropertiesView::ObjectPropertiesView(QWidget* parent)
{
	addComponentWidget_ = NULL;	//�Ӻ󴴽�

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
	positionWidget_ = new NodeWidget();
	lstWidges.push_back(positionWidget_);

	lightWidget_ = new LightWidget();
	lstWidges.push_back(lightWidget_);

	sceneWidget_ = new SceneWidget();
	lstWidges.push_back(sceneWidget_);

	skyboxWidget_ = new SkyboxWidget();
	lstWidges.push_back(skyboxWidget_);

	staticModelWidget_ = new StaticModelWidget();
	lstWidges.push_back(staticModelWidget_);

	animatedModelWidget_ = new AnimatedModelWidget();
	lstWidges.push_back(animatedModelWidget_);

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

void ObjectPropertiesView::SetEditNode(Node* pNode)
{
	fixWidget->setParent(NULL);

	descWidget_->SetText(pNode->GetName().CString());
	list<ShowHidePanel*>::iterator it = lstWidges.begin();
	while(it != lstWidges.end())
	{
		(*it)->setParent(NULL);
		++ it;
	}

	if(pNode == EditorRoot::Instance()->scene_)
	{
		sceneWidget_->Init(pNode);
		vContentLayout->addWidget(sceneWidget_);
		vContentLayout->addWidget(fixWidget);
		return;
	}
	
	//Position ÿ��Node����
	positionWidget_->Init(pNode);
	vContentLayout->addWidget(positionWidget_);

	if(pNode->GetComponent<Light>() != NULL)
	{
		lightWidget_->InitComponent(pNode->GetComponent<Light>());
		vContentLayout->addWidget(lightWidget_);
	}

	if(pNode->GetComponent<StaticModel>() != NULL)
	{
		staticModelWidget_->InitComponent(pNode->GetComponent<StaticModel>());
		vContentLayout->addWidget(staticModelWidget_);
	}

	if(pNode->GetComponent<AnimatedModel>() != NULL)
	{
		animatedModelWidget_->InitComponent(pNode->GetComponent<AnimatedModel>());
		vContentLayout->addWidget(animatedModelWidget_);
	}

	if(pNode->GetComponent<TerrainPatch>() != NULL)
	{
		terrainWidget_->InitComponent(pNode->GetComponent<TerrainPatch>());
		vContentLayout->addWidget(terrainWidget_);
	}

	if(pNode->GetComponent<Skybox>() != NULL)
	{
		skyboxWidget_->InitComponent(pNode->GetComponent<Skybox>());
		vContentLayout->addWidget(skyboxWidget_);
	}

	if(addComponentWidget_ == NULL)
	{
		addComponentWidget_ = new AddComponentWidget();
	}
	vContentLayout->addWidget(addComponentWidget_);

	vContentLayout->addWidget(btnAddComponent);

	vContentLayout->addWidget(fixWidget);
}
