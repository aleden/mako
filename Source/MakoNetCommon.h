#pragma once
#include "MakoCommon.h"
#include "MakoMath.h"
#include "MakoString.h"

MAKO_BEGIN_NAMESPACE

//! Transport Layer protocols, such as TCP, UDP, SCTP, and DCCP,
//! specify a source and destination port number in their packet 
//! headers. A port number is a 16-bit unsigned integer, thus 
//! ranging from 0 to 65535. A process associates with a particular 
//! port (known as binding) to send and receive data, meaning that 
//! it will listen for incoming packets whose destination port number 
//! and IP destination address match that port, and/or send outgoing 
//! packets whose source port number is set to that port. Processes 
//! may bind to multiple ports.
//! 
//! There are plenty of uncontested port numbers to choose from in 
//! the "safe" 5000-49151 range. You should avoid port numbers with 
//! patterns to them, or a widely-recognized meaning. People tend to 
//! pick these since they're easy to remember, but this increases the 
//! chances of a collision. Ports 6969, 5150 and 22222 are bad choices, 
//! for example. (Taken from the Winsock Programmer's FAQ (http://tangentsoft.net/wskfaq/))
typedef UInt16 NetPort;

//! Identifies the different IP protocols that are supported. The values
//! of each of these enums are equal to the Berkeley socket library's
//! corrosponding IPPROTO #defines. The documentation for each enum has
//! been taken from the Winsock Programmer's FAQ (http://tangentsoft.net/wskfaq/)
enum IP_PROTOCOL
{
	//! The Transmission Control Protocol is a reliable stream protocol.
	//! "Reliable" means that it always succeeds in sending the data to 
	//! the remote peer: TCP can deal with lost, corrupted, duplicated and 
	//! fragmented packets. "Stream" means that the remote peer sees incoming 
	//! data as a stream of individual bytes: there is no notion of packets, 
	//! from the program's viewpoint.
	//!
	//! TCP can coalesce sending of data, for efficiency: if you make four 
	//! quick sends of data with 100, 50, 30 and 120 bytes in each, TCP is 
	//! likely to pack all these up into a single 300-byte TCP packet when 
	//! it decides to send them out on the network. (This is called the Nagle 
	//! algorithm.) Compare UDP.
	IPP_TCP = 6,

	//! The User Datagram Protocol (UDP) is an alternative to TCP. Sometimes 
	//! you see the term "TCP/IP" used to refer to all basic Internet 
	//! technologies, including UDP, but the proper term is UDP/IP, meaning 
	//! UDP over IP.
	//! 
	//! UDP is an "unreliable" protocol: the stack does not make any effort 
	//! to handle lost, duplicated, or out-of-order packets. UDP packets are 
	//! checked for corruption, but a corrupt UDP packet is simply dropped 
	//! silently.
	//! 
	//! The stack will fragment a UDP datagram when it's larger than the 
	//! network's MTU. The remote peer's stack will reassemble the complete 
	//! datagram from the fragments before it delivers it to the receiving 
	//! application. If a fragment is missing or corrupted, the whole datagram 
	//! is thrown away. This makes large datagrams impractical: an 8K UDP 
	//! datagram will be broken into 6 fragments when sent over Ethernet, for 
	//! example, because it has a 1500 byte MTU. If any of those 6 fragments 
	//! is lost or corrupted, the stack throws away the entire 8K datagram.
	//!
	//! Datagram loss can also occur within the stack at the sender or the 
	//! receiver, usually due to lack of buffer space. It is even possible for 
	//! two communicating programs running on the same machine to have data loss 
	//! if they use UDP. (This actually happens on Windows under high load 
	//! conditions, because it starts dropping datagrams when the stack buffers 
	//! get full.) This limits UDP's value as a local IPC mechanism.
	//! 
	//! If any of these types of loss occur, no notification will be sent to the 
	//! sender or receiver, even if the loss happens within the network stack.
	//! 
	//! Duplicated datagrams are not dropped: they are delivered to the receiver. 
	//! It is up to the application to detect this problem, and it is the program's 
	//! choice what to do with the duplicate datagram.
	//!
	//! UDP datagrams can be delivered in any order. Datagrams often get reordered 
	//! on the network when two datagrams get delivered via different routes, and 
	//! the second datagram's route happens to be quicker.
	//!
	//! From the above discussion, UDP looks pretty useless, right? Well, it does 
	//! have a few advantages over reliable protocols like TCP:
	//!
	//! \li UDP is a slimmer protocol: its protocol header is fixed at 8 bytes, 
	//! whereas TCP's is 20 bytes at minimum and can be more.
	//!
	//! \li UDP has no congestion control and no data coalescing. This eliminates 
	//! the delays caused by the delayed ACK and Nagle algorithms. (This is also a 
	//! disadvantage in many situations, of course.)
	//!
	//! \li There is less code in the UDP section of the stack than the TCP section.
	//! This means that there is less latency between a packet arriving at the 
	//! network card and being delivered to the application.
	//!
	//! \li Only UDP packets can be broadcast or multicast. This makes UDP good for 
	//! applications where timeliness and control is more important than reliability.
	//! Also, some applications are inherently tolerant of UDP problems: data loss in 
	//! a streaming video program just means a frame or two is dropped.
	//!
	//! Be careful not to let UDP's advantages blind you to its bad points: too many 
	//! application writers have started with UDP, and then later been forced to add
	//! reliability features.
	IPP_UDP = 17,
	IPP_ENUM_LENGTH = 2
};

