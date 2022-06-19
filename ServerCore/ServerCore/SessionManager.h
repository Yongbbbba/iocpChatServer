#pragma once
#include "pch.h"
#include "Session.h"
// ������ ���� �ϴ� ��ü

#define SESSION_CAPACITY		(5000)
#define _session_manager		SessionManager::GetInstance()

class SessionManager : public Singleton<SessionManager>
{
	// ������ ����� �߰������� ���� list�� ����
    typedef list<Session*>		SessionList;

    SessionList		            sessionList_;
	int									sessionCount_;
	int									maxConnection_;
    Lock								lock_;

	oid_t								sessionSeed_;			// ���� �޴������� �����ϴ� �õ�

	// ���� ���� ��ɾ�
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