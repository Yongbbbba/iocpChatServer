#pragma once
#include "pch.h"
// ���� ���� ����ü

struct SOCKET_DATA {
	SOCKET				socket_;
	SOCKADDR_IN			addrInfo_;
};

enum {
	SESSION_TYPE_TERMINAL, // ���� ���� �������� ����
	SESSION_TYPE_CLIENT, // Ŭ���̾�Ʈ���� ����
};

class Session
{
protected:
	SOCKET_DATA		    socketData_;
	oid_t				id_;
	int8_t				type_;
	tick_t				lastHeartBeat_;
	bool				SetSocketOpt();

public:
    Session();
    virtual ~Session();

	virtual bool		OnAccept(SOCKET socket, SOCKADDR_IN addrInfo);

	virtual void		OnSend(size_t transferSize) = 0;
	virtual void		SendPacket(Packet *packet) = 0;
	
	virtual Package*	OnRecv(size_t transferSize) = 0;
	virtual void		RecvStandBy() {};

    virtual void		OnClose(bool force = false);

	SOCKET&				Socket();
    wstr_t				ClientAddress();

	oid_t				Id();
	void				SetId(oid_t id);

	int8_t				Type();
	void				SetType(int8_t type);

	tick_t				HeartBeat();
	void				UpdateHeartBeat();
};