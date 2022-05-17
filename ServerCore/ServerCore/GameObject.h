#pragma once
#include "pch.h"

class Object
{
private:
	wstr_t			mAllocFile;
	int				mAllocLine;
};

class NameObject
{
	wstr_t mName;
public:
	wstr_t& Name() 
	{
		return mName;
	}

	void SetName(wstr_t name)
	{
		mName = name;
	}
};

class Work
{
public:
	virtual void Tick() = 0;
	virtual void Wakeup() {};
	virtual void Suspend() {};
	virtual void Stop() {};
	virtual void Start() {};
};

class GameObject : public NameObject, public Work
{
	POINT		mPosition;
	float		mDirection;

public:
	// Ŭ���� �̸� ���� �Ҹ��� �̸��� �ٸ��Ƿ�, Free�� ���� ��Ű��
	virtual ~GameObject() 
	{
		this->Free();
	}
	virtual void Initialize() {};
	virtual void Free() {};
};