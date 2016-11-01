#pragma once
#include "MakoNetCommon.h"
#include "MakoStream.h"

MAKO_BEGIN_NAMESPACE

// Forward declaration
class NetworkingDevice;

//! The only real difference between client and server sockets is that
//! the client socket initiates the connection, and the server waits
//! for incoming connections. Once they have handshaked, then they can
//! send data bi-directionally.
enum NETSOCKET_TYPE
{ NST_SERVER, NST_CLIENT, NST_ENUM_LENGTH };

class NetSocket : public ReferenceCounted
{
public:
	//! Get the type of NetSocket this is. If NST_SERVER, you can cast this
	//! to ServerSocket, and if NST_CLIENT this can be casted likewise to
	//! a ClientSocket.
	virtual NETSOCKET_TYPE GetType() = 0;

	//! Get the NetworkingDevice
	virtual NetworkingDevice* GetNetworkingDevice() = 0;

	//! Get an InputStream in order to read data sent from the other socket
	virtual InputStream* GetInputStream() = 0;
	
	//! Get an OutputStream in order to send data to the other socket
	virtual OutputStream* GetOutputStream() = 0;

	//! Tests whether the Socket has made a connection with the other
	//! socket.
	virtual bool IsConnected() const = 0;

};

//! ClientSockets and ServerSockets can both send and receive data from either ends,
//! but a ClientSocket has the duty of initializing the connection and a ServerSocket
//! has the duty of accepting the connection.
class ClientSocket : public NetSocket
{
public:
	MAKO_INLINE NETSOCKET_TYPE GetType()
	{ return NST_CLIENT; }

	//! Blocking function. Tries to connect to server (specified earler
	//! by IP address).
	virtual void Connect() = 0;
};

//! ClientSockets and ServerSockets can both send and receive data from either ends,
//! but a ClientSocket has the duty of initializing the connection and a ServerSocket
//! has the duty of accepting the connection.
class ServerSocket : public NetSocket
{
public:
	MAKO_INLINE NETSOCKET_TYPE GetType()
	{ return NST_SERVER; }

	//! Blocking function, places the socket in a state in which it is
	//! listening for an incoming connection.
	virtual void Listen() = 0;
};

MAKO_END_NAMESPACE