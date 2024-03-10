#include <API/core.h>
#include <API/network.h>
using namespace clan;

#include "clan_client.h"
#include "StartupHelpers.h"

//FRED #ifdef WIN32
//FRED int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
//FRED #else
int main(int argc, char* argv[])
//FRED #endif
{
  try
  {
    ConsoleWindow console("Client Console", 160, 1000);
    ConsoleLogger logger;
    string fileIni = "Cybiosphere.ini";

    if (argc == 2)
    {
      fileIni = argv[1];
      log_event("Start ", "Use specific ini file " + fileIni);
    }

    string serverAddrStr = getIpServerAddrFromIniFile(fileIni);
    string serverPortStr = getIpPortStrFromIniFile(fileIni);
    string logInStr = getIpServerLoginFromIniFile(fileIni);

    Client client(serverAddrStr, serverPortStr, logInStr);
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
