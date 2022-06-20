#pragma once
#include "pch.h"

// �͹̳� ������, Ÿ ������ �����ؼ� ������ �ۼ��ſ� ����Ѵ�.
enum TERMINAL_STATUS {
	TERMINAL_STOP,
	TERMINAL_READY,
};

class Server;
class Terminal
{
protected:
	Server					*server_;
	wstr_t					name_;
	TERMINAL_STATUS			status_;

	char					ip_[16];
	int						port_;

	TerminalSession			session_;

	Thread					*processThread_;

public:
	Terminal(Server *server, wstr_t name);
	virtual ~Terminal();
	TERMINAL_STATUS &Status();

	void		Initialize(xmlNode_t *config);
	void		SendPacket(Packet *packet);
	const char *Ip();
	int			Port();

private:
	void		ConnectProcess();
	void		Run();
};