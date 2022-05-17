#pragma once

#include "pch.h"
#include <queue>
#include <stdexcept>

template<typename T>
class ThreadJobqueue
{
private:
	enum
	{
		WRITE_QUEUE,
		READ_QUEUE,
		MAX_QUEUE,
	};
	std::queue<T> mQueue[MAX_QUEUE];

	std::queue<T>* mWriteQueue; // input용, 여러 쓰레드에서 입력을 받음
	std::queue<T>* mReadqueue; // output용, 한 쓰레드에서 출력을 함

	Lock mLock;

public:
	ThreadJobqueue(const WCHAR* name) : mLock{ name }
	{
		mWriteQueue = &mQueue[WRITE_QUEUE];
		mReadqueue = &mQueue[READ_QUEUE];
	}
	~ThreadJobqueue()
	{
		mReadqueue->empty();
		mWriteQueue->empty();
	}

	inline void Push(const T& t)
	{
		SAFE_LOCK(mLock);
		mWriteQueue->push(t);
	}

	inline bool Pop(T& t)
	{
		SAFE_LOCK(mLock);
		size_t size = this->Size();
		if (size == 0)
		{
			return false;
		}
		if (mReadqueue->empty())
		{
			this->Swap();
		}
		t = mReadqueue->front();
		mReadqueue->pop();
		return true;
	}

	inline void Swap()
	{
		SAFE_LOCK(mLock);
		if (mWriteQueue == &mQueue[WRITE_QUEUE])
		{
			mWriteQueue = &mQueue[READ_QUEUE];
			mReadqueue = &mQueue[WRITE_QUEUE];
		}
		else
		{
			mWriteQueue = &mQueue[WRITE_QUEUE];
			mReadqueue = &mQueue[READ_QUEUE];
		}
	}

	inline bool IsEmpty() { return mReadqueue->empty(); }

	inline size_t Size()
	{
		SAFE_LOCK(mLock);
		size_t size = (size_t)(mQueue[WRITE_QUEUE].size() + mQueue[READ_QUEUE].size());
		return size;
	}
};

