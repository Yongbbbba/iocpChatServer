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
	// 클래스 이름 마다 소멸자 이름이 다르므로, Free로 통일 시키자
	virtual ~GameObject() 
	{
		this->Free();
	}
	virtual void Initialize() {};
	virtual void Free() {};
};