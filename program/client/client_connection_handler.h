#include <string>
#include "connection.h"

namespace client{
	class client_connection_handler{
	public:
		client_connection_handler(client_server::Connection* connection) : conn(connection){};
		~client_connection_handler();

		bool send_command_list_ng();
		bool send_command_create_ng(std::string ng_name);
		bool send_command_delete_ng(unsigned int ng_id);
		bool send_command_list_art(unsigned int ng_id);
		bool send_command_create_art(unsigned int ng_id, std::string art_title, std::string art_author, std::string art_text);
		bool send_command_delete_art(unsigned int ng_id, unsigned int art_id);
		bool send_command_get_art(unsigned int ng_id, unsigned int art_id);
	private:
		client_server::Connection* conn;
	};
}