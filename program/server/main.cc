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

//using namespace client_server;
using namespace database;
int main(){	
	in_memory_database db();
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
	/* code */
	return 0;
}

int readCommand(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
	unsigned char b1 = conn->read();
	std::cout<<"inReadCommand"<<std::endl;
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
	std::cout<<"inListNG"<<std::endl;
	unsigned char par_type = conn->read();
	unsigned int n;
	if(par_type == protocol::Protocol::PAR_STRING){
		char b1 = conn->read();
		char b2 = conn->read();
		char b3 = conn->read();
		char b4 = conn->read();
		n = ((b1<<24) | (b2<<16) | (b3<<8) | (b4));
		std::vector<char> str;
		for(unsigned int i = 0; i < n; ++i){
			str.push_back(conn->read());
		}
		char end = conn->read();
		if(end == protocol::Protocol::COM_END){
			for(std::vector<char>::iterator iter = str.begin(); iter != str.end(); iter++){
				std::cout<<*iter<<std::endl;
			}
			return 0;
		}else{
			return 1;
		}
	}else if(par_type == protocol::Protocol::PAR_NUM){
		return 0;
	}
	return 0;
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

