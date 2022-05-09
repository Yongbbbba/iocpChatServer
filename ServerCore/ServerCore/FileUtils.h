#pragma once
#include <vector>
#include "Types.h"

class FileUtils {
 public:
  static vector<Byte> ReadFile(const WCHAR* path);
  static wstring Convert(string str); //for utf16
};


