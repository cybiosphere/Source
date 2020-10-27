#pragma once

//FRED #include <ClanLib/core.h>
#include "API/core.h"

using namespace clan;

namespace clan {
class NetGameConnection;
class NetGameEvent;
}

class ServerUser
{
public:
	static ServerUser *get_user(NetGameConnection* connection);

public:
	ServerUser();

	void attach_to_connection(NetGameConnection *connection);

	void send_event(const NetGameEvent &gameEvent);

public:
	int id;
	std::string user_name;
	bool isCoprocessor;
	bool isNextSecondCompleted;

private:
	NetGameConnection *connection;
};
