#include "clan_server.h"
#include "CBiotop.h"

//#ifdef WIN32
//int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
//#else
int main(int, char**)
//#endif
{
	try
	{
		ConsoleWindow console("Console", 160, 1000);
		ConsoleLogger logger;

    CBiotop* pBiotop;
    char resuBuffer[512];
    string resuStr;
    string fileIni = "Cybiosphere.ini";
    int resu = getStringSectionFromFile("CYBIOSPHERE", "Biotop", "", resuBuffer, 512, fileIni);
    resuStr = resuBuffer;

    if (resuStr != "")
    {
      string resuDataPath;
      pBiotop = new CBiotop(0,0,0);
      BOOL resu = getStringSectionFromFile("CYBIOSPHERE", "DataPath", "", resuBuffer, 512, fileIni);
      resuDataPath = resuBuffer;
      if (resuDataPath != "")
        pBiotop->loadFromXmlFile(resuStr, resuDataPath);
      else
        pBiotop->loadFromXmlFile(resuStr, "..\\dataXml\\");
      log_event("Biotop", "Biotop loaded");
    }
    else
    {
      pBiotop = new CBiotop(80,40,3);
      pBiotop->initGridDefaultLayerType();
      pBiotop->initGridDefaultAltitude();
      pBiotop->initGridEntity();
      pBiotop->setDefaultEntitiesForTest();
      log_event("biotop", "Default empty biotop created");
    }

		Server server(pBiotop);
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
