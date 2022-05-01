#include "pch.h"
#include "Config.h"

bool LoadConfig(xml_t* config) {
  if (!config->LoadFile("./config.xml")) {
    cout << "! not exist config file.") << endl;
    return false;
  }
  return true;
}
