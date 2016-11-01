#pragma once
#include "MakoCommon.h"
#if MAKO_PLATFORM == MAKO_PLATFORM_WIN32
#include "MakoNetworkingDevice.h"
#include "MakoOS.h"

MAKO_BEGIN_NAMESPACE

// Forward declaration
class Application;

class WinsockDevice : public NetworkingDevice
{
private:
	WSAData wsaData;
public:
	WinsockDevice();
	~WinsockDevice();

	NetPeer* GetPeer(const IPv4Address& ipaddr, IP_PROTOCOL ipp);
	ServerSocket* CreateServerSocket(IP_PROTOCOL ipp, NetPort port);
	ClientSocket* CreateClientSocket(const IPv4Address& ipaddr, IP_PROTOCOL ipp, NetPort port);

	MAKO_INLINE WSAData GetWSAData()
	{ return wsaData; }

	String GetName() const
	{ return Text("Winsock 2.2"); }
};

MAKO_END_NAMESPACE
#endif