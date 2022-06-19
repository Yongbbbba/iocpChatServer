#include "pch.h"
#include "SessionMonitor.h"

SessionMonitor::SessionMonitor() 
{
	static bool init = false;
	if (init) {
		return;
	}
	init = true;

	const int MONITOR_REPORTING_SEC = 1;
	TaskManager::GetInstance().Add(this, MONITOR_REPORTING_SEC, TICK_INFINITY);
}

void SessionMonitor::Tick()
{
	// 카카오 오목처럼 30초간 반응 없으면 연결 끊기
	const int MAX_HEART_BEAT = 30 * 1000;
	auto list = _session_manager.GetSessionList();
	tick_t now = CLOCK.SystemTick();

	for (auto session : list) {
		if (session->Type() != SESSION_TYPE_CLIENT) {
			continue;
		}

		tick_t lastTick = session->HeartBeat();
		if (now - lastTick > MAX_HEART_BEAT) {
			SLog(L"* [%s] Closing by heartBeat", session->ClientAddress().c_str());
			session->OnClose(true);
		}
	}
}
