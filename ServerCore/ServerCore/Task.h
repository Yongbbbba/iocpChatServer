#pragma once
#include "pch.h"

#define TICK_INFINITY		0

class Work;
class TaskNode
{
	Work*		mWorkObject;
	int			mFreqSec;
	int			mDurationSec;

	tick_t		mNextTick;
public:
	TaskNode(Work *workObject, int freqSec, int durationSec);
	~TaskNode();

	void NextTick();
	bool Expired();

	void Tick();
};

//---------------------------------------------------------------------//
class Task
{
	std::list<TaskNode *> mTaskList;
	Thread		*mThread;
	int			mId;

public:
	Task(int id);
	~Task();
	void Add(TaskNode *taskNode);
	void Remove(TaskNode *taskNode);

	void Process();
	void Run();
};

//---------------------------------------------------------------------//
class TaskManager : public Singleton <TaskManager>
{
	int                     mThreadCount;
	std::vector<Task *>     mTaskPool;

public:
	TaskManager();
	virtual ~TaskManager();

	void Initialize(xml_t *config);

	void Add(Work *workObject, int freqSec, int durationSec);
};
