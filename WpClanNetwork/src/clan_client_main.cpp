#include <API/core.h>
#include <API/network.h>
using namespace clan;

#include "clan_client.h"

//FRED #ifdef WIN32
//FRED int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
//FRED #else
int main(int, char**)
//FRED #endif
{
	try
	{
		ConsoleWindow console("Client Console", 160, 1000);
		ConsoleLogger logger;

    // Get Server info in ini file
    string ServerAddrStr;
    string ServerPortStr;
    char resuBuffer[512];
    string fileIni = "Cybiosphere.ini";
    int resu = getStringSectionFromFile("CYBIOSPHERE", "ServerAddr", "localhost", resuBuffer, 512, fileIni);
    ServerAddrStr = resuBuffer;
    resu = getStringSectionFromFile("CYBIOSPHERE", "ServerPort", "4556", resuBuffer, 512, fileIni);
    ServerPortStr = resuBuffer;

		Client client(ServerAddrStr, ServerPortStr);
		client.exec();

		return 0;
	}
	catch (Exception e)
	{
#ifdef WIN32
//FRED		MessageBox(0, e.get_message_and_stack_trace().c_str(), TEXT("Unhandled Exception"), MB_OK);	
#else
		Console::write_line("Unhandled exception: %1", e.get_message_and_stack_trace());
#endif

		return 1;
	}
}
