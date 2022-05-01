#include "pch.h"
#include "Thread.h"
#include "Lock.h"

Thread::Thread(thread_t *thread, wstr_t name)
{
	name_ = name;
	thread_ = thread;
	id_ = thread_->get_id();

	ThreadManager::GetInstance().Put(this);
}

Thread::~Thread()
{
	thread_->join();
	SAFE_DELETE(thread_);
	SAFE_DELETE(lock_);
}

threadId_t Thread::Id()
{
	return id_;
}

wstr_t &Thread::Name()
{
	return name_;
}

void Thread::SetLock(Lock *lock)
{
	lock_ = lock;
}

Lock* Thread::GetLock()
{
	return lock_;
}

//--------------------------------------------------//
ThreadManager::~ThreadManager()
{
	for (auto thread : threadPool_){
		SAFE_DELETE(thread.second);
	}
}

void ThreadManager::Put(Thread *thread)
{
	std::pair<threadId_t, Thread *> node(thread->Id(), thread);
	threadPool_.insert(node);
	SLog(L"* create thread : id[0x%X] name[%s], pool size[%d]", thread->Id(), thread->Name().c_str(), threadPool_.size());
}

void ThreadManager::Remove(threadId_t id)
{
	auto iter = threadPool_.find(id);
	if (iter == threadPool_.end()) {
		return;
	}
	auto thread = iter->second;
	threadPool_.erase(iter);
}

Thread* ThreadManager::At(threadId_t id)
{
	if (threadPool_.empty()) {
		return nullptr;
	}
	auto iter = threadPool_.find(id);
	if (iter == threadPool_.end()) {
		return nullptr;
	}
	auto thread = iter->second;
	return thread;
}