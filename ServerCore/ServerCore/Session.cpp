#include "pch.h"
#include "Session.h"
#include "IOCPServer.h"

Session::Session()
{
	type_ = SESSION_TYPE_CLIENT;
	this->UpdateHeartBeat();
}

Session::~Session()
{
	this->OnClose();
}

// 3초 간격으로 세션이 살아있는지 체크, 응답이 안오면 재차 패킷 보내고, 그래도 안오면 바로 세션 연결 종료
bool Session::SetSocketOpt()
{
#ifdef linux
	int keepAlive = 1;
	int keepAliveIdle = 1;
	int keepAliveCnt = 3;
	int keepAliveInterval = 3;

	int ret;
	ret = ::setsockopt(socketData_.socket_, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof(keepAlive));
	if (ret == SOCKET_ERROR) {
		return false;
	}
	ret = ::setsockopt(socketData_.socket_, SOL_TCP, SO_KEEPIDLE, &keepAliveIdle, sizeof(keepAliveIdle));
	if (ret == SOCKET_ERROR) {
		return false;
	}
	ret = ::setsockopt(socketData_.socket_, SOL_TCP, SO_KEEPCNT, &keepAliveCnt, sizeof(keepAliveCnt));
	if (ret == SOCKET_ERROR) {
		return false;
	}
	ret = ::setsockopt(socketData_.socket_, SOL_TCP, SO_KEEPINTVL, &keepAliveInterval, sizeof(keepAliveInterval));
	if (ret == SOCKET_ERROR) {
		return false;
	}
#else
	tcp_keepalive keepAliveSet = { 0 }, returned = { 0 };
	keepAliveSet.onoff = 1;
	keepAliveSet.keepalivetime = 3000;        // Keep Alive in 3 sec.
	keepAliveSet.keepaliveinterval = 3000;    // Resend if No-Reply

	DWORD dwBytes;
	if (WSAIoctl(socketData_.socket_, SIO_KEEPALIVE_VALS, &keepAliveSet, sizeof(keepAliveSet), &returned, sizeof(returned), &dwBytes, NULL, NULL) != 0) {
		return false;
	}
#endif
	return true;
}

bool Session::OnAccept(SOCKET socket, SOCKADDR_IN addrInfo)
{
	socketData_.socket_ = socket;
	int addrLen;
	getpeername(socketData_.socket_, (struct sockaddr *)&socketData_.addrInfo_, &addrLen);
	socketData_.addrInfo_ = addrInfo;
	if (!this->SetSocketOpt()) {
		return false;
	}
	return true;
}

void Session::OnClose(bool force)
{
	if (force) {
		_session_manager.ForceCloseSession(this);
	}
	else {
		_session_manager.CloseSession(this);
	}
}

SOCKET& Session::Socket()
{
	return socketData_.socket_;
}

wstr_t Session::ClientAddress()
{
	array<char, SIZE_64> ip;
	inet_ntop(AF_INET, &(socketData_.addrInfo_.sin_addr), ip.data(), ip.size());

	array<WCHAR, SIZE_64> wip;
	StrConvA2W(ip.data(), wip.data(), wip.max_size());
	wstr_t stringData = wip.data();

	return stringData;
}

oid_t Session::Id()
{
	return id_;
}

void Session::SetId(oid_t id)
{
	id_ = id;
}

int8_t Session::Type()
{
	return type_;
}

void Session::SetType(int8_t type)
{
	type_ = type;
}

tick_t Session::HeartBeat()
{
	return lastHeartBeat_;
}

void Session::UpdateHeartBeat()
{
	lastHeartBeat_ = CLOCK.SystemTick();
}