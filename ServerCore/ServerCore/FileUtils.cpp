#include "pch.h"
#include "FileUtils.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

vector<Byte> FileUtils::ReadFile(const WCHAR* path) { vector<Byte> ret;
  fs::path filePath{path};

  const UInt32 fileSize = static_cast<UInt32>(fs::file_size(filePath));
  ret.resize(fileSize);

  basic_ifstream<Byte> inputStream{filePath};
  inputStream.read(&ret[0], fileSize);

  return ret;
}

wstring FileUtils::Convert(string str) {
  const Int32 srcLen = static_cast<Int32>(str.size());
  wstring ret;
  if (srcLen == 0) return ret;

  const Int32 retLen = ::MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<char*>(&str[0]), srcLen, NULL, 0);
	ret.resize(retLen);
	::MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<char*>(&str[0]), srcLen, &ret[0], retLen);

  return ret;
}
