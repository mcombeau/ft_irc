#include "../includes/Application.hpp"
#include "../includes/ft_irc.hpp"
#include <netinet/in.h>
#include <poll.h>

#define RPL_WELCOME "001"

std::string const welcome()
{
	std::string reply = "127.0.0.1 ";
	reply += RPL_WELCOME " ";
	reply += "alice";
	reply += " :Welcome to the Internet Relay Network ";
	reply += "alice!alice@alicehost";
	reply += "\r\n";
	return reply;
}

Application::Application()
{
	context = new Context();

	std::cout << "Creating server socket..." << std::endl;
	server.fd = socket( AF_INET, SOCK_STREAM, 0 );

	if ( server.fd == -1 )
	{
		std::cerr << "Can't create a socket!";
		throw std::runtime_error( "Can't create a socket!" );
	}

	server.info.sin_family = AF_INET;
	// TODO: try multiple ports
	server.info.sin_port = htons( 6667 ); // TODO test other ports ?
	server.info.sin_addr.s_addr = htonl( INADDR_ANY );

	std::cout << "Binding socket to sockaddr..." << std::endl;
	if ( bind( server.fd, ( struct sockaddr * ) &server.info,
	           sizeof( server.info ) ) == -1 )
	{
		std::cerr << "Can't bind to IP/port";
		throw std::runtime_error( "Can't bind to IP/port" );
	}
	std::cout << "Mark the socket for listening..." << std::endl;
	if ( listen( server.fd, SOMAXCONN ) == -1 )
	{
		std::cerr << "Can't listen !";
		throw std::runtime_error( "Can't listen !" );
	}
}

void Application::launch_server()
{
	std::vector<pollfd> client_fds( max_clients + 1 );
	client_fds[0].fd = server.fd;
	client_fds[0].events = POLLIN;

	int num_clients = 0;  // keep track of number of connected clients

	while ( true )
	{
		int num_ready = poll( client_fds.data(), num_clients + 1, -1 );
		if ( num_ready == -1 )
		{
			std::cerr << "Error in poll()";
			throw std::runtime_error( "Error in poll()" );
		}

		if ( client_fds[0].revents & POLLIN )
		{
			socklen_t clientSize = sizeof( clients.info );

			std::cout << "Accept client call..." << std::endl;
			clients.fd = accept( server.fd, ( struct sockaddr * ) &clients.info,
			                     &clientSize );

			std::cout << "Received call..." << std::endl;
			if ( clients.fd == -1 )
			{
				std::cerr << "Problem with client connecting!";
				throw std::runtime_error( "Problem with client connecting!" );
			}

			// add new client to the list of file descriptors to monitor
			if ( num_clients == max_clients )
			{
				std::cerr << "Too many clients!";
				throw std::runtime_error( "Too many clients!" );
			}
			client_fds[num_clients + 1].fd = clients.fd;
			client_fds[num_clients + 1].events = POLLIN;

			// Creating new user for client
			context->create_unregistered_user( clients.fd );

			num_clients++;
		}
		int i = 1;
		while ( i <= num_clients )
		{
			if ( client_fds[i].fd != -1 && client_fds[i].revents & POLLIN )
			{
				read_message( client_fds[i].fd, &num_clients );
			}
			i++;
		}
	}
	close( server.fd );
}

void examineChar( char c )
{
	if( c == '\n' )
	{
		std::cout << "\\n";
	}
	else if( c == '\r' )
	{
		std::cout << "\\r";
	}
	else if( c == '\b' )
	{
		std::cout << "\\b";
	}
	else if( c == '\t' )
	{
		std::cout << "\\t";
	}
	else if( c == '\a' )
	{
		std::cout << "\\a";
	}
	else if( c == '\v' )
	{
		std::cout << "\\v";
	}
	else if( c == '\0' )
	{
		std::cout << "\\0";
	}
	else
	{
		std::cout << c;
	}
}

void examineCharStar( char *cs )
{
	size_t i;
	for( i = 0; i < std::strlen( cs ); i++ )
	{
		examineChar( cs[i] );
	}
}

void examineString( std::string s )
{
	size_t i;
	if( s != "" )
		for( i = 0; i < s.length(); i++ )
		{
			examineChar( s[i] );
		}
}

void Application::read_message( int fd, int *num_clients )
{
	char buf[4096];
	memset( buf, 0, sizeof( buf ) );
	size_t terminator = std::string::npos;
	int bytes_recv = 0;

	std::string message_buffer;
	while ( terminator == std::string::npos )
	{
		bytes_recv = recv( fd, buf, sizeof( buf ), 0 );
		std::cerr << "- BUFFER contains: [" << buf << "]" << std::endl;
		if ( bytes_recv == -1 )
		{
			std::cerr << "There was a connection issue." << std::endl;
			break;
		}
		if ( bytes_recv == 0 )
		{
			std::cout << "The client disconnected" << std::endl;
			close( fd );
			fd = -1;
			( *num_clients )--;
			break;
		}
		message_buffer += std::string( buf );
		std::cerr << "- STRING BUFFER contains: [";
		examineString( message_buffer );
		std::cerr << "]" << std::endl;
		terminator = message_buffer.find( "\r\n", 0 );
		std::cerr << "- TERMINATOR is pos :" << terminator << std::endl;
	}
	if ( terminator != std::string::npos )
	{
		size_t pos = 0;
		while ( terminator != std::string::npos )
		{
			std::string first_command = message_buffer.substr( pos, terminator + 2 );
			/* std::string received = std::string( buf, 0, bytes_recv ); */

			std::cout << "---------------------------------" << std::endl;
			std::cout << "--- Received: [" << message_buffer << "]" << std::endl;
			std::cout << "--- FIRST COMMAND: [";
			examineString( first_command );
			std::cout << "]" << std::endl;

			std::cout << "---------------------------------" << std::endl;

			// TODO : check for incomplete messages / read until \r\n
			context->handle_message( context->get_user_by_socket( fd ),
			                         first_command );

			pos = terminator + 2;
			terminator = message_buffer.find( "\r\n", pos );
			std::cout << "position : " << pos << std::endl;
			examineChar( message_buffer[pos] ) ;
			std::cout << std::endl;
			std::cout << "terminator : " << terminator << std::endl;
			examineChar( message_buffer[terminator] ) ;
			std::cout << std::endl;
		}
	}

	/* std::string response = welcome(); */
	/* send( client_fds[i].fd, response.c_str(), response.length() + 1, 0 ); */
}

Application::~Application()
{
	delete context;
}
