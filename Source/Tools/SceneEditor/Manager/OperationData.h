#pragma once
#include "XMLFile.h"

enum enOpType
{
	enOpType_Add,
	enOpType_Delete,
	enOpType_Edit,
};

struct NodeData 
{
	int ParentNodeID;
	int NodeID;
	XMLFile* xmlData;

	NodeData()
	{
		ParentNodeID = 0;
		NodeID = 0;
		xmlData = NULL;
	}

	~NodeData()
	{
		if(xmlData)
		{
			//delete xmlData; todo:�ͷ�����
			xmlData = NULL;
		}
	}
};

//��¼�༭���еı༭����
class OperationData
{
public:
	OperationData()
	{
	}

	enOpType opType;

	//���������������������������
	list<NodeData> nodesData;
};
