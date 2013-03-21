#include "database.h"
#include <iostream>
#include "in_memory_database.h"
#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"
#include <vector>
#include <cstdlib>
#include "article.h"


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
	client_server::Server s(2014);
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
	std::cout<<" -Done"<<std::endl;
	return 1;
}

void int_to_byte_array(int n, unsigned char* c){
	c[0] = (n >> 24) & 0xFF;
	c[1] = (n >> 16) & 0xFF;
	c[2] = (n >> 8) & 0xFF;
	c[3] = n & 0xFF;
}

void string_to_byte_array(std::string n, unsigned char* c){
//	c = n.str();
}

int com_list_ng(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
	std::cout<<"  -inListNG"<<std::endl;
	unsigned char is_end = conn->read();
	if(is_end == protocol::Protocol::COM_END){
		conn->write(protocol::Protocol::ANS_LIST_NG);
		std::vector<news_group> vec = imd->list_news_groups();
		unsigned int size = vec.size();
		conn->write(protocol::Protocol::PAR_NUM);
		unsigned char bytes[4];
		int_to_byte_array(size, bytes);
		conn->write(bytes[0]);
		conn->write(bytes[1]);
		conn->write(bytes[2]);
		conn->write(bytes[3]);
		for(std::vector<news_group>::iterator iter = vec.begin(); iter != vec.end(); iter++){
			std::cout<<iter->get_name()<<std::endl;
			unsigned int id = iter->get_id();
			std::string desc = iter->get_name();
			unsigned char sBArray[4];
			int_to_byte_array(id, sBArray);
//			unsigned char sSArray[4];
//			string_to_byte_array(desc, sSArray);
			unsigned int i = 0;
			conn->write(protocol::Protocol::PAR_NUM);
			while(i < 4){
				conn->write(sBArray[i]);
				std::cout<<sBArray[i]<<std::endl;
				i++;
			}
			i = 0;
			conn->write(protocol::Protocol::PAR_STRING);
			unsigned char bytes2[4];
			int_to_byte_array(desc.size(),bytes2);
			conn->write(bytes2[0]);
			conn->write(bytes2[1]);
			conn->write(bytes2[2]);
			conn->write(bytes2[3]);
			for(unsigned int t = 0; t < desc.size(); ++t){
				conn->write(desc[t]);
			}
		}
		conn->write(protocol::Protocol::ANS_END);
		std::cout<<"  -Done"<<std::endl;
		return 0; //All fine
	}else{
		return 1; //Exception
	}
}

