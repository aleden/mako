#pragma once
#include "MakoNetCommon.h"
#include "MakoNetSockets.h"
#include "MakoNetPeer.h"
#include "MakoDevice.h"

MAKO_BEGIN_NAMESPACE

enum NETWORKING_DEVICE_TYPE
{
	//! The Windows Sockets (abbreviated "Winsock" or "WinSock") specification 
	//! defines a network programming interface for Microsoft Windows which is based 
	//! on the "socket" paradigm popularized in BSD Unix. It encompasses both 
	//! familiar Berkeley socket style routines and a set of Windows-specific
	//! extensions. (Taken from the Winsock Programmer's FAQ)
	NDT_WINSOCK2,
	NDT_ENUM_LENGTH
};

//! This class provides functionality for all things networking.
class NetworkingDevice : public Device
{
public:
	//! Get a NetPeer
	//! \param[in] ipaddr The IP address of the NetPeer
	//! \param[in] ipp The IP protocol (UDP/TCP) to use when communicating with the
	//! NetPeer.
	//! \return The NetPeer
	virtual NetPeer*      GetPeer(const IPv4Address& ipaddr, IP_PROTOCOL ipp)  = 0;
	
	//! Create a ServerSocket
	//! \param[in] ipp The IP protocol to use when accepting new connections.
	//! \param[in] port The port number (UInt16) to accept connections to.
	//! \return The created ServerSocket
	virtual ServerSocket* CreateServerSocket(IP_PROTOCOL ipp, NetPort port) = 0;
	
	//! Create a ClientSocket
	//! \param[in] ipaddr The IP address of the ServerSocket you want to connect to.
	//! \param[in] ipp The IP protocol (UDP/TCP) to use when communicating through to
	//! the ServerSocket.
	//! \param[in] port The port number (UInt16) to connect to.
	//! \return The created ClientSocket
	virtual ClientSocket* CreateClientSocket(const IPv4Address& ipaddr,
	                                         IP_PROTOCOL ipp, NetPort port) = 0;

	//! Gets the default port that NetPeers use
	MAKO_INLINE NetPort GetDefaultPeerPort()
	{ return 22307; }

	virtual ~NetworkingDevice() {}
};

MAKO_END_NAMESPACE