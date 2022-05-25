#include "pch.h"
#include "IOCPServer.h"
#include "IOCPSession.h"

IOCPServer::IOCPServer(ContentsProcess *contentsProcess)
:Server(contentsProcess)
{
}

IOCPServer::~IOCPServer()
{
	::closesocket(listenSocket_);
}

bool IOCPServer::CreateListenSocket()
{
    listenSocket_ = WSASocket(AF_INET, SOCK_STREAM, NULL, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (listenSocket_ == INVALID_SOCKET) {
        SErrLog(L"! listenSocket fail");
        return false;
    }

    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons((u_short)port_);
	inet_pton(AF_INET, ip_, &(serverAddr.sin_addr));

	int reUseAddr = 1; // 재기동시 유용
	setsockopt(listenSocket_, SOL_SOCKET, SO_REUSEADDR, (char *)&reUseAddr, (int)sizeof(reUseAddr));

    int retval = ::bind(listenSocket_, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
    if (retval == SOCKET_ERROR) {
        SErrLog(L"! bind fail");
        return false;
    }

    const int BACK_SOCKETS = 5;
    retval = ::listen(listenSocket_, BACK_SOCKETS);
    if (retval == SOCKET_ERROR) {
        SErrLog(L"! listen fail");
        return false;
    }

	array<char, SIZE_64> ip;
	inet_ntop(AF_INET, &(serverAddr.sin_addr), ip.data(), ip.size());
	SLog(L"* server listen socket created, ip: %S, port: %d", ip.data(), port_);
    return true;
}

bool IOCPServer::Run()
{
	if (MAX_IOCP_THREAD < workerThreadCount_) {
		SErrLog(L"! workerThread limit[%d], but config setting [%d]", MAX_IOCP_THREAD, workerThreadCount_);
		return false;
	}

	iocp_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, workerThreadCount_);
	if (iocp_ == nullptr) {
		return false;
	}
	this->CreateListenSocket();
	
	acceptThread_ = MAKE_THREAD(IOCPServer, AcceptThread);
	for (int i = 0; i < workerThreadCount_; ++i) {
		workerThread_[i] = MAKE_THREAD(IOCPServer, WorkerThread);
	}
	this->status_ = SERVER_READY;

	while (!_shutdown) {
		wstring cmdLine;
		std::getline(std::wcin, cmdLine);

		SLog(L"Input was: %s", cmdLine.c_str());
		_session_manager.RunCommand(cmdLine);
	}
	return true;
}

SOCKET IOCPServer::ListenSocket()
{
    return listenSocket_;
}

HANDLE IOCPServer::Iocp()
{
    return iocp_;
}

void IOCPServer::OnAccept(SOCKET accepter, SOCKADDR_IN addrInfo)
{
    IOCPSession *session = new IOCPSession();
	if (session == nullptr) {
		SLog(L"! accept session create fail");
		return;
	}
	if (!session->OnAccept(accepter, addrInfo)) {
		SAFE_DELETE(session);
		return;
	}
	if (!_session_manager.AddSession(session)) {
		SAFE_DELETE(session);
		return;
	}
	session->ioData_[IO_READ].clear();

	HANDLE handle = CreateIoCompletionPort((HANDLE)accepter, this->Iocp(), (ULONG_PTR)&(*session), NULL);
	if (!handle) {
		SAFE_DELETE(session);
		return;
	}
    
    SLog(L"* client accecpt from [%s]", session->ClientAddress().c_str());
	session->RecvStandBy();
}

DWORD WINAPI IOCPServer::AcceptThread(LPVOID serverPtr)
{
	IOCPServer	*server = (IOCPServer *)serverPtr;
	
    while (!_shutdown) {
		SOCKET		acceptSocket = INVALID_SOCKET;
		SOCKADDR_IN recvAddr;
		static int addrLen = sizeof(recvAddr);
		acceptSocket = WSAAccept(server->ListenSocket(), (struct sockaddr *)&recvAddr, &addrLen, NULL, 0);
		if (acceptSocket == SOCKET_ERROR) {
			if (!server->Status() == SERVER_STOP) {
				SLog(L"! Accept fail");
				break;
			}
		}
		server->OnAccept(acceptSocket, recvAddr);

		if (server->Status() != SERVER_READY) {
			break;
		}
	}
	return 0;
}

DWORD WINAPI IOCPServer::WorkerThread(LPVOID serverPtr)
{
	IOCPServer *server = (IOCPServer *)serverPtr;

	while (!_shutdown) {
		IoData			*ioData = nullptr;
		IOCPSession	*session = nullptr;
		DWORD			transferSize;

		BOOL ret = GetQueuedCompletionStatus(server->Iocp(), &transferSize, (PULONG_PTR)&session, (LPOVERLAPPED *)&ioData, INFINITE);
		if (!ret) {
			continue;
		}
		if (session == nullptr) {
			SLog(L"! socket data broken");
			return 0;
		}
		if (transferSize == 0) {
			SLog(L"* close by client[%d][%s]", session->Id(), session->clientAddress().c_str());
			_session_manager.CloseSession(session);
			continue;
		}

		switch (ioData->Type()) {
		case IO_WRITE:
			session->OnSend((size_t)transferSize);
			continue;

		case IO_READ:
			{
				Package *package = session->OnRecv((size_t)transferSize);
				if (package != nullptr) {
					server->PutPackage(package);
				}
			}
			continue;

		case IO_ERROR:
			SLog(L"* close by client error [%d][%s]", session->Id(), session->ClientAddress().c_str());
			_session_manager.CloseSession(session);
			continue;
		}
	}
	return 0;
}