#include "pch.h"
#include "Session.h"
#include "IOCPSession.h"
#include "SessionManager.h"
#include "PacketAnalyzer.h"

IoData::IoData()
{
	ZeroMemory(&overlapped_, sizeof(overlapped_));
	ioType_ = IO_ERROR;
	
	this->Clear();
}

void IoData::Clear()
{
	buffer_.fill(0);
	totalBytes_ = 0;
	currentBytes_ = 0;
}

bool IoData::NeedMoreIO(size_t transferSize)
{
	currentBytes_ += transferSize;
	if (currentBytes_ < totalBytes_) {
		return true;
	}
	return false;
}

int32_t IoData::SetupTotalBytes()
{
	packet_size_t offset = 0;
	packet_size_t packetLen[1] = { 0, };
	if (totalBytes_ == 0) {
		memcpy_s((void *)packetLen, sizeof(packetLen), (void *)buffer_.data(), sizeof(packetLen));
		PacketObfuscation::GetInstance().DecodingHeader((Byte*)&packetLen, sizeof(packetLen));

		totalBytes_ = (size_t)packetLen[0];
	}
	offset += sizeof(packetLen);

	return offset;
}
size_t IoData::TotalByte()
{
	return totalBytes_;
}

IO_OPERATION &IoData::Type()
{
	return ioType_;
}

void IoData::SetType(IO_OPERATION type)
{
	ioType_ = type;
}

char* IoData::Data()
{
	return buffer_.data();
}

bool IoData::SetData(Stream &stream)
{
	this->Clear();

	if (buffer_.Max_size() <= stream.Size()) {
		SLog(L"! packet size too big [%d]byte", stream.Size());
		return false;
	}

	const size_t packetHeaderSize = sizeof(packet_size_t);
	packet_size_t offset = 0;

	char *buf = buffer_.Data();
	//									 head size  + real data size
	packet_size_t packetLen[1] = { (packet_size_t)packetHeaderSize + (packet_size_t)stream.Size(), };
	// insert packet len
	memcpy_s(buf + offset, buffer_.Max_size(), (void *)packetLen, packetHeaderSize);
	offset += packetHeaderSize;

	// packet obfuscation
	PacketObfuscation::GetInstance().EncodingHeader((Byte*)buf, packetHeaderSize);
	PacketObfuscation::GetInstance().EncodingData((Byte*)stream.Data(), stream.Size());

	// insert packet data
	memcpy_s(buf + offset, buffer_.Max_size(), stream.Data(), (int32_t)stream.Size());
	offset += (packet_size_t)stream.Size();

	totalBytes_ = offset;
	return true;
}

LPWSAOVERLAPPED IoData::Overlapped()
{
	return &overlapped_;
}

WSABUF IoData::Wsabuf()
{
	WSABUF wsaBuf;
	wsaBuf.buf = buffer_.Data() + currentBytes_;
	wsaBuf.len = (ULONG)(totalBytes_ - currentBytes_);
	return wsaBuf;
}

//-----------------------------------------------------------------//
IOCPSession::IOCPSession()
: Session()
{
	this->Initialize();
}

void IOCPSession::Initialize()
{
	ZeroMemory(&socketData_, sizeof(SOCKET_DATA));
	ioData_[IO_READ].SetType(IO_READ);
	ioData_[IO_WRITE].SetType(IO_WRITE);
}

void IOCPSession::CheckErrorIO(DWORD ret)
{
	if (ret == SOCKET_ERROR
		&& (WSAGetLastError() != ERROR_IO_PENDING)) {
        SLog(L"! socket error: %d", WSAGetLastError());
	}
}

void IOCPSession::Recv(WSABUF wsaBuf)
{
	DWORD flags = 0;
	DWORD recvBytes;
	DWORD errorCode = WSARecv(socketData_.socket_, &wsaBuf, 1, &recvBytes, &flags, ioData_[IO_READ].Overlapped(), NULL);
	this->CheckErrorIO(errorCode);
}

bool IOCPSession::IsRecving(size_t transferSize)
{
	if (ioData_[IO_READ].NeedMoreIO(transferSize)) {
		this->Recv(ioData_[IO_READ].Wsabuf());
		return true;
	}
	return false;
}

void IOCPSession::RecvStandBy()
{
	ioData_[IO_READ].Clear();

	WSABUF wsaBuf;
	wsaBuf.buf = ioData_[IO_READ].Data();
	wsaBuf.len = SOCKET_BUF_SIZE;

	this->Recv(wsaBuf);
}

void IOCPSession::Send(WSABUF wsaBuf)
{
	DWORD flags = 0;
	DWORD sendBytes;
	DWORD errorCode = WSASend(socketData_.socket_, &wsaBuf, 1, &sendBytes, flags, ioData_[IO_WRITE].Overlapped(), NULL);
	this->CheckErrorIO(errorCode);
}

void IOCPSession::OnSend(size_t transferSize)
{
	if (ioData_[IO_WRITE].NeedMoreIO(transferSize)) {
		this->Send(ioData_[IO_WRITE].Wsabuf());
	}
}

void IOCPSession::SendPacket(Packet *packet)
{
	Stream stream;
	packet->Encode(stream);
	if (!ioData_[IO_WRITE].SetData(stream)) {
		return;
	}
	
	WSABUF wsaBuf;
	wsaBuf.buf = ioData_[IO_WRITE].Data();
	wsaBuf.len = (ULONG) stream.Size();

	this->Send(wsaBuf);
    this->RecvStandBy();
}

Package *IOCPSession::OnRecv(size_t transferSize)
{
	packet_size_t offset = 0;
	offset += ioData_[IO_READ].SetupTotalBytes();

	if (this->IsRecving(transferSize)) {
		return nullptr;
	}

	const size_t packetHeaderSize = sizeof(packet_size_t);
	packet_size_t packetDataSize = (packet_size_t)(ioData_[IO_READ].TotalByte() - packetHeaderSize);
	Byte *packetData = (Byte*) ioData_[IO_READ].Data() + offset;

	PacketObfuscation::GetInstance().DecodingData(packetData, packetDataSize);
	Packet *packet = PacketAnalyzer::GetInstance().Analyzer((const char *)packetData, packetDataSize);
	if (packet == nullptr) {
		SLog(L"! invaild packet");
		this->OnClose(true);
		return nullptr;
	}

	this->RecvStandBy();

	Package *package = new Package(this, packet);
	return package;
}