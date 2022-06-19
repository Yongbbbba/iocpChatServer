#pragma once
#include "pch.h"
class Session;
class SessionManager;
class Package;

typedef enum {
	IO_READ,
	IO_WRITE,
	IO_ERROR,
} IO_OPERATION;

#define IO_DATA_MAX     (2)

class IoData
{
	OVERLAPPED		overlapped_;
	IO_OPERATION	ioType_;
    size_t  		totalBytes_;
	size_t			currentBytes_;
	array<char, SOCKET_BUF_SIZE> buffer_;

public:
	IoData();

	void Clear();

	bool NeedMoreIO(size_t transferSize);
	int32_t SetupTotalBytes();
	size_t TotalByte();

	IO_OPERATION& Type();
	void SetType(IO_OPERATION type);

	WSABUF Wsabuf();
	char* Data();
	bool SetData(Stream& stream);
	LPWSAOVERLAPPED Overlapped();
};

//-----------------------------------------------------------------//
class IOCPSession : public Session
{
public:
	array<IoData, IO_DATA_MAX> ioData_;

private:
	void			Initialize();

	void			CheckErrorIO(DWORD ret);

	void			Recv(WSABUF wsaBuf);
	bool			IsRecving(size_t transferSize);

	void			Send(WSABUF wsaBuf);

public:
    IOCPSession();

	void			OnSend(size_t transferSize);
	void		    SendPacket(Packet *packet);
	
	Package*		OnRecv(size_t transferSize);
    void			RecvStandBy();

};