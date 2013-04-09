#include "client_connection_handler.h"
#include <string>
#include "connection.h"
#include <vector>
#include "protocol.h"
#include <iostream>

namespace client{

int read_int(client_server::Connection* conn){
	char c[4];
	c[0] = conn->read();
	c[1] = conn->read();
	c[2] = conn->read();
	c[3] = conn->read();
	int i = ((c[0] << 24) | (c[1] << 16) | (c[2] << 8) | c[3]);
	return i;
}

void int_to_byte_array(unsigned int n, unsigned char* c){
	c[0] = (n >> 24) & 0xFF;
	c[1] = (n >> 16) & 0xFF;
	c[2] = (n >> 8) & 0xFF;
	c[3] = n & 0xFF;
}

std::string read_string(unsigned int n,client_server::Connection* conn){
	std::vector<char> str;
	for(unsigned int i = 0; i < n; ++i){
		str.push_back(conn->read());
	}
	std::string ret(str.begin(),str.end());
	return ret;
}

void write_string(std::string text, client_server::Connection* conn){
	for(unsigned int i = 0; i < text.size(); ++i){
		conn->write(text[i]);
	}
}

void write_int(int N, client_server::Connection* conn){
	unsigned char bytes[4];
	int_to_byte_array(N, bytes);
	conn->write(bytes[0]);
	conn->write(bytes[1]);
	conn->write(bytes[2]);
	conn->write(bytes[3]);
}

client_connection_handler::client_connection_handler(client_server::Connection* connection){
	conn = connection;
}

client_connection_handler::~client_connection_handler(){
	
}

bool client_connection_handler::send_command_list_ng(){
	conn->write(protocol::Protocol::COM_LIST_NG);
	conn->write(protocol::Protocol::COM_END);
	unsigned char com = conn->read();
	std::vector<unsigned int> ng_ids;
	std::vector<std::string> ng_names;
	if(com == protocol::Protocol::ANS_LIST_NG){
		com = conn->read();
		if(com == protocol::Protocol::PAR_NUM){
			unsigned int nbr_ng = read_int(conn);
			for(unsigned int i = 0; i < nbr_ng; ++i){
				com = conn->read();
				if(com != protocol::Protocol::PAR_NUM){
					return false;
				}
				unsigned int id = read_int(conn);
				com = conn->read();
				if(com != protocol::Protocol::PAR_STRING){
					return false;
				}
				unsigned int str_len = read_int(conn);
				std::string name = read_string(str_len,conn);
				ng_ids.push_back(id);
				ng_names.push_back(name);
			}
		}else{
			return false; //something is wrong with server
		}
	}else{
		return false; //something is wrong with server
	}
	for(unsigned int t = 0; t < ng_ids.size();++t){
		std::cout<<ng_ids[t]<<" : "<<ng_names[t]<<std::endl;
	}
	return true;
}

bool client_connection_handler::send_command_create_ng(std::string ng_name){
	conn->write(protocol::Protocol::COM_CREATE_NG);
	conn->write(protocol::Protocol::PAR_STRING);
	write_int(ng_name.size(), conn);
	write_string(ng_name, conn);
	conn->write(protocol::Protocol::COM_END);
	unsigned char com = conn->read();
	if(com != protocol::Protocol::ANS_CREATE_NG){
		return false;
	}
	com = conn->read();
	if(com == protocol::Protocol::ANS_NAK){
		com = conn->read();
		if(com != protocol::Protocol::ERR_NG_ALREADY_EXISTS){
			return false;
		}
	}else if(com != protocol::Protocol::ANS_ACK){
		return false;
	}
	com = conn->read();
	if(com != protocol::Protocol::ANS_END){
		return false;
	}
	return true;
}

bool client_connection_handler::send_command_delete_ng(unsigned int ng_id){
	conn->write(protocol::Protocol::COM_DELETE_NG);
	conn->write(protocol::Protocol::PAR_NUM);
	write_int(ng_id,conn);
	conn->write(protocol::Protocol::COM_END);
	unsigned char com = conn->read();
	if(com != protocol::Protocol::ANS_DELETE_NG){
		return false; //Something went wrong
	}
	com = conn->read();
	if(com != protocol::Protocol::ANS_ACK){
		if(com != protocol::Protocol::ANS_NAK){
			return false; //Something went wrong
		}else{
			com = conn->read();
			if(com != protocol::Protocol::ERR_NG_DOES_NOT_EXIST){
				return false; //Something went wrong
			}
		}
	}
	com = conn->read();
	if(com != protocol::Protocol::ANS_END){
		return false; //Something went wrong
	}
	return true;
}

bool client_connection_handler::send_command_list_art(unsigned int ng_id){

	conn->write(protocol::Protocol::COM_LIST_ART);
	conn->write(protocol::Protocol::PAR_NUM);
	write_int(ng_id, conn);
	conn->write(protocol::Protocol::COM_END);
	unsigned char com = conn->read();
	if(com!=protocol::Protocol::ANS_LIST_ART){
		return false;
	}
	else{
		com = conn->read();
		if(com == protocol::Protocol::ANS_ACK){
			com = conn->read();
			int nbr_arts = read_int(conn);
			std::cout<<"Number of articles: "<<nbr_arts<<std::endl;
			com = conn->read();
			for(unsigned int i = 0; i<nbr_arts;++i){
				com = conn->read();
				int art_id = read_int(conn);
				std::cout<<"Article ID: "<<art_id<<std::endl;
				com = conn->read();
				if(com!=protocol::Protocol::PAR_STRING){
					return false;
				}
				int length = read_int(conn);
				std::string content = read_string(length,conn);		
				std::cout<<content<<std::endl;	
			}
			return true;
		}
		else {
			return false;
		}
	}
	return false;
}

bool client_connection_handler::send_command_create_art(unsigned int ng_id, std::string art_title, std::string art_author, std::string art_text){
	conn->write(protocol::Protocol::COM_CREATE_ART);
	conn->write(protocol::Protocol::PAR_NUM);
	write_int(ng_id,conn);

	conn->write(protocol::Protocol::PAR_STRING);
	write_int(art_title.size(),conn);
	write_string(art_title,conn);

	conn->write(protocol::Protocol::PAR_STRING);
	write_int(art_author.size(),conn);
	write_string(art_author,conn);

	conn->write(protocol::Protocol::PAR_STRING);
	write_int(art_text.size(),conn);
	write_string(art_text,conn);

	conn->write(protocol::Protocol::COM_END);

	unsigned char com = conn->read();
	if(com != protocol::Protocol::ANS_CREATE_ART){
		return false; //Something went wrong
	}
	com = conn->read();
	if(com != protocol::Protocol::ANS_ACK){
		if(com != protocol::Protocol::ANS_NAK){
			return false; //Something wrong
		}else{
			com = conn->read();
			if(com != protocol::Protocol::ERR_NG_DOES_NOT_EXIST){
				return false; //Something went wrong
			}
		}
	}
	com = conn->read();
	if(com != protocol::Protocol::ANS_END){
		return false; //Something is wrong
	}
	return true;
}

bool client_connection_handler::send_command_delete_art(unsigned int ng_id, unsigned int art_id){
	conn->write(protocol::Protocol::COM_DELETE_ART);
	conn->write(protocol::Protocol::PAR_NUM);
	write_int(ng_id,conn);
	conn->write(protocol::Protocol::PAR_NUM);
	write_int(art_id,conn);
	conn->write(protocol::Protocol::COM_END);

	unsigned char com = conn->read();
	if(com != protocol::Protocol::ANS_DELETE_ART){
		return false; //Something went wrong
	}
	com = conn->read();
	if(com != protocol::Protocol::ANS_ACK){
		if(com != protocol::Protocol::ANS_NAK){
			return false; //Something went wrong
		}else{
			com = conn->read();
			if(com != protocol::Protocol::ERR_NG_DOES_NOT_EXIST && com != protocol::Protocol::ERR_ART_DOES_NOT_EXIST){
				return false; //Something went wrong
			}
		}
	}
	com = conn->read();
	if(com != protocol::Protocol::ANS_END){
		return false; //Something went wrong
	}
	return true;
}

bool client_connection_handler::send_command_get_art(unsigned int ng_id, unsigned int art_id){
	conn->write(protocol::Protocol::COM_GET_ART);
	conn->write(protocol::Protocol::PAR_NUM);
	write_int(ng_id,conn);
	conn->write(protocol::Protocol::PAR_NUM);
	write_int(art_id,conn);
	conn->write(protocol::Protocol::COM_END);
	std::string title;
	std::string author;
	std::string text;
	unsigned char com = conn->read();
	if(com != protocol::Protocol::ANS_GET_ART){
		return false; //Something went wrong
	}
	com = conn->read();
	if(com != protocol::Protocol::ANS_ACK){
		if(com != protocol::Protocol::ANS_NAK){
			return false; //Something went wrong
		}else{
			com = conn->read();
			if(com != protocol::Protocol::ERR_NG_DOES_NOT_EXIST && com != protocol::Protocol::ERR_ART_DOES_NOT_EXIST){
				return false; //Something went wrong
			}
		}
	}else{
		com = conn->read();
		if(com != protocol::Protocol::PAR_STRING){
			return false; //Something went wrong
		}
		unsigned int n = read_int(conn);
		title = read_string(n,conn);

		com = conn->read();
		if(com != protocol::Protocol::PAR_STRING){
			return false; //Something went wrong
		}
		n = read_int(conn);
		author = read_string(n,conn);

		com = conn->read();
		if(com != protocol::Protocol::PAR_STRING){
			return false; //Something went wrong
		}
		n = read_int(conn);
		text = read_string(n,conn);
	}
	com = conn->read();
	if(com != protocol::Protocol::ANS_END){
		return false; //Something went wrong
	}
	std::cout<<"Title: "<<title<<"\nAuthor: "<<author<<"\nText: "<<text<<std::endl;
	return true;
}

}