int com_create_ng(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
	std::cout<<"  -inCreateNG"<<std::endl;
	unsigned char par_str = conn->read();
	if(par_str == protocol::Protocol::PAR_STRING){
		unsigned char n1 = conn->read();
		unsigned char n2 = conn->read();
		unsigned char n3 = conn->read();
		unsigned char n4 = conn->read();
		unsigned int N = (n1<<24 | n2<<16 | n3<<8 | n4);
		std::vector<unsigned char> chars;
		for(int i = 0; i < N; ++i){
			chars.push_back(conn->read());
		}
		unsigned char end = conn->read();
		if(end == protocol::Protocol::COM_END){
			
				std::string s(chars.begin(),chars.end());
					int fine = imd->add_news_group(s);
					conn->write(protocol::Protocol::ANS_CREATE_NG);
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
	std::cout<<"  -Done"<<std::endl;
	return 0;
}

int com_delete_ng(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
	std::cout<<"  -inDeleteNG"<<std::endl;
	unsigned char par_num = conn->read();
	if(par_num == protocol::Protocol::PAR_NUM){
		unsigned char n1 = conn->read();
		unsigned char n2 = conn->read();
		unsigned char n3 = conn->read();
		unsigned char n4 = conn->read();
		unsigned int N = (n1<<24 | n2<<16 | n3<<8 | n4);
		unsigned char end = conn->read();
		if(end == protocol::Protocol::COM_END){
			bool succ = imd->delete_news_group(N);
			conn->write(protocol::Protocol::ANS_DELETE_NG);
			if(succ){
				conn->write(protocol::Protocol::ANS_ACK);
			}else{
				conn->write(protocol::Protocol::ANS_NAK);
				conn->write(protocol::Protocol::ERR_NG_DOES_NOT_EXIST);
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
		std::cout<<"  -Done"<<std::endl;
		conn->write(protocol::Protocol::ANS_END);
		return 0;
}

int com_create_art(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
	
	std::cout<<"  -inCreateArt"<<std::endl;
	
	unsigned char is_parnum1 = conn->read();
	if(is_parnum1!=protocol::Protocol::PAR_NUM){
			std::cerr<<"Not a create command"<<std::endl;
			return 1;
		}
	unsigned char group1 = conn->read();
	unsigned char group2 = conn->read();
	unsigned char group3 = conn->read();
	unsigned char group4 = conn->read();
	unsigned int group = (group1<<24) | (group2<<16) | (group3<<8) | (group4); 
	unsigned char is_parstring = conn->read();
	if(is_parstring!=protocol::Protocol::PAR_STRING){
			std::cerr<<"Not a create command"<<std::endl;
			return 1;
		}
	std::vector<char> title_vector;
	unsigned char title1 = conn->read();
	unsigned char title2 = conn->read();
	unsigned char title3 = conn->read();
	unsigned char title4 = conn->read();
	unsigned int title_n = (title1<<24) | (title2<<16) | (title3<<8) | (title4); 
	for(unsigned int i = 0; i<title_n;++i){
		is_parstring=conn->read();
		title_vector.push_back(is_parstring);
	}
	std::string title(title_vector.begin(),title_vector.end());

	std::vector<char> author_vector;
	unsigned char author1 = conn->read();
	unsigned char author2 = conn->read();
	unsigned char author3 = conn->read();
	unsigned char author4 = conn->read();
	unsigned int author_n = (author1<<24) | (author2<<16) | (author3<<8) | (author4); 
	for(unsigned int i = 0; i<author_n;++i){
		is_parstring=conn->read();
		author_vector.push_back(is_parstring);
	}
	std::string author(author_vector.begin(),author_vector.end());
	std::vector<char> content_vector;
	unsigned char content1 = conn->read();
	unsigned char content2 = conn->read();
	unsigned char content3 = conn->read();
	unsigned char content4 = conn->read();
	unsigned int content_n = (content1<<24) | (content2<<16) | (content3<<8) | (content4); 
	for(unsigned int i = 0; i<content_n;++i){
		is_parstring=conn->read();
		content_vector.push_back(is_parstring);
	}
	std::string content(content_vector.begin(),content_vector.end());
	unsigned char is_comend = conn.read();
	if(is_comend!=protocol::Protocol::COM_END){
		std::cerr<<"Not a create command"<<std::endl;
		return 1;
	}	

	bool found = imd->add_article(group,title,author,content);
	if(found){
		conn->write(protocol::Protocol::ANS_ACK);
	}
	else{
		conn->write(protocol::Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	conn->write(protocol::Protocol::ANS_END);
	return 0;
}

int com_get_art(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
		std::cout<<"  -inGetArt"<<std::endl;
		unsigned char is_parnum1 = conn->read();
		if(is_parnum1!=protocol::Protocol::PAR_NUM){
			std::cerr<<"Not a get command"<<std::endl;
			return 1;
		}
		unsigned char group1 = conn->read();
		unsigned char group2 = conn->read();
		unsigned char group3 = conn->read();
		unsigned char group4 = conn->read();
		unsigned int group = (group1<<24) | (group2<<16) | (group3<<8) | (group4); 
		unsigned char is_parnum2 = conn->read();
		if(is_parnum2!=protocol::Protocol::PAR_NUM){
			std::cerr<<"Not a get command"<<std::endl;
			return 1;
		}
		unsigned char article1 = conn->read();
		unsigned char article2 = conn->read();
		unsigned char article3 = conn->read();
		unsigned char article4 = conn->read();
		unsigned int article = (article1<<24) | (article2<<16) | (article3<<8) | (article4); 
		unsigned char is_comend = conn->read();
		if(is_comend!=protocol::Protocol::COM_END){
			std::cerr<<"Not a get command"<<std::endl;
			return 1;
		}
		unsigned char is_comend = conn.read();
		if(is_comend!=protocol::Protocol::COM_END){
			std::cerr<<"Not a create command"<<std::endl;
			return 1;
		}	
		article* art_pointer = imd->get_article(group, article);
		




	return 0;
}

