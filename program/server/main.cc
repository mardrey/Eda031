#include "database.h"
#include <iostream>
#include "in_memory_database.h"
#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"
#include <vector>
#include <cstdlib>

int com_list_ng(client_server::Connection* conn) throw(client_server::ConnectionClosedException);
int com_create_ng(client_server::Connection* conn) throw(client_server::ConnectionClosedException);
int com_delete_ng(client_server::Connection* conn) throw(client_server::ConnectionClosedException);
int com_list_art(client_server::Connection* conn) throw(client_server::ConnectionClosedException);
int com_create_art(client_server::Connection* conn) throw(client_server::ConnectionClosedException);
int com_delete_art(client_server::Connection* conn) throw(client_server::ConnectionClosedException);
int com_get_art(client_server::Connection* conn) throw(client_server::ConnectionClosedException);
int readCommand(client_server::Connection* conn) throw(client_server::ConnectionClosedException);

//database::in_memory_database imd;
//using namespace client_server;
using namespace database;

in_memory_database* imd;

int main(){	
	imd = new in_memory_database();
	client_server::Server s(2011);
	if(!s.isReady()){
		std::cerr << "Server could not be initialized" << std::endl;
		exit(1);
	}else{
		std::cout <<"Server initialized"<< std::endl;
	}
	while(true){
		client_server::Connection* conn = s.waitForActivity();
		if(conn != 0){
			try{
				//do connection stuff
				int nbr = readCommand(conn);
			}catch(client_server::ConnectionClosedException&){
				s.deregisterConnection(conn);
				delete conn;
				std::cout << "Client closed connection" << std::endl;
			}
		}else{
			s.registerConnection(new client_server::Connection);
			std::cout <<"New client connects"<< std::endl;
		}
	}
	delete(imd);
	/* code */
	return 0;
}

int readCommand(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
	unsigned char b1 = conn->read();
	std::cout<<" -inReadCommand"<<std::endl;
	switch(b1){
		case protocol::Protocol::COM_LIST_NG : return com_list_ng(conn);
		break;
		case protocol::Protocol::COM_CREATE_NG: return com_create_ng(conn);
		break;
		case protocol::Protocol::COM_DELETE_NG: return com_delete_ng(conn);
		break;
		case protocol::Protocol::COM_LIST_ART: return com_list_art(conn);
		break;
		case protocol::Protocol::COM_DELETE_ART: return com_delete_art(conn);
		break;
		case protocol::Protocol::COM_CREATE_ART: return com_create_art(conn);
		break;
		case protocol::Protocol::COM_GET_ART: return com_get_art(conn);
		break;
		default: std::cout<<b1<<std::endl;
		break;
	}
	return 1;
}

int com_list_ng(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
	std::cout<<"  -inListNG"<<std::endl;
	unsigned char is_end = conn->read();
	if(is_end == protocol::Protocol::COM_END){
		conn->write(protocol::Protocol::ANS_LIST_NG);
		std::vector<news_group> vec = imd->list_news_groups();
		unsigned int size = vec.size();
		conn->write(protocol::Protocol::PAR_NUM);
		char* bArray = static_cast<char*>(static_cast<void*>(&size));
		conn->write(bArray[0]);
		conn->write(bArray[1]);
		conn->write(bArray[2]);
		conn->write(bArray[3]);
		for(std::vector<news_group>::iterator iter = vec.begin(); iter != vec.end(); iter++){
			unsigned int id = iter->get_id();
			std::string desc = iter->get_name();
		}
		
		conn->write(protocol::Protocol::PAR_NUM);
		conn->write(char(0));
		conn->write(char(0));
		conn->write(char(0));
		conn->write(char(1));
		conn->write(protocol::Protocol::PAR_NUM);
		conn->write(char(0));
		conn->write(char(0));
		conn->write(char(0));
		conn->write(char(1));
		conn->write(protocol::Protocol::PAR_STRING);
		conn->write(char(0));
		conn->write(char(0));
		conn->write(char(0));
		conn->write(char(1));
		conn->write('A');
		conn->write(protocol::Protocol::ANS_END);
		return 0;
	}else{
		return 1;
	}
}

int com_create_ng(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
	return 0;
}

int com_delete_ng(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
	return 0;
}

int com_list_art(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
	return 0;
}

int com_delete_art(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
	return 0;
}

int com_create_art(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
	return 0;
}

int com_get_art(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
	return 0;
}

