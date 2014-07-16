#pragma once
#include "Node.h"
#include <list>
using namespace std;
using namespace Urho3D;

//�ڲ����ݱ䶯Ӧ�ò����仯�б��������ⲿ����

enum enChangeType
{
	enChangeType_MoveTo,	//һ��Ԫ���ƶ�����һ��

	enChangeType_Dispear,	//��ʧ
	enChangeType_AddNew,	//�����µ�
};

struct ChangeData
{
	ChangeData()
	{
		memset(this,0,sizeof(*this));
	}

	enChangeType cType;
	Node* pNode;
	int Frames;	//����֡���

	//Move ��ʾ�ӵ�ǰ�ƶ���x,y
	int Param1;
	int Param2;
};

struct Pos
{
	int X;
	int Y;
};

//�仯�Ķ�������,�����е�Ч���ǰ�˳��ִ�е�
typedef list<ChangeData> CHANGE_ORDER;

#define out

class DataGrid
{
public:
	DataGrid();
	~DataGrid();

	//������һ�������������ʧ�ܣ���Ϸ����
	bool GenNext(list<CHANGE_ORDER>& outChange);

	//�������²���,�����Ƿ����仯
	bool OnLeft(list<CHANGE_ORDER>& outChange);
	bool OnRight(list<CHANGE_ORDER>& outChange);
	bool OnUp(list<CHANGE_ORDER>& outChange);
	bool OnDown(list<CHANGE_ORDER>& outChange);

	void OnNewGame(list<CHANGE_ORDER>& outChange);

	bool CheckEnd();

	//��ֵΪ0
	int Score;	//��ǰ����
	int data[4][4];	//y,x
private:
	bool IsChanged;
	void MergeRow(int ColIndex,int fromRow,int toRow);
	void MergeCol(int RowIndex,int fromCol,int toCol);

	void PushToVSide(int yFrom,int yTo);
	void PushToHSide(int xFrom,int xTo);
};
