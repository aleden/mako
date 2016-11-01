#pragma once
#include "MakoCommon.h"
#if MAKO_PLATFORM == MAKO_PLATFORM_WIN32
#include "MakoNetPeer.h"

MAKO_BEGIN_NAMESPACE

// Forward declaration
class WinsockDevice;

class WinsockPeer : public NetPeer
{
public:
	WinsockPeer(WinsockDevice* wd, const IPv4Address& ipaddr, IP_PROTOCOL ipp) {}
	~WinsockPeer() {}
	
	void Send(const NetPacket& packet) {}
	
	const IPv4Address& GetIPv4Address() const { return ASCIIText("foo"); }
};


MAKO_END_NAMESPACE
#endif