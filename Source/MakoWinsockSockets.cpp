#include "MakoCommon.h"
#if MAKO_PLATFORM == MAKO_PLATFORM_WIN32
#include <ws2tcpip.h>
#include "MakoWinsockSockets.h"
#include "MakoWinsockDevice.h"
#include "MakoException.h"

MAKO_BEGIN_NAMESPACE

////////////////////////////////////////////////////////////////////////////////////////////
// WinsockSocketOutputStream

void WinsockSocketOutputStream::WriteData(const void* ptr, UInt32 cBytes)
{
	if (send(s, static_cast<const char*>(ptr), cBytes, 0) == SOCKET_ERROR)
		throw Exception(Text("send() failed in WinsockSocketOutputStream::WriteData()"));
}

////////////////////////////////////////////////////////////////////////////////////////////
// WinsockSocketInputStream

WinsockSocketInputStream::~WinsockSocketInputStream()
{
	if (cleanup)
	{
		// shutdown the connection since we're done,
		// The shutdown function disables sends or receives on a socket.
		if (shutdown(s, SD_SEND) == SOCKET_ERROR)
		{
			closesocket(s);
			throw Exception(Text("shutdown() failed in WinsockSocketInputStream::~WinsockSocketInputStream()"));
		}

		// cleanup
		closesocket(s);
	}
}

