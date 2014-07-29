#pragma once

#ifndef _WIN32_WINNT            // ָ��Ҫ������ƽ̨�� Windows Vista
#define _WIN32_WINNT 0x0600     // ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#include "glew.h"

//Urho3d��һЩ����ͷ�ļ�
#include "Graphics.h"
#include "Context.h"
#include "Engine.h"
#include "Renderer.h"
#include "Font.h"
#include "Node.h"
#include "Octree.h"
#include "CoreEvents.h"
#include "Model.h"
#include "Camera.h"
#include "Input.h"
#include "IOEvents.h"
#include "Material.h"
#include "ResourceCache.h"
#include "Log.h"
#include "Image.h"
#include "FileSystem.h"

#include "DrawableEvents.h"
#include "VertexBuffer.h"
#include "Geometry.h"
#include "Color.h"

#include "UI.h"
#include "UIEvents.h"
#include "Text.h"
#include "Sprite.h"
#include "Texture2D.h"
#include "Zone.h"
#include "Graphics.h"

#include "Technique.h"
#include "Terrain.h"
#include "TerrainPatch.h"
#include "Scene.h"
#include "StaticModel.h"
#include "AnimatedModel.h"

#include "PhysicsWorld.h"
#include "RigidBody.h"
#include "CollisionShape.h"

#include "DecalSet.h"
using namespace Urho3D;

//Qt����ͷ�ļ�
#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QMenuBar.h>
#include <QMenu.h>
#include <QToolBox>
#include <QStatusBar>
#include <QTreeWidget>
#include <QComboBox>
#include <QBoxLayout>
#include <QDockWidget>
#include <QList>
#include <QUrl>
#include <QStatusBar>
#include <QGroupBox>
#include <QSlider>
#include <QLabel>
#include <QSpinBox>
#include <QToolBar>
