#pragma once
#include "pch.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#pragma comment(lib, "Winmm.lib")

#include <Ws2tcpip.h>
#include <winsock2.h>
#include <mswsock.h>
#include <Mmsystem.h>
#include <Ws2spi.h>
#include <Mstcpip.h>

#include <windows.h>
#include <iostream>
#include <io.h>
#include <cstdlib>
#include <stdio.h>
#include <cstdint>

#include <assert.h> 
#include <fcntl.h>
#include <algorithm>  
#include <functional>

#include <thread>
#include <mutex>
#include <memory>

#include <string>
#include <tchar.h>

#include <ctime>
#include <random>
#include <typeinfo>    //typeinfo
// TODO: 공용 매크로
//#define CONTEXT_SWITCH     std::this_thread::sleep_for(std::chrono::nanoseconds(1))
#if _DEBUG
#define CONTEXT_SWITCH		Sleep(1)
#else
#define CONTEXT_SWITCH		::SwitchToThread()
#endif

typedef void(*Function)(void*);

#include "Type.h"
#include "Util.h"
#include "Singleton.h"
#include "RandomMT.h"
#include "Clock.h"
/*
//기타 유틸
#include "./Util/csv_parser/csv_parser.hpp"
#include "./Util/tinyXml/tinyxml.h"

// TODO: 필수 헤더 파일
//-------------------------------------------------------------------//
#include "WinSocket.h"
#include "Type.h"
#include "Util.h"

#include "GameObject.h"
#include "Singleton.h"
#include "RandomMT.h"

#include "Clock.h"
#include "Logger.h"
#include "Assert.h"

#include "Table.h"
#include "Thread.h"
#include "Lock.h"
#include "ThreadJobQueue.h"
#include "Task.h"

#include "MemoryLeak.h"
#include "Memory_LowFragmentationHeap.h"
#include "Minidump.h"

#include "SMTPMail.h"
#include "Config.h"
#include "Monitoring.h"
#include "ProgramValidation.h"

//패킷들
#include "Stream.h"
#include "PacketHeader.h"
#include "PacketClass.h"
#include "PacketAnalyzer.h"
#include "PacketFactory.h"
#include "Package.h"
#include "PacketObfuscation.h"

//컨텐츠 관련
#include "ContentsProcess.h"

//서버
#include "Session.h"
#include "SessionManager.h"
#include "Server.h"

#include "IOCPServer.h"
#include "IOCPSession.h"
#include "SessionMonitor.h"

//터미널
#include "TerminalSession.h"
#include "TerminalTerminal.h"
#include "TerminalManager.h"

//DB
#include "ADODatabase.h"
#include "Query.h"
#include "DBManager.h"

// 전역 변수
#include "Shutdown.h"
*/

