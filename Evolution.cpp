/*
https://github.com/cybiosphere
copyright (c) 2005-2014 Frederic RIBERT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

// Evolution.cpp : Defines the entry point for the console application.
//

//#include "CBrain.h"
#include "CBiotop.h"
#include "CMineral.h"
#include "CVegetSpermatophyta.h"
#include "CAnimMammal.h"
#include "CGenome.h"
#include <stdio.h> 
#include <stdarg.h>
#include <fstream>
#include <chrono>
#include <ctime>
#include "CScenarioPlayer.h"
#include "CybioXmlDef.h"
#include "TestsCybioCore.h"
#include "StartupHelpers.h"


CBiotop* pBiotop;
CScenarioPlayer* pScenarioPlayer;

int main(int argc, char* argv[])
{

  //char react;
  printf("Hello World!\n");
  char resuBuffer[512];
  string resuStr;
  string fileIni = "Cybiosphere.ini";

  createBiotopAndScenarioFromIniFile(fileIni, &pBiotop, &pScenarioPlayer);

  int i,j;
  int score;
  int scoreHistory[10];
  TestsCybioCore testsCybioCore("../DataScriptMammal/");

  while (true)
  {
    printf("\nACTIONS:\n");
    printf("0 - Run Tests\n");
    printf("1 - Display all entities\n");
    printf("2 - Display entity details\n");
    printf("3 - Save entity\n");
    printf("4 - Next second\n");
    printf("5 - Next hour\n");
    printf("6 - Next day\n");
    printf("7 - Next Month\n");
    printf("8 - Save biotop\n");
    printf("9 - Exit\n");
    printf("10- Play scenario\n");
    printf("Choice: ");
    char choice;
    scanf("%d",&choice);
    printf ("\n");

    switch(choice)
    {
    case 0:
      printf("Start unit tests\n");
      testsCybioCore.launchAllTests();
      break;
    case 1:
      pBiotop->displayEntities();
      break;
    case 2:
      {
        int uid;
        printf ("Enter entity ID\n");
        scanf("%d",&uid);
        CBasicEntity* pEntity = pBiotop->getEntityById(uid);
        if (pEntity!=NULL)
        {
          printf(pEntity->getLabel().c_str());
		      printf("\n");
          CGenome* pGenome = pEntity->getGenome();
          printf(pGenome->getSpecieName().c_str());
          printf("\n");
		      for (int i=0;i<pEntity->getNumParameter();i++)
		      {
			    printf(pEntity->getParameter(i)->getLabel().c_str());
			    printf(" %6.2f\n",pEntity->getParameter(i)->getVal());
		      }
        }
        // TBD...
        printf ("\n");
        break;
      }
    case 3:
      {
        int uid;
        printf ("Enter entity ID\n");
        scanf("%d",&uid);
        CBasicEntity* pEntity = pBiotop->getEntityById(uid);
        if (pEntity!=NULL)
        {
	        string name =  "../DataScriptMammal/" + pEntity->getLabel() + ".xml";
          pEntity->saveInXmlFile(name);
        }
        printf ("\n");
        break;
      }
    case 4:
      pBiotop->nextSecond();
      break;
    case 5:
      {
        auto start = std::chrono::system_clock::now();
        for (int cpt = 0; cpt < 3600; cpt++)
        {
          pBiotop->nextSecond();
        }
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        printf("Process duration: %.1f sec\n", elapsed_seconds.count());
      }
      break;
    case 6:
      {
        auto start = std::chrono::system_clock::now();
        for (i = 0; i < 3600 * 24; i++)
        {
          pBiotop->nextSecond();
        }
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        printf("Process duration: %.1f sec\n", elapsed_seconds.count());
      }
      break;
    case 7:
    {
      auto start = std::chrono::system_clock::now();
      for (i = 0; i < 31; i++)
      {
        printf("Start day %d\n", i);
        //pBiotop->displayEntities();
        printf("\n");
        for (j = 0; j < 3600 * 24; j++)
        {
          pBiotop->nextSecond();
        }
      }
      auto end = std::chrono::system_clock::now();
      std::chrono::duration<double> elapsed_seconds = end - start;
      printf("Process duration: %.1f sec\n", elapsed_seconds.count());
      break;
    }
    case 8:
      {
        if (pBiotop!=NULL)
        {
	        string name =  "../DataScriptMammal/" + pBiotop->getLabel() + ".bio";
          pBiotop->saveInXmlFile(name, "");
          printf ("Biotop saved\n");
        }
        break;
      }
    case 9:
      delete pBiotop;
      exit(0);
      break;
    case 10:
    {
      CScenarioPlayer player(pBiotop);
      CScenarioPlayer playerChi(pBiotop);
      CScenarioPlayer playerAdu(pBiotop);

      int time;
      char pathName[500];
      string tempFileName;
      printf("Training files path: ");
      scanf("%s", &pathName);

      printf("Number of iteration: ");
      scanf("%d", &time);
      printf("\n");

      for (int i = 0; i < time; i++)
      {
        tempFileName = pathName;
        tempFileName += "\\";
        player.ReadScenarioFile("babyLearning.scp", tempFileName);
        player.PlayScenario();
        printf("Baby : %d / %d\n", player.GetSuccessScore(), player.GetTotalScore());

        tempFileName = pathName;
        tempFileName += "\\";
        playerChi.ReadScenarioFile("childLearning.scp", tempFileName);
        playerChi.PlayScenario();
        printf("Child: %d / %d\n", playerChi.GetSuccessScore(), playerChi.GetTotalScore());

        tempFileName = pathName;
        tempFileName += "\\";
        playerAdu.ReadScenarioFile("adultLearning.scp", tempFileName);
        playerAdu.PlayScenario();
        printf("Adult: %d / %d\n", playerAdu.GetSuccessScore(), playerAdu.GetTotalScore());

        score = 100 * (player.GetSuccessScore() + playerChi.GetSuccessScore() + playerAdu.GetSuccessScore());
        score = score / (player.GetTotalScore() + playerChi.GetTotalScore() + playerAdu.GetTotalScore());
        scoreHistory[i % 10] = score;
        printf("Total: %d / 100\n", score);

        if ((i % 10) == 9)
        {
          score = scoreHistory[0] + scoreHistory[1] + scoreHistory[2] + scoreHistory[3] + scoreHistory[4] + scoreHistory[5] + scoreHistory[6] + scoreHistory[7] + scoreHistory[8] + scoreHistory[9];
          score = score / 10;
          printf("Avarage 10 run: %d / 100\n", score);
        }

      }
    }
    break;
    default:
      break;
    }
  }

  return 0;
}


