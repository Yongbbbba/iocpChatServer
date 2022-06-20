// Ÿ ������Ʈ�� include �� �͵��� �̰��� ����
#pragma once

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#pragma comment(lib, "Winmm.lib")

#include <WS2tcpip.h>
#include <winsock2.h>
#include <MSWSock.h>
#include <mmsystem.h>
#include <WS2spi.h>
#include <mstcpip.h>

#include <Windows.h>
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
#include <typeinfo>

#include <array>

#if _DEBUG
#define CONTEXT_SWITCH Sleep(1)
#else
#define CONTEXT_SWITCH ::SwitchToThread()
#endif

// typedef void (*Function)(void *);
using Function = void(*)(void*);

//��Ÿ ��ƿ
#include "tinyxml.h"

// �ʼ� �������
#include "Types.h"
#include "Util.h"

//#include "GameObject.h"
#include "Singleton.h"
#include "RandomMt.h"

#include "Clock.h"
#include "Logger.h"
#include "Assert.h"

#include "Thread.h"
#include "Lock.h"

#include "Minidump.h"
#include "LowFragmentationHeap.h"

#include "Config.h"
#include "GameObject.h"
#include "Task.h"
#include "Monitoring.h"

// Network
#include "Server.h"
#include "Session.h"
#include "IOCPServer.h"
#include "IOCPSession.h"
#include "SessionManager.h"
#include "SessionMonitor.h"

// Terminal
#include "TerminalSession.h"
#include "Terminal.h"

// ��������
#include "Shutdown.h"
