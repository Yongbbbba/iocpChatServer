#pragma once

// ���⿡ �̸� �������Ϸ��� ��� �߰�
#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#ifdef _DEBUG
#pragma comment(lib, "Debug\\ServerLibrary.lib")
#else
#pragma comment(lib, "Release\\ServerLibrary.lib")
#endif // DEBUG
