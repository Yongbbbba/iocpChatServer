#pragma once

#include  "pch.h"
#include <atomic>

// ------------------------------------------------
// WCHAR* ��� ����
#define cout wcout

//-------------------------------------------------//
// Ÿ�� ����
using object_t = UINT64;
using oid_t = UINT64;
using packet_size_t = int32_t;

using tick_t = std::time_t;
using thread_t = std::thread;
using threadId_t = std::thread::id;

using lock_t = std::recursive_mutex;

using str_t = std::string;
using wstr_t = std::wstring;

//-------------------------------------------------------------------//
// ũ�� ����
#define SIZE_8 8
#define SIZE_64 64
#define SIZE_128 128
#define SIZE_256 256
#define SIZE_1024 1024
#define SIZE_4096 4096
#define SIZE_8192 8192

//-------------------------------------------------------------------//
#define DB_PARAM_SIZE 8192
#define SOCKET_BUF_SIZE 1024 * 10

//-------------------------------------------------------------------//
// ��Ŷ�� type�� ���߱� ���� ������ C# ����
using Byte = unsigned char;
using Char = char;
using Int16 = INT16;
using UInt16 = UINT16;
using Int32 = INT32;
using UInt32 = UINT32;
using Int64 = INT64;
using UInt64 = UINT64;
using Float = float;

// for xml 
using xml_t = TiXmlDocument;
using xmlNode_t = TiXmlElement;
using xmlHandle_t = TiXmlHandle;

