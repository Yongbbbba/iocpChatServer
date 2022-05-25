#include "pch.h"
#include "Server.h"

Server::Server(ContentsProcess *contentsProcess)
{
	SLog(L"# Initialze network base");
	
	contentsProcess_ = contentsProcess;
	status_ = SERVER_STOP;

	xml_t config;
	if (!LoadConfig(&config)) {
		return;
	}
	this->Initialize(&config);

	// db �ʱ�ȭ
	// TODO) db_manager
	_db_manager;
}

Server::~Server()
{
	ShutdownServer();

	status_ = SERVER_STOP;
	SAFE_DELETE(contentsProcess_);

	SLog(L"# End network base");
}

void Server::Initialize(xml_t *config)
{
	//�͹̳� ����
	_terminal.Run(this);

	//�׽�ũ ����
	TaskManager::GetInstance();

	//���� ����
	xmlNode_t *root = config->FirstChildElement("App")->FirstChildElement("Server");
	if (!root) {
		SLog(L"@ not exist server setting");
		return;
	}
	xmlNode_t *elem = root->FirstChildElement("IP");
	strcpy_s(ip_, elem->GetText());

	elem = root->FirstChildElement("Port");
	sscanf_s(elem->GetText(), "%d", &port_);

	elem = root->FirstChildElement("ThreadCount");
	sscanf_s(elem->GetText(), "%d", &workerThreadCount_);

	status_ = SERVER_INITIALZE;
	SLog(L"* IO worker thread count : %d", workerThreadCount_);

	root = config->FirstChildElement("App");
	elem = root->FirstChildElement("Name");

	SLog(L"### %S start!!! ###", elem->GetText());
}

SERVER_STATUS &Server::Status()
{
	return status_;
}

void Server::PutPackage(Package *package)
{
	contentsProcess_->PutPackage(package);
}
