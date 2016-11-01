#include <Mako.h>
using namespace Mako;

class NetServer : public Mako::SimpleApplication
{
public:
	void Initialize()
	{
		InitNetworking();

		ServerSocket* ss = ND()->CreateServerSocket(IPP_TCP, 25767);
		ss->Hold();
		UInt32 num = ss->GetInputStream()->Read32BitUInt();
		ss->Drop();

		GetOSDevice()->ShowMessageBox(Text("Received data from client"), String::From32BitUInt(num));

		Quit();
	}
};

RUN_MAKO_APPLICATION(NetServer)