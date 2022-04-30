#pragma once

#include <chrono>
#include <ctime>

#include "pch.h"

#define CLOCK Clock::GetInstance()
#define NOW_TICK CLOCK.SystemTick
#define NOW_STRING CLOCK.NowTime

#define TICK_MIN (60)
#define TICK_HOUR (TICK_MIN * 60)
#define TICK_DAY (TICK_HOUR * 24)

#define TICK_TO_MIN(x) (x / TICK_MIN)
#define MIN_TO_TICK(x) (x * TICK_MIN)

#define TICK_TO_HOUR(x) (x / TICK_HOUR)
#define HOUR_TO_TICK(x) (x * TICK_HOUR)

#define TICK_TO_DAY(x) (x / TICK_DAY)
#define DAY_TO_TICK(x) (x * TICK_DAY)

typedef enum {
  DAY_SUNDAY = 0,
  DAY_MONDAY = 1,     
  DAY_TUESDAY = 2,    
  DAY_WEDNESDAY = 3,  
  DAY_THURSDAY = 4,
  DAY_FRIDAY = 5,
  DAY_SATURDAY = 6,
} DayOfTheWeek ;

#define DATETIME_FORMAT L"D%Y-%m-%dT%H:%M:%S"
#define DATE_FORMAT L"%Y-%m-%d"
#define TIME_FORMAT L"%H:%M:%S"
#define DB_TIME_FORMAT L"%4d-%2d-%2d %2d:%2d:%2d"

using namespace std::chrono;
using namespace std;
typedef system_clock::time_point timePoint;

class Clock : public Singleton<Clock> {
  tick_t serverStartTick_;

  wstr_t TickToStr(tick_t tick, const WCHAR *fmt = DATETIME_FORMAT);

 public:
  Clock();
  ~Clock();

  tick_t ServerStartTick();
  tick_t SystemTick();
  tick_t StrToTick(wstr_t str, const WCHAR *fmt = DB_TIME_FORMAT);

  wstr_t NowTime(const WCHAR *fmt = DATETIME_FORMAT);
  wstr_t NowTimeWithMilliSec(const WCHAR *fmt = DATETIME_FORMAT);

  wstr_t Today();
  wstr_t Tomorrow();
  wstr_t Yesterday();

  DayOfTheWeek TodayOfTheWeek();
};