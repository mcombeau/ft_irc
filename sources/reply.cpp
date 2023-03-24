#include "reply.hpp"
#include "ft_irc.hpp"
#include "reply_macros.hpp"
#include "reply_macros_error.hpp"
#include "Channel.hpp"

std::string const rpl::welcome( User & user )
{
	std::string reply = SERVER_PREFIX " ";
	reply += RPL_WELCOME " ";
	reply += user.get_nickname();
	reply += " :Welcome to the Internet Relay Network ";
	reply += user.get_identifier();
	reply += "\r\n";
	return ( reply );
}

std::string const rpl::yourhost( User & user )
{
	std::string reply = SERVER_PREFIX " ";
	reply += RPL_YOURHOST " ";
	reply += user.get_nickname();
	reply += " :Your host is ";
	reply += SERVER_NAME;
	reply += ", running version ";
	reply += SERVER_VERSION;
	reply += "\r\n";
	return ( reply );
}

std::string const rpl::created( User & user )
{
	std::string reply = SERVER_PREFIX " ";
	reply += RPL_CREATED " ";
	reply += user.get_nickname();
	reply += " :This server was created ";
	reply += SERVER_CREATION;
	reply += "\r\n";
	return ( reply );
}

std::string const rpl::myinfo( User & user )
{
	std::string reply = SERVER_PREFIX " ";
	reply += RPL_MYINFO " ";
	reply += user.get_nickname();
	reply += " :";
	reply += SERVER_NAME " ";
	reply += SERVER_VERSION " ";
	reply += "io i";
	reply += "\r\n";
	return ( reply );
}

std::string const rpl::server_version( User & user )
{
	std::string reply = SERVER_PREFIX " ";
	reply += RPL_VERSION " ";
	reply += user.get_nickname();
	reply += " " SERVER_VERSION " " SERVER_NAME;
	reply += " :N/A\r\n";
	return ( reply );
}

/* When replying to an ADMIN message, a server */
/* is expected to use replies RPL_ADMINME */
/* through to RPL_ADMINEMAIL and provide a text */
/* message with each.  For RPL_ADMINLOC1 a */
/* description of what city, state and country */
/* the server is in is expected, followed by */
/* details of the institution (RPL_ADMINLOC2) */
/* and finally the administrative contact for the */
/* server (an email address here is REQUIRED) */
/* in RPL_ADMINEMAIL. */
std::string const rpl::adminme( User & user )
{
	std::string reply = SERVER_PREFIX " ";
	reply += RPL_ADMINME " ";
	reply += user.get_nickname();
	reply += " " SERVER_NAME;
	reply += " :Administrative info\r\n";
	return ( reply );
}

std::string const rpl::adminloc1( User & user )
{
	std::string reply = SERVER_PREFIX " ";
	reply += RPL_ADMINLOC1 " ";
	reply += user.get_nickname();
	reply += " :";
	reply += ADMIN_INFO_1 "\r\n";
	return ( reply );
}

std::string const rpl::adminloc2( User & user )
{
	std::string reply = SERVER_PREFIX " ";
	reply += RPL_ADMINLOC2 " ";
	reply += user.get_nickname();
	reply += " :";
	reply += ADMIN_INFO_2 "\r\n";
	return ( reply );
}

std::string const rpl::adminemail( User & user )
{
	std::string reply = SERVER_PREFIX " ";
	reply += RPL_ADMINEMAIL " ";
	reply += user.get_nickname();
	reply += " :";
	reply += ADMIN_EMAIL "\r\n";
	return ( reply );
}

/* A server responding to an INFO message is required to */
/* send all its 'info' in a series of RPL_INFO messages */
/* with a RPL_ENDOFINFO reply to indicate the end of the */
/* replies. */
std::string const rpl::info( User & user, int line )
{
	std::string reply = SERVER_PREFIX " ";
	reply += RPL_INFO " ";
	reply += user.get_nickname();
	reply += " :";
	switch ( line )
	{
		case 0:
			reply += "Server name: " SERVER_NAME "\r\n";
			break;
		case 1:
			reply += "Version: " SERVER_VERSION "\r\n";
			break;
		case 2:
			reply += "Created on: " SERVER_CREATION "\r\n";
			break;
	}
	return ( reply );
}

std::string const rpl::info_end( User & user )
{
	std::string reply = SERVER_PREFIX " ";
	reply += RPL_ENDOFINFO " ";
	reply += user.get_nickname();
	reply += " :End of INFO list\r\n";
	return ( reply );
}

std::string const rpl::forward( User & sender, Message & message )
{
	std::string reply = ":";
	reply += sender.get_identifier();
	reply += " ";
	reply += message.get_command();
	reply += " ";
	reply += message.get( "msgtarget" );
	reply += " :";
	reply += message.get( "text to send" );
	reply += "\r\n";
	return ( reply );
}

std::string const rpl::confirmation( std::string const identifier,
                                     Message & message )
{
	std::string reply = ":";
	reply += identifier;
	reply += " ";
	reply += message.get_command();
	reply += " ";
	reply += message.get( "nickname" );
	reply += "\r\n";
	return ( reply );
}

/* :coucou!~b@localhost JOIN :#test */
std::string const rpl::join_channel( User & user, Channel & channel )
{
	std::string reply = ":";
	reply += user.get_identifier();
	reply += " JOIN :";
	reply += channel.get_name();
	reply += "\r\n";
	return ( reply );
}

/* To reply to a NAMES message, a reply pair consisting */
/* of RPL_NAMREPLY and RPL_ENDOFNAMES is sent by the */
/* server back to the client.  If there is no channel */
/* found as in the query, then only RPL_ENDOFNAMES is */
/* returned.  The exception to this is when a NAMES */
/* message is sent with no parameters and all visible */
/* channels and contents are sent back in a series of */
/* RPL_NAMEREPLY messages with a RPL_ENDOFNAMES to mark */
/* the end. */
/* Message format: "( "=" / "*" / "@" ) <channel> :[ "@" / "+" ] <nick> *( " " [ "@" / "+" ] <nick> )" */
/* "@" is used for secret channels, "*" for private */
/* channels, and "=" for others (public channels). */
std::string const rpl::namreply( User & user, Channel & channel )
{
	std::string reply = SERVER_PREFIX " ";
	reply += RPL_NAMREPLY " ";
	reply += user.get_nickname();
	reply += " = ";
	reply += channel.get_name();
	reply += " :";
	reply += channel.get_user_list();
	reply += "\r\n";
	return ( reply );
}

std::string const rpl::endofnames( User & user, Channel & channel )
{
	std::string reply = SERVER_PREFIX " ";
	reply += RPL_ENDOFNAMES " ";
	reply += user.get_nickname();
	reply += " ";
	reply += channel.get_name();
	reply += " :End of NAMES list\r\n";
	return ( reply );
}

