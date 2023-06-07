#include "log_event.hpp"

void log_event::info( std::string msg )
{
	std::stringstream ss;

	ss << RESET "[INFO] " << msg << RESET;
	std::cerr << ss.str() << std::endl;
}

void log_event::warn( std::string msg )
{
	std::stringstream ss;

	ss << YELLOW "[WARN] " << msg << RESET;
	std::cerr << ss.str() << std::endl;
}

void log_event::warn( std::string msg, std::string command )
{
	std::string formatted_command = get_formatted_command( command );

	std::stringstream ss;
	ss << YELLOW "[WARN] " << msg << ": [" << formatted_command << "]" << RESET;
	std::cerr << ss.str() << std::endl;
}

void log_event::error( std::string msg )
{
	std::stringstream ss;

	ss << RED "[ERROR] " << msg << RESET;
	std::cerr << ss.str() << std::endl;
}

void log_event::reply( int socket, std::string msg )
{
	std::stringstream ss;

	ss << CYAN "[REPLY][Socket " << socket << "] " << msg << RESET;
	std::cerr << ss.str() << std::endl;
}

void log_event::command( int socket, std::string command )
{
	std::string formatted_command = get_formatted_command( command );
	std::stringstream ss;

	ss << PURPLE "[COMMAND][Socket " << socket << "]: [" << formatted_command << "]" << RESET;
	std::cerr << ss.str() << std::endl;
}

std::string log_event::get_formatted_command( std::string command )
{
	std::stringstream ss;

	for( size_t i = 0; i < command.length(); i++ )
	{
		switch ( command[i] )
		{
			case '\n':
				ss << "\\n";
				break ;
			case '\r':
				ss << "\\r";
				break ;
			case '\b':
				ss << "\\b";
				break ;
			case '\t':
				ss << "\\t";
				break ;
			case '\a':
				ss << "\\a";
				break ;
			case '\v':
				ss << "\\v";
				break ;
			case '\0':
				ss << "\\0";
				break ;
			default:
				ss << command[i];
		}
	}
	return ( ss.str() );
}