void WinsockSocketInputStream::ReadTo(void* ptr, UInt32 cBytes)
{
	// The recv function receives data from a connected socket or a bound connectionless socket.
	int iResult = recv(s, static_cast<char*>(ptr), cBytes, 0);
	if (iResult > 0)
	{
		// Bytes received = iResult
	}
	else if (iResult == 0)
	{
		// Connection closing
	}
	else 
	{
		// recv() failed
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// WinsockClientSocket
WinsockClientSocket::WinsockClientSocket(WinsockDevice* wd, const IPv4Address& ipaddr,
		IP_PROTOCOL ipp, NetPort port)
		: wd(wd), ipaddr(ipaddr), ipp(ipp), port(port), connectSocket(INVALID_SOCKET),
		  hasConnected(false)
{
	// 1. Initialize Winsock.
	// 2. Create a socket.
	// 3. Connect to the server.
	// 4. Send and receive data.
	// 5. Disconnect.
	Connect();
	is = new WinsockSocketInputStream(connectSocket);
	is->Hold();
	os = new WinsockSocketOutputStream(connectSocket);
	os->Hold();
}

WinsockClientSocket::~WinsockClientSocket()
{
	is->Drop();
	os->Drop();

	
	// The proper sequence for closing a TCP connection is:

	// 1. Finish sending data.
	// 2. Call shutdown() with the how parameter set to 1.
	// 3. Loop on recv() until it returns 0.
	// 4. Call closesocket().
	

	// shutdown the connection since no more data will be sent
	if (shutdown(connectSocket, SD_SEND) == SOCKET_ERROR)
	{
		closesocket(connectSocket);
		throw Exception(Text("shutdown() failed in WinsockClientSocket::~WinsockClientSocket()"));
	}

	// Receive until the peer closes the connection
	int iResult;
	do
	{
		iResult = recv(connectSocket, nullptr, 0, 0);
		if (iResult > 0)
			; // Bytes received = iResult
		else if (iResult == 0)
			;  // Connection closed
		else
			; // throw Exception(Text("recv() failed in WinsockClientSocket::~WinsockClientSocket()"));

	} while (iResult > 0);

	closesocket(connectSocket);
}

NetworkingDevice* WinsockClientSocket::GetNetworkingDevice()
{ return wd; }

//InputStream* WinsockClientSocket::GetInputStream()
//{ return new WinsockSocketInputStream(connectSocket); }
//
//OutputStream* WinsockClientSocket::GetOutputStream()
//{ return new WinsockSocketOutputStream(connectSocket); }

void WinsockClientSocket::Connect()
{
	if (hasConnected)
		return ;

	addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = ipp;

	addrinfo* result = nullptr;
	if (getaddrinfo(ipaddr.ToASCIIString().GetData(), ASCIIString::From32BitUInt(port).GetData(), &hints, &result) != 0)
		throw Exception(Text("getaddrinfo() failed in WinsockClientSocket::Connect()."));

	// Attempt to connect to an address until one succeeds
	for (addrinfo* ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		// Create a SOCKET for connecting to server
		connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (connectSocket == INVALID_SOCKET)
		{
			freeaddrinfo(result);
			WSACleanup();
			throw Exception(Text("socket() failed in WinsockClientSocket::WinsockClientSocket()."));
		}

		// Connect to server.
		if (connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR)
		{
			closesocket(connectSocket);
			connectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	if (connectSocket == INVALID_SOCKET)
	{
		// failed to connect to the specified IP address
		freeaddrinfo(result);
		WSACleanup();
		throw Exception(Text("Unable to connect to ") + ipaddr.ToString() + Text("\n"));
	}

	freeaddrinfo(result);

	hasConnected = true;
}

////////////////////////////////////////////////////////////////////////////////////////////
// WinsockServerSocket
WinsockServerSocket::WinsockServerSocket(WinsockDevice* wd, IP_PROTOCOL ipp, NetPort port)
: wd(wd), ipp(ipp), port(port)
{
	// Initialize Winsock.
	// Create a socket.
	// Bind the socket.
	// Listen on the socket for a client.
	// Accept a connection from a client.
	// Receive and send data.
	// Disconnect.

	addrinfo* result;
	addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	// IPv4
	hints.ai_family = AF_INET;
	hints.ai_socktype = ipp == IPP_TCP ? SOCK_STREAM : SOCK_DGRAM;
	// IPP
	hints.ai_protocol = ipp;
	// ai_flags : flags that indicate options used in the getaddrinfo function.
	// AI_PASSIVE : The socket address will be used in a call to the bind function.
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL /*"127.0.0.1"*/, ASCIIString::From32BitUInt(port).GetData(), &hints, &result) != 0)
		throw Exception(Text("getaddrinfo() failed in WinsockServerSocket::WinsockServerSocket()"));

	// Create a SOCKET for connecting to server,
	// The socket function creates a socket that is bound to a specific transport service provider.
	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET)
	{
		int lasterr = WSAGetLastError();
		freeaddrinfo(result);
		throw Exception(Text("socket() failed in WinsockServerSocket::WinsockServerSocket()"), lasterr);
	}

	// Setup the TCP listening socket,
	// The bind function associates a local address with a socket.
	if (bind(listenSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR)
	{
		int lasterr = WSAGetLastError();
		freeaddrinfo(result);
		throw Exception(Text("bind() failed in WinsockServerSocket::WinsockServerSocket()"), lasterr);
	}

	// freeaddrinfo() frees address information that the getaddrinfo function dynamically allocates in addrinfo structures.
	freeaddrinfo(result);

	// The listen function places a socket in a state in which it is listening for an incoming connection.
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		int lasterr = WSAGetLastError();
		closesocket(listenSocket);
		throw Exception(Text("bind() failed in WinsockServerSocket::WinsockServerSocket()"), lasterr);
	}

	// Accept a client socket,
	// The accept function permits an incoming connection attempt on a socket.
	clientSocket = accept(listenSocket, NULL, NULL);
	if (clientSocket == INVALID_SOCKET)
	{
		int lasterr = WSAGetLastError();
		closesocket(listenSocket);
		throw Exception(Text("accept() failed in WinsockServerSocket::WinsockServerSocket()"), lasterr);
	}

	is = new WinsockSocketInputStream(clientSocket);
	is->Hold();
	os = new WinsockSocketOutputStream(clientSocket);
	os->Hold();
}

WinsockServerSocket::~WinsockServerSocket()
{
	is->Drop();
	os->Drop();

	// No longer need server socket
	closesocket(listenSocket);

	// shutdown the connection since we're done,
	// The shutdown function disables sends or receives on a socket.
	if (shutdown(clientSocket, SD_SEND) == SOCKET_ERROR)
	{
		closesocket(clientSocket);
		throw Exception(Text("shutdown() failed in WinsockSocketInputStream::~WinsockSocketInputStream()"));
	}

	// cleanup
	closesocket(clientSocket);
}

NetworkingDevice* WinsockServerSocket::GetNetworkingDevice()
{ return wd; }

MAKO_END_NAMESPACE
#endif