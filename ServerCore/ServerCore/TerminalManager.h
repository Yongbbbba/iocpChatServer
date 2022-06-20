#pragma once
#include "pch.h"

#define _terminal			TerminalManager::GetInstance()

// 다른 서버간의 연결을 위한 통로
class TerminalManager : public Singleton<TerminalManager>
{
	unordered_map<wstr_t, Terminal*> terminalPool_;
	Server						*server_;

public:
	TerminalManager();
	virtual ~TerminalManager();

	void Initialize(xml_t *config);

	void Put(wstr_t serverName, Terminal *terminal);
	Terminal* Get(wstr_t name);
	
	bool IsTerminal(const char *ip);
	void Run(Server *server);
};