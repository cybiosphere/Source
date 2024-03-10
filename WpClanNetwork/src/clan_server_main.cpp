#include "clan_server.h"
#include "event_definitions.h"
#include "CBiotop.h"
#include "CScenarioPlayer.h"
#include "StartupHelpers.h"

//#ifdef WIN32
//int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
//#else
int main(int argc, char* argv[])
//#endif
{
	try
	{
		ConsoleWindow console("Server Console", 160, 1000);
		ConsoleLogger logger;
    CBiotop* pBiotop{nullptr};
    CScenarioPlayer* pScenarioPlayer{nullptr};
    string fileIni = "Cybiosphere.ini";

    if (argc == 2)
    {
      fileIni = argv[1];
      log_event(labelServer, "Open ini file " + fileIni);
    }
    string serverPortStr = getIpPortStrFromIniFile(fileIni);
    createBiotopAndScenarioFromIniFile(fileIni, &pBiotop, &pScenarioPlayer);

    if (pBiotop != nullptr)
      log_event(labelServer, "Biotop loaded: %1", pBiotop->getLabel());
    if (pScenarioPlayer != nullptr)
      log_event(labelServer, "Scenario loaded");

    double startupSpeed = getStartupSpeedFromIniFile(fileIni);
    log_event(labelServer, "Starup speed %1", startupSpeed);
 
    Server server(serverPortStr, pBiotop, pScenarioPlayer, startupSpeed);
    server.exec();

    delete pBiotop;
    delete pScenarioPlayer;
    return 0;
	}
	catch (Exception e)
	{
#ifdef WIN32
		// MessageBox(0, e.get_message_and_stack_trace().c_str(), TEXT("Unhandled Exception"), MB_OK);	
#else
		Console::write_line("Unhandled exception: %1", e.get_message_and_stack_trace());
#endif
		return 1;
	}
}
