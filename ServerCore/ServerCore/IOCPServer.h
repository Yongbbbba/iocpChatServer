#pragma once
#include "pch.h"
#include "Server.h"

#define MAX_IOCP_THREAD		SIZE_64

class IOCPServer : public Server, public Singleton<IOCPServer>
{
    SOCKET					listenSocket_;
    HANDLE					iocp_;
	Thread					*acceptThread_;
	array<Thread *, SIZE_64> workerThread_;

private:
	bool					CreateListenSocket();

    static DWORD WINAPI		AcceptThread(LPVOID serverPtr);
	static DWORD WINAPI		WorkerThread(LPVOID serverPtr);

public:
	IOCPServer(ContentsProcess *contentsProcess);
    virtual ~IOCPServer();

	bool					Run();

    SOCKET					ListenSocket();
    HANDLE					Iocp();
	void					OnAccept(SOCKET accepter, SOCKADDR_IN addrInfo);
};