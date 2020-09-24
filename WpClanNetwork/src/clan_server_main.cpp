#include "clan_server.h"
#include "CBiotop.h"

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

    CBiotop* pBiotop;
    char resuBuffer[512];
    string resuStr;
    string fileIni = "Cybiosphere.ini";
    if (argc == 2)
    {
      fileIni = argv[1];
      log_event("Start ", "Open ini file " + fileIni);
    }
    string serverPortStr;
    if (getStringSectionFromFile("CYBIOSPHERE", "ServerPort", "4556", resuBuffer, 512, fileIni) > 0)
    {
      serverPortStr = resuBuffer;
    }
    else
    {
      serverPortStr = "4556";
    }

    int resu = getStringSectionFromFile("CYBIOSPHERE", "Biotop", "", resuBuffer, 512, fileIni);
    resuStr = resuBuffer;

    if (resuStr != "")
    {
      string resuDataPath;
      pBiotop = new CBiotop(0,0,0);
      bool resu = getStringSectionFromFile("CYBIOSPHERE", "DataPath", "", resuBuffer, 512, fileIni);
      resuDataPath = resuBuffer;
      if (resuDataPath != "")
        pBiotop->loadFromXmlFile(resuStr, resuDataPath);
      else
        pBiotop->loadFromXmlFile(resuStr, "..\\dataXml\\");
      log_event("Biotop  ", "Biotop loaded");
    }
    else
    {
      pBiotop = new CBiotop(80,40,3);
      pBiotop->initGridDefaultLayerType();
      pBiotop->initGridDefaultAltitude();
      pBiotop->initGridEntity();
      pBiotop->setDefaultEntitiesForTest();
      log_event("Biotop  ", "Default empty biotop created");
    }

		Server server(serverPortStr, pBiotop);
		server.exec();

    delete pBiotop;
		return 0;
	}
	catch (Exception e)
	{
#ifdef WIN32
		//FRED MessageBox(0, e.get_message_and_stack_trace().c_str(), TEXT("Unhandled Exception"), MB_OK);	
#else
		Console::write_line("Unhandled exception: %1", e.get_message_and_stack_trace());
#endif
		return 1;
	}
}
