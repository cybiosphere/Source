//FRED #include <ClanLib/core.h>
//FRED #include <ClanLib/network.h>

#include "API/core.h"
#include "API/network.h"

using namespace clan;
#include "clan_server_user.h"

ServerUser::ServerUser()
: connection(0), id(0), isNextSecondCompleted(true)
{
}

void ServerUser::attach_to_connection(NetGameConnection *connection)
{
	this->connection = connection;

	if(connection)
		connection->set_data("serveruser", this);
}

ServerUser *ServerUser::get_user(NetGameConnection* connection)
{
	if(connection)
		return reinterpret_cast<ServerUser*>(connection->get_data("serveruser"));
	else
		return 0;
}

void ServerUser::send_event(const NetGameEvent &gameEvent)
{
	if(connection)
		connection->send_event(gameEvent);
}
