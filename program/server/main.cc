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
			char* sBArray = static_cast<char*>(static_cast<void*>(&id));
			char* sSArray = static_cast<char*>(static_cast<void*>(&desc));
			unsigned int i = 0;
			conn->write(protocol::Protocol::PAR_NUM);
			while(sBArray[i] != '\0'){
				conn->write(sBArray[i]);
				i++;
			}
			i = 0;
			conn->write(protocol::Protocol::PAR_STRING);
			while(sSArray[i] != '\0'){
				conn->write(sSArray[i]);
				i++;
			}
		}
		conn->write(protocol::Protocol::ANS_END);
		return 0; //All fine
	}else{
		return 1; //Exception
	}
}

int com_create_ng(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
	std::cout<<"  -inCreateNG"<<std::endl;
	unsigned char par_str = conn->read();
	if(par_str == protocol::Protocol::PAR_STRING){
		unsigned char n = conn->read();
		std::vector<unsigned char> chars;
		for(int i = 0; i < n; ++i){
			chars.push_back(conn->read());
		}
		unsigned char end = conn->read();
		if(end == protocol::Protocol::COM_END){
			
				std::string s(chars.begin(),chars.end());
					int fine = imd->add_news_group(s);
					if(fine == 0){
					conn->write(protocol::Protocol::ANS_ACK);
					}else{
						conn->write(protocol::Protocol::ANS_NAK);
					conn->write(protocol::Protocol::ERR_NG_ALREADY_EXISTS);
					}
			conn->write(protocol::Protocol::ANS_END);
		}else{
			return 1; //Exception
		}
	}else{
		return 1; //Exception
	}

	return 0;
}

int com_delete_ng(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
	return 0;
}

int com_list_art(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
	return 0;
}

int com_delete_art(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
		std::cout<<"  -inDeleteArt"<<std::endl;
		unsigned char is_parnum1 = conn->read();
		if(is_parnum1!=protocol::Protocol::PAR_NUM){
			std::cerr<<"Not a delete command"<<std::endl;
			return 1;
		}
		unsigned char group1 = conn->read();
		unsigned char group2 = conn->read();
		unsigned char group3 = conn->read();
		unsigned char group4 = conn->read();
		unsigned int group = (group1<<24) | (group2<<16) | (group3<<8) | (group4); 
		unsigned char is_parnum2 = conn->read();
		if(is_parnum2!=protocol::Protocol::PAR_NUM){
			std::cerr<<"Not a delete command"<<std::endl;
			return 1;
		}
		unsigned char article1 = conn->read();
		unsigned char article2 = conn->read();
		unsigned char article3 = conn->read();
		unsigned char article4 = conn->read();
		unsigned int article = (article1<<24) | (article2<<16) | (article3<<8) | (article4); 
		unsigned char is_comend = conn->read();
		if(is_comend!=protocol::Protocol::COM_END){
			std::cerr<<"Not a delete command"<<std::endl;
			return 1;
		}
		int deleted = imd->delete_article(group,article);
		if(deleted==0){
			conn->write(protocol::Protocol::ANS_ACK);
		}
		else if(deleted == 1){
			conn->write(protocol::Protocol::ERR_ART_DOES_NOT_EXIST);
		}
		else{
			conn->write(protocol::Protocol::ERR_NG_DOES_NOT_EXIST);
		}
		return 0;
}

int com_create_art(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
	return 0;
}

int com_get_art(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
	return 0;
}

