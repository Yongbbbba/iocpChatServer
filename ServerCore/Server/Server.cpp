#include "pch.h"
#include <iostream>
#include "CorePch.h"

int main() {
  cout << "Yesterday : "<<  CLOCK.Yesterday().c_str() << endl;
  cout << "Today : " <<  CLOCK.Today().c_str()
  << endl;

  cout << "Tommorow :" << CLOCK.Tomorrow().c_str() << endl;

  cout << "today is [" << CLOCK.TodayOfTheWeek() << "]week of the day" << endl;

  cout << "system : " <<  NOW_TICK() << endl;

  for (int i = 0; i < 30; i++) {
  
	  cout << CLOCK.NowTimeWithMilliSec().c_str() << endl;
  }
  return 0;
}



