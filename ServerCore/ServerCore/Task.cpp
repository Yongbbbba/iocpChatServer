#include "pch.h"
#include "Task.h"

TaskNode::TaskNode(Work *workObject, int freqSec, int durationSec)
{
	mWorkObject = workObject;
	mFreqSec = freqSec;
	mDurationSec = durationSec;
	this->NextTick();
}

TaskNode::~TaskNode()
{
	SAFE_DELETE(mWorkObject);
}

void TaskNode::NextTick()
{
	mNextTick = NOW_TICK() + (tick_t)mFreqSec;
}

bool TaskNode::Expired()
{
	if (mWorkObject == nullptr) {
		return true;
	}
	if (mDurationSec != TICK_INFINITY) {
		if (mDurationSec < NOW_TICK()) {
			return true;
		}
	}
	return false;
}

void TaskNode::Tick()
{
	if (mNextTick < NOW_TICK()) 
	{
		mWorkObject->Tick();
		this->NextTick();
	}
}

//---------------------------------------------------------------------//
Task::Task(int id) 
{
	mId = id;
}

Task::~Task()
{
	for (auto node : mTaskList) {
		SAFE_DELETE(node);
	}
}

void Task::Add(TaskNode *taskNode)
{
	mTaskList.push_back(taskNode);
}

void Task::Remove(TaskNode *taskNode)
{
	mTaskList.remove(taskNode);
}

void Task::Process()
{
	while (!_shutdown) {
		std::vector<TaskNode *> deleteNodes;
		for (auto taskNode : mTaskList) {
			if (taskNode->Expired()) {
				deleteNodes.push_back(taskNode);
				continue;
			}
			taskNode->Tick();
		}

		for (auto node : deleteNodes) {
			this->Remove(node);
		}
		CONTEXT_SWITCH;
	}
}

void Task::Run()
{
	mThread = MAKE_THREAD(Task, Process);
}

//---------------------------------------------------------------------//
TaskManager::TaskManager()
{
	xml_t config;
	if (!LoadConfig(&config)) {
		return;
	}
	this->Initialize(&config);
}

void TaskManager::Initialize(xml_t *config)
{
	xmlNode_t *root = config->FirstChildElement("App")->FirstChildElement("Task");
	if (!root) {
		SLog(L"@ not exist task setting");
		return;
	}
	xmlNode_t *elem = root->FirstChildElement("ThreadCount");
	sscanf_s(elem->GetText(), "%d", &mThreadCount);

	for (int i = 0; i < mThreadCount; ++i) {
		Task *task = new Task(i);
		mTaskPool.push_back(task);
		task->Run();
	}
	SLog(L"* task thread, [%d] maked", mThreadCount);
}

TaskManager::~TaskManager()
{
	for (auto task : mTaskPool) {
		SAFE_DELETE(task);
	}
}

void TaskManager::Add(Work *workObject, int freqSec, int durationSec)
{
	const int MINIMAL_THREAD_COUNT = 1;
	if (mThreadCount < MINIMAL_THREAD_COUNT) {
		return;
	}
	static int nodeCount = 0;
	
	TaskNode *node = new TaskNode(workObject, freqSec, durationSec);
	int index = nodeCount % mThreadCount;
	Task *task = mTaskPool[index];
	task->Add(node);

	++nodeCount;
}