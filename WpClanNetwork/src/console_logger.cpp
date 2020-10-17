/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

#include "Core/precomp.h"
#include "API/Core/Text/console_logger.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"

namespace clan
{
	ConsoleLogger::ConsoleLogger()
	{
    m_currentCommand = "";
#ifdef WIN32
		AllocConsole();
#endif
	}

	ConsoleLogger::~ConsoleLogger()
	{
	}

	void ConsoleLogger::log(const std::string &type, const std::string &text)
	{
		StringFormat format = get_log_string(type, text);

#ifdef WIN32
		std::wstring log_line = StringHelp::utf8_to_ucs2(format.get_result());

		DWORD bytesWritten = 0;

		WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), log_line.data(), log_line.size(), &bytesWritten, 0);
#else
		std::string log_line = StringHelp::text_to_local8(format.get_result());
		write(1, log_line.data(), log_line.length());
#endif
	}

  bool ConsoleLogger::get_console_input(std::string &input_cmd) //FRI
  {
    bool newCommandReady = false;
#ifdef WIN32
    INPUT_RECORD irInBuf[128];  // a record of input events
    DWORD cNumEvent = 0; // how many events took place
    DWORD cNumRead = 0; // how many events took place
    input_cmd = "";

    // Check event
    GetNumberOfConsoleInputEvents(GetStdHandle(STD_INPUT_HANDLE), &cNumEvent);
    if (cNumEvent<1)
      return false;

    // read and process event
    if (ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), irInBuf, 128, &cNumRead) == 0)
      return false;
    
    for (int i = 0; i < cNumRead; i++) 
    {
      if(irInBuf[i].EventType == KEY_EVENT && irInBuf[i].Event.KeyEvent.bKeyDown)
      { // A key was pressed, so return it.
        DWORD bytesWritten = 0;
        if ((irInBuf[i].Event.KeyEvent.uChar.AsciiChar) == '\r')
        {
          WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), "\n", 1, &bytesWritten, 0);
          //log("Command:", m_currentCommand);
          input_cmd = m_currentCommand;
          m_currentCommand = "";
          newCommandReady = true;
        }
        else if ((irInBuf[i].Event.KeyEvent.uChar.AsciiChar) != '\0')
        {
          m_currentCommand+=irInBuf[i].Event.KeyEvent.uChar.AsciiChar;
          WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), &(irInBuf[i].Event.KeyEvent.uChar), 1, &bytesWritten, 0);
        }
      }
    }
#endif
    return newCommandReady;
  }

}
