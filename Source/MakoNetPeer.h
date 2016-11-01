#pragma once
#include "MakoNetCommon.h"
#include "MakoReferenceCounted.h"

MAKO_BEGIN_NAMESPACE

class NetPacket
{
};

//! This can only be used in conjunction with another Mako processes' NetPeer.
//! To program more general networking, use ClientSocket and ServerSocket. A
//! NetPeer underneath the interface is no more than a regular Socket, but which
//! connects to a default port that the Mako Game Engine uses; it's more convienent
//! than using sockets when communicating with other Mako processes. Instead of using
//! blocking functions, data sent between NetPeers are received via NetPeerPacketEvents.
class NetPeer : public ReferenceCounted
{
public:
	//! Send a packet to the peer
	virtual void Send(const NetPacket& packet) = 0;
	
	//! Get the IPv4 address of the peer you specified
	virtual const IPv4Address& GetIPv4Address() const = 0;
};

MAKO_END_NAMESPACE