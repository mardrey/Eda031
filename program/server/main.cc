#include "database.h"
#include <iostream>
#include "in_memory_database.h"
#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"

int readCommand(client_server::Connection* conn) throw(client_server::ConnectionClosedException);

//using namespace client_server;
using namespace database;
int main(){	
	in_memory_database db();
	client_server::Server s(2011);
	if(!s.isReady()){
		std::cerr << "Server could not be initialized" << std::endl;
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
	switch(b1){
		case protocol::Protocol::PAR_STRING : std::cout<<"It is 1!" <<b1<<std::endl;
		break;
		case protocol::Protocol::PAR_NUM: std::cout<<"It is 1!" <<b1<<std::endl;
		break;
		default: std::cout<<b1<<std::endl;
		break;
	}
	return 1;
}

