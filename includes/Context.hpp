#ifndef CONTEXT_H
#define CONTEXT_H

#include "ft_irc.hpp"
#include "Message.hpp"
#include "Message_Handler.hpp"
#include "reply.hpp"

class User;
class Channel;

class Context
{
	private:
		typedef std::pair<std::string, User *> pair_string_user;
		typedef std::pair<int, User *> pair_int_user;
		typedef std::pair<std::string, Channel *> pair_string_chan;

		std::map<int, User *> unregistered_users;
		std::map<std::string, User *> registered_users;
		std::map<std::string, Channel *> channels;

		Message_Handler * message_handler;

		Context( const Context & src );
		Context & operator=( const Context & rhs );

		void remove_registered_user( User & user );
		void remove_unregistered_user( User & user );

	public:
		Context();
		virtual ~Context();

		void create_unregistered_user( int socket );
		void register_user( User & user );
		void remove_user( User & user );
		void create_channel( User & user, std::string name );
		void remove_channel( Channel & channel );

		void handle_message( User & sender, std::string raw_message );

		User & get_user_by_socket( int socket );
		User & get_user_by_nick( std::string nickname );
		bool does_user_with_nick_exist( std::string nickname );

		Channel & get_channel_by_name( std::string name );
		bool does_channel_exist( std::string name );

		void debug_print_unregistered_users( void ) const;
		void debug_print_registered_users( void ) const;
};

#endif /* CONTEXT_H */