enum IPV4ADDR_PART
{
	IPV4P_1,
	IPV4P_2,
	IPV4P_3,
	IPV4P_4,
	IPV4P_ENUM_LENGTH
};

class IPv4Address
{
	UInt32 addr;

	// Ex: "127.2.5.091"
	template <typename CharType>
	void SetAddrToAnyStringData(const CharType* str)
	{
		static const UInt mul[] = {1, 10, 100};

		UInt part = IPV4P_4;
		UInt8 n = 0;
		UInt c = 0;

		for (const CharType* p = str + (GetAnyStrDataLen(str)-1); p >= str; --p)
		{
			if (p == str)
			{
				n += ((*p - '0') * mul[c]);
				++c;

				SetPart(static_cast<IPV4ADDR_PART>(part), n);
				break;
			}

			if (*p != '.')
			{
				n += ((*p - '0') * mul[c]);
				++c;
			}
			else
			{
				SetPart(static_cast<IPV4ADDR_PART>(part), n);
				n = c = 0;
				--part;
			}
		}
	}
public:
	MAKO_INLINE IPv4Address(UInt8 part1, UInt8 part2, UInt8 part3, UInt8 part4)
		: addr((part1 << 24) | (part2 << 16) | (part3 << 8) | part4) {}
	
	MAKO_INLINE IPv4Address(const ASCIIString& asciiStr) : addr(0)
	{ SetAddrToAnyStringData(asciiStr.GetData()); }

	//! IPv4 Addresses will never require unicode characters, so you never
	//! need to initialize them with StringData, but we provide the constructor
	//! for it anyway since most things use StringData rather than ASCIIStringData.
	MAKO_INLINE IPv4Address(const String& string) : addr(0)
	{ SetAddrToAnyStringData(string.GetData()); }
	
	MAKO_INLINE ~IPv4Address() {}
	
	MAKO_INLINE operator UInt32() const
	{ return addr; }

	MAKO_INLINE String ToString() const
	{
		String addr = String::From32BitUInt(GetPart(IPV4P_1));
		addr.Append(StringChar('.'));
		addr.Append32BitUInt(GetPart(IPV4P_2));
		addr.Append(StringChar('.'));
		addr.Append32BitUInt(GetPart(IPV4P_3));
		addr.Append(StringChar('.'));
		addr.Append32BitUInt(GetPart(IPV4P_4));

		return addr;
	}

	MAKO_INLINE ASCIIString ToASCIIString() const
	{
		ASCIIString addr = ASCIIString::From32BitUInt(GetPart(IPV4P_1));
		addr.Append('.');
		addr.Append32BitUInt(GetPart(IPV4P_2));
		addr.Append('.');
		addr.Append32BitUInt(GetPart(IPV4P_3));
		addr.Append('.');
		addr.Append32BitUInt(GetPart(IPV4P_4));

		return addr;
	}


	MAKO_INLINE UInt8 GetPart(IPV4ADDR_PART part) const
	{ return static_cast<UInt8>((addr >> (Abs(part - 3) * 8)) & 0xFF); }

	MAKO_INLINE void SetPart(IPV4ADDR_PART part, UInt8 val)
	{
		static const UInt32 masks[] = {0x00FFFFFF, 0xFF00FFFF, 0xFFFF00FF, 0xFFFFFF00};
		
		addr &= masks[part];
		addr |= (static_cast<UInt32>(val) << (Abs(static_cast<Int32>(part) - 3) * 8));
	}
};

MAKO_END_NAMESPACE