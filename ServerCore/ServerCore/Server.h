#pragma once
#include "pch.h"
#include "SessionManager.h"

enum SERVER_STATUS {
	SERVER_STOP,
	SERVER_INITIALZE,
	SERVER_READY,
};

class Server
{
protected:
	char					ip_[16];
	int						port_;
	int						workerThreadCount_;

	SERVER_STATUS			status_;
	ContentsProcess			*contentsProcess_;

public:
	Server(ContentsProcess *contentsProcess);
	virtual ~Server();

	virtual void			Initialize(xml_t *config);

	virtual bool			Run() = 0;
	SERVER_STATUS&			Status();

	void					PutPackage(Package *package);
};