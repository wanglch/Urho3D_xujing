/*
˵����
	���еı༭��������Щ��̳�
	�����༭����Ӧ�¼�����ʱ��������֪ͨ����ע���˸��¼��ı༭��
*/
#pragma once

class MouseListener
{
public:
	MouseListener() {};
	virtual ~MouseListener() {};

	virtual void OnMouseMove(float x,float y,unsigned int buttons)
	{

	}

	virtual void OnMouseLeave(float x,float y,unsigned int buttons)
	{

	}

	//��갴��ʱֻ�ᴥ��һ��
	virtual void OnMouseLeftDown(float x,float y,unsigned int buttons)
	{

	}

	virtual void OnMouseLeftUp(float x,float y,unsigned int buttons)
	{

	}

	virtual void OnMouseRightDown(float x,float y,unsigned int buttons)
	{

	}

	virtual void OnMouseRightUp(float x,float y,unsigned int buttons)
	{

	}

	virtual void OnMouseMiddleDown(float x,float y,unsigned int buttons)
	{

	}

	virtual void OnMouseMiddleUp(float x,float y,unsigned int buttons)
	{

	}

	virtual void OnMouseWheel(float x,float y,unsigned int buttons)
	{

	}
};

class KeyboardListener
{
public:
	KeyboardListener() {};
	virtual ~KeyboardListener() {};

	virtual bool OnKeyDown(unsigned short key)
	{

		return true;
	}

	virtual bool OnKeyUp(unsigned short key)
	{
		return true;
	}
};

class DragDropHandler
{
public:
	DragDropHandler() {};
	virtual ~DragDropHandler() {};

	virtual bool OnDragEnter () = 0;
	virtual void OnDragLeave () = 0;
	virtual bool OnDragMove (float x,float y,unsigned int buttons) = 0;
	virtual void OnDragWheel (float x,float y,unsigned int buttons, float delta) = 0;
	virtual void OnDragDropped (float x,float y,unsigned int buttons) = 0;
};
