#include <Mako.h>
using namespace Mako;

class NetClient : public Mako::SimpleApplication
{
public:
	void Initialize()
	{
		InitNetworking();

		ClientSocket* cs = ND()->CreateClientSocket(ASCIIText("127.0.0.1"), IPP_UDP, 25767);
		cs->Hold();
		cs->GetOutputStream()->Write32BitUInt(0xABCDEFAB);
		cs->Drop();

		Quit();
	}
};

RUN_MAKO_APPLICATION(NetClient)