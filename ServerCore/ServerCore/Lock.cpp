#include "pch.h"
#include "Lock.h"
#include "Thread.h"

#ifdef _DEBUG
#define STERN_MODE
#endif

const int INVALID_LINE = -1;

Lock::Lock(WCHAR *name)
{
	lockId_ = LockManager::GetInstance().GeneralId();
	name_ = name;

	lockingFile_.clear();
	lockingLine_ = INVALID_LINE;
}

Lock::~Lock()
{
	name_.clear();
}

const WCHAR* Lock::Name() 
{
	return name_.c_str();
}

size_t Lock::LockId()
{
	return lockId_;
}

lock_t& Lock::Mutex()
{
	return mutex_;
}

void Lock::FileLock(LPCWSTR fileName, int lineNo)
{
	mutex_.lock();

	lockingFile_ = fileName;
	lockingLine_ = lineNo;
}

void Lock::FileUnlock()
{
	mutex_.unlock();

	lockingFile_.clear();
	lockingLine_ = INVALID_LINE;
}

void Lock::SetThreadId(threadId_t id)
{
	threadId_ = id;
}

threadId_t Lock::ThreadId()
{
	return threadId_;
}
//-------------------------------------------------------//
// �����ڿ��� ���� �ɰ�, ������ ���������� ��ü ������ ���� Ǭ��.
// std::lock_guard<lock_t> guard(lock) �� ����������, 
// ����� ������ ���� ���� �����Ѵ�.
LockSafeScope::LockSafeScope(Lock *lock, LPCWSTR fileName, int lineNo)
{
	if (lock == nullptr) {
		return;
	}
	//if (_shutdown == true) {
	//	return;
	//}

	lock_ = lock;
	Lock *deadLock = LockManager::GetInstance().CheckDeadLock(lock_);
	if (deadLock != nullptr) {
#ifdef STERN_MODE
		SErrLog(L"! [%s]lock and [%s]lock is dead detecting!", deadLock->Name(), lock->Name());
#else
		std::lock(lock_->mutex(), deadLock->mutex());
		SLog(L"!!! [%s]lock and [%s]lock is dead detecting!", deadLock->name(), lock->name());
#endif
		return;
	}

	lock_->FileLock(fileName, lineNo);
	lock->SetThreadId(GET_CURRENT_THREAD_ID());
}

LockSafeScope::~LockSafeScope()
{
	if (!lock_) {
		return;
	}
	/*if (_shutdown == true) {
		return;
	}*/
	lock_->FileUnlock();
	//lock_->setThreadId(nullptr);
}

//-------------------------------------------------------//
LockManager::LockManager()
{
	idSeed_ = 0;
}

/*
http://www.npteam.net/849
http://www.boost.org/doc/libs/1_46_1/libs/graph/doc/astar_visitor.html
http://includestdio.tistory.com/m/post/12
http://www.bogotobogo.com/cplusplus/multithreaded4_cplusplus11B.php#nested_lock
���� �ڽ��� �ɸ� threadId��,
Thread������ �ɰ��ִ� Lock ��ȣ�� ����.
�̵��� ���� ���ٰ� �ڽ��� �ɷ��� lock�� ������ lock ��ȯ
�� ����� ó��.
*/
Lock* LockManager::SearchLockCycle(Lock *newLock)
{
	//list ���� lock �̸��� ���� ����.
	Thread *thread = ThreadManager::GetInstance().At(GET_CURRENT_THREAD_ID());
	if (!thread) {
		return nullptr;
	}
	std::vector<Lock *> trace;		// ����� Ž����, �ɸ� �� stact ��ô
	trace.push_back(newLock);

	Lock *deadLock = nullptr;
	while (true) {
		Lock *threadLock = thread->GetLock();
		if (threadLock == nullptr) {
			break;
		}
		if (threadLock->LockId() == trace[0]->LockId()) {
			deadLock = threadLock;
			break;
		}
		trace.push_back(threadLock);
		thread = ThreadManager::GetInstance().At(threadLock->ThreadId());
		if (!thread) {
			break;
		}
	}
	trace.empty();
	return deadLock;
}

Lock* LockManager::CheckDeadLock(Lock *newLock)
{
	Lock *deadLock = this->SearchLockCycle(newLock);
	if (deadLock) {
		return deadLock;
	}

	return nullptr;
}

size_t LockManager::GeneralId()
{
	size_t id = idSeed_++;
	return id;
}
