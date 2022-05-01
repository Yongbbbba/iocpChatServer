#pragma once
#include "pch.h"

class Lock {
 private:
  lock_t mutex_;
  wstr_t name_;
  size_t lockId_;
  threadId_t threadId_;

  wstr_t lockingFile_;
  int lockingLine_;

 public:
  Lock(WCHAR* name);
  virtual ~Lock();

  const WCHAR* Name();
  size_t LockId();

  lock_t& Mutex();
  void FileLock(LPCWSTR fileName, int lineNo); // TODO 메소드명 재정의
  void FileUnlock();

  void SetThreadId(threadId_t id);
  threadId_t ThreadId();
};

class LockSafeScope {
  Lock* lock_;

 public:
  LockSafeScope(Lock* lock, LPCWSTR fileName, int lineNo);
  ~LockSafeScope();
};

#define SAFE_LOCK(lock) LockSafeScope __lockSafe(&lock, _W(__FILE__), __LINE__);

// 데드락 탐지용
class LockManager : public Singleton<LockManager> {
  size_t idSeed_;

 public:
  LockManager();

  Lock* SearchLockCycle(Lock* newLock);
  Lock* CheckDeadLock(Lock* lock);

  size_t GeneralId();
};
