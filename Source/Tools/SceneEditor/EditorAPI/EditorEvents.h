#pragma once
#include "Object.h"

//ѡ�����ݷ����仯
EVENT(E_NODE_SELECTION_CHANGE,NodeSelectionChange)
{
}

//Node����ʾ�����仯
EVENT(E_NODE_UPDATE_SHOW,NodeUpdateShow)
{
	PARAM(P_NODE,Node);
}
