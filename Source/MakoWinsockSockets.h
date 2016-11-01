#pragma once
#include "MakoCommon.h"
#if MAKO_PLATFORM == MAKO_PLATFORM_WIN32
#include "MakoNetSockets.h"
#include "MakoOS.h"

MAKO_BEGIN_NAMESPACE

// Forward declaration
class WinsockDevice;

class WinsockSocketOutputStream : public OutputStream
{
	SOCKET s;
public:
	MAKO_INLINE WinsockSocketOutputStream(SOCKET s) : s(s) {}
	MAKO_INLINE ~WinsockSocketOutputStream() {}

	void WriteData(const void* ptr, UInt32 cBytes);
};

class WinsockSocketInputStream : public InputStream
{
	SOCKET s;
	bool cleanup;
public:
	MAKO_INLINE WinsockSocketInputStream(SOCKET s, bool cleanup = false) : s(s), cleanup(cleanup) {}
	MAKO_INLINE ~WinsockSocketInputStream();
	
	void ReadTo(void* buffer, UInt32 cBytes);
};

class WinsockClientSocket : public ClientSocket
{
	WinsockDevice* wd;
	IPv4Address ipaddr;
	IP_PROTOCOL ipp;
	NetPort port;
	bool hasConnected;

	SOCKET connectSocket;

	InputStream* is;
	OutputStream* os;
public:
	WinsockClientSocket(WinsockDevice* wd, const IPv4Address& ipaddr,
		IP_PROTOCOL ipp, NetPort port);
	~WinsockClientSocket();
	
	MAKO_INLINE NETSOCKET_TYPE GetType()
	{ return NST_CLIENT; }

	NetworkingDevice* GetNetworkingDevice();

	void Connect();

	InputStream* GetInputStream()
	{ return is; }

	OutputStream* GetOutputStream()
	{ return os; }

	bool IsConnected() const
	{ return hasConnected; }
};

class WinsockServerSocket : public ServerSocket
{
	WinsockDevice* wd;
	IP_PROTOCOL ipp;
	NetPort port;

	SOCKET listenSocket;
	SOCKET clientSocket;

	InputStream* is;
	OutputStream* os;
public:
	WinsockServerSocket(WinsockDevice* wd, IP_PROTOCOL ipp, NetPort port);
	~WinsockServerSocket();

	MAKO_INLINE NETSOCKET_TYPE GetType()
	{ return NST_SERVER; }

	NetworkingDevice* GetNetworkingDevice();

	void Listen() {}

	InputStream* GetInputStream()
	{ return is; }

	OutputStream* GetOutputStream()
	{ return os; }
	
	bool IsConnected() const
	{ return true; }
};

MAKO_END_NAMESPACE
#endif