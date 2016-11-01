#include "MakoCommon.h"
#if MAKO_PLATFORM == MAKO_PLATFORM_WIN32
#include "MakoWinsockDevice.h"
#include "MakoException.h"
#include "MakoWinsockPeer.h"
#include "MakoWinsockSockets.h"

#pragma comment (lib, "Ws2_32.lib")

MAKO_BEGIN_NAMESPACE

WinsockDevice::WinsockDevice()
{
	// Initialize Winsock 2.2
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
		throw Exception(Text("The initialization of Winsock 2.2 failed."));
}

WinsockDevice::~WinsockDevice()
{ WSACleanup(); }

NetPeer*      WinsockDevice::GetPeer(const IPv4Address& ipaddr, IP_PROTOCOL ipp)
{ return new WinsockPeer(this, ipaddr, ipp); }

ServerSocket* WinsockDevice::CreateServerSocket(IP_PROTOCOL ipp, NetPort port)
{ return new WinsockServerSocket(this, ipp, port); }

ClientSocket* WinsockDevice::CreateClientSocket(const IPv4Address& ipaddr, IP_PROTOCOL ipp, NetPort port)
{ return new WinsockClientSocket(this, ipaddr, ipp, port); }


MAKO_END_NAMESPACE
#endif
