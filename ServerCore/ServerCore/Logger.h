#pragma once
#include <fstream>

#include "Singleton.h"
#include "Types.h"
#include "pch.h"

#define SLog(arg, ...) SystemLog::GetInstance().Log(arg, __VA_ARGS__);
#define SErrLog(arg, ...)                         \
  SystemLog::GetInstance().Log(arg, __VA_ARGS__); \
  ::ExitProcess(0);

//-----------------------------------------------------------------//
class LogBase {
 public:
  LogBase() {}
  virtual ~LogBase() {}
  virtual void Initialize() {}
  virtual void UnInitialize() {}
  virtual void Log(const WCHAR *logStr) = 0;
};

//-----------------------------------------------------------------//
class LogPrintf : public LogBase {
 public:
  LogPrintf();
  void Log(const WCHAR *logStr);
};

//-----------------------------------------------------------------//
class LogFile : public LogBase {
  std::wfstream fs_;
  wstr_t fileName_;

 public:
  LogFile(xml_t *config);
  virtual ~LogFile();

  void Initialize() {}
  void Initialize(const WCHAR *logFileName);
  void Log(const WCHAR *logStr);
};
//-----------------------------------------------------------------//
// 로그 쓰는 주체
//-----------------------------------------------------------------//
class LogWriter {
 private:
  LogBase *base_;
  wstr_t prefix_;

 public:
  LogWriter();
  virtual ~LogWriter();

  void SetLogger(LogBase *base, const WCHAR *logPrefix);
  LogBase *Logger();

  void Log(const WCHAR *fmt, ...);
  void Log(const WCHAR *fmt, va_list args);
};
typedef LogWriter *LogWriterPtr;

//-----------------------------------------------------------------//
// 어플 시스템 로그
//-----------------------------------------------------------------//
class SystemLog : public Singleton<SystemLog> {
 private:
  LogWriter logWrite_;

 public:
  SystemLog();
  virtual ~SystemLog();

  void Initialize(xml_t *config);
  void Log(const WCHAR *fmt, ...);
};
