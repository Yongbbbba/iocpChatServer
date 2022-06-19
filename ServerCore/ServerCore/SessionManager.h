#pragma once
#include "pch.h"
#include "Session.h"
// 세션을 관리 하는 주체

#define SESSION_CAPACITY		(5000)
#define _session_manager		SessionManager::GetInstance()

class SessionManager : public Singleton<SessionManager>
{
	// 세션의 빈번한 추가삭제를 위해 list로 구성
    typedef list<Session*>		SessionList;

    SessionList		            sessionList_;
	int									sessionCount_;
	int									maxConnection_;
    Lock								lock_;

	oid_t								sessionSeed_;			// 세션 메니져에서 관리하는 시드

	// 서버 수동 명령어
    typedef std::function<void (SessionList *sessionList, wstr_t *arg)> cmdFunc;
	unordered_map<wstr_t, cmdFunc>   serverCommand_;

public:
	SessionManager(int maxConnection = SESSION_CAPACITY);
	~SessionManager();
	oid_t				CreateSessionId();

	bool				AddSession(Session *session);

	list<Session*>		&GetSessionList();
	bool				CloseSession(Session *session);
	void				ForceCloseSession(Session *session);

	Session				*GetSession(oid_t id);

    void                RunCommand(wstr_t cmd);
    void                CommandFuncInitialize();
};