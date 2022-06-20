#include "pch.h"
#include "Terminal.h"
#include "PacketAnalyzer.h"

Terminal::Terminal(Server *server, wstr_t name)
{
	server_ = server;
	name_ = name;
}

Terminal::~Terminal()
{
	status_ = TERMINAL_STOP;
}

void Terminal::Initialize(xmlNode_t *config)
{
	xmlNode_t *elem;

	elem = config->FirstChildElement("IP");
	strcpy_s(ip_, elem->GetText());

	elem = config->FirstChildElement("Port");
	sscanf_s(elem->GetText(), "%d", &port_);

	this->Run();
}

TERMINAL_STATUS& Terminal::Status()
{
	return status_;
}

void Terminal::SendPacket(Packet *packet)
{
	if (status_ == TERMINAL_READY) {
		session_.SendPacket(packet);
	}
}

const char *Terminal::Ip()
{
	return ip_;
}

int Terminal::Port()
{
	return port_;
}

void Terminal::ConnectProcess()
{
CONNECT_START:
	int tryCount = 0;
	while (_shutdown == false) {
		if (session_.ConnectTo(ip_, port_)) {
			break;
		}
		SLog(L"* try connect [%s] server[%S]:[%d]... [%d]", name_.c_str(), ip_, port_, tryCount++);
		Sleep(1000);        // 1초마다 연결 시도
	}
	status_ = TERMINAL_READY;

	// 자신이 터미널 세션임을 알려준다.
	PK_I_NOTIFY_TERMINAL terminalPacket;
	this->SendPacket(&terminalPacket);

	SLog(L"* [%s]terminal connect [%S]:[%d] ready", name_.c_str(), ip_, port_);
	while (_shutdown == false) {
		Package *package = session_.OnRecv(0);

		if (package == nullptr) {
			SLog(L"! termnal [%s] disconnected !", name_.c_str());
			session_.OnClose();
			goto CONNECT_START;
		}

		server_->PutPackage(package);
	}
}

void Terminal::Run()
{
	processThread_ = MAKE_THREAD(Terminal, ConnectProcess);
}