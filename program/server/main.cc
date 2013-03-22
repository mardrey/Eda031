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


int main(int argc, char* argv[]){	
	if(argc != 2){
		std::cerr<<"Run format is: "<<argv[0]<<" [port]"<<std::endl;
		return 1;
	}
	imd = new in_memory_database();
	client_server::Server s(atoi(argv[1]));
	if(!s.isReady()){
		std::cerr << "Server could not be initialized" << std::endl;
		exit(1);
	}else{
		std::cout <<"Server initialized"<< std::endl;
	}
	while(true){
		std::cout<<"looping"<<std::endl;
		client_server::Connection* conn = s.waitForActivity();
		if(conn != 0){
			std::cout<<"conn is not zero"<<std::endl;
			try{
				//do connection stuff
				int nbr = readCommand(conn);
				if(nbr == 1){
					std::cerr<<"Something went wrong"<<std::endl;
				}
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

void write_int(int N, client_server::Connection* conn){
	unsigned char bytes[4];
	int_to_byte_array(N, bytes);
	conn->write(bytes[0]);
	conn->write(bytes[1]);
	conn->write(bytes[2]);
	conn->write(bytes[3]);
}

int com_list_ng(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
	std::cout<<"  -inListNG"<<std::endl;
	unsigned char is_end = conn->read();
	if(is_end == protocol::Protocol::COM_END){
		conn->write(protocol::Protocol::ANS_LIST_NG);
		std::vector<news_group> vec = imd->list_news_groups();
		unsigned int size = vec.size();
		conn->write(protocol::Protocol::PAR_NUM);
		write_int(size,conn);

		for(std::vector<news_group>::iterator iter = vec.begin(); iter != vec.end(); iter++){
			unsigned int id = iter->get_id();
			std::string desc = iter->get_name();
			unsigned char sBArray[4];
			int_to_byte_array(id, sBArray);
			unsigned int i = 0;
			conn->write(protocol::Protocol::PAR_NUM);
			while(i < 4){
				conn->write(sBArray[i]);
				i++;
			}
			i = 0;
			conn->write(protocol::Protocol::PAR_STRING);
			
			write_int(desc.size(),conn);

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
		
		unsigned int N = read_int(conn);
		std::vector<unsigned char> chars;
		for(unsigned int i = 0; i < N; ++i){
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

		unsigned int N = read_int(conn);
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

	std::cout<<"  -Done"<<std::endl;

	return 0;
}

int com_list_art(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
	std::cout<<"  -inListArt"<<std::endl;
	unsigned char par_num = conn->read();
	if(par_num == protocol::Protocol::PAR_NUM){

		unsigned int N = read_int(conn);

		unsigned char end = conn->read();
		if(end == protocol::Protocol::COM_END){
			news_group* ng = imd->get_news_group(N);
			conn->write(protocol::Protocol::ANS_LIST_ART);
			if(ng == 0){
				conn->write(protocol::Protocol::ANS_NAK);
				conn->write(protocol::Protocol::ERR_ART_DOES_NOT_EXIST);
			}else{
				std::vector<article>* arts = ng->list_articles();
				conn->write(protocol::Protocol::ANS_ACK);
				conn->write(protocol::Protocol::PAR_NUM);

				write_int(arts->size(), conn);

				for(unsigned int i = 0; i < arts->size(); ++i){
					conn->write(protocol::Protocol::PAR_NUM);

					write_int((*arts)[i].get_id(), conn);



					unsigned int n = (*arts)[i].get_title().size();
					conn->write(protocol::Protocol::PAR_STRING);

					write_int((*arts)[i].get_title().size(), conn);

					for(unsigned int j = 0; j < n; ++j){
						conn->write((*arts)[i].get_title()[j]);
					}
				}
			}
			conn->write(protocol::Protocol::ANS_END);
		}
	}
	std::cout<<"  -Done"<<std::endl;
	return 0;
}

int com_delete_art(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
		std::cout<<"  -inDeleteArt"<<std::endl;
		unsigned char is_parnum1 = conn->read();
		if(is_parnum1!=protocol::Protocol::PAR_NUM){
			std::cerr<<"Not a delete command"<<std::endl;
			return 1;
		}
		
		unsigned int group = read_int(conn);
		unsigned char is_parnum2 = conn->read();
		if(is_parnum2!=protocol::Protocol::PAR_NUM){
			std::cerr<<"Not a delete command"<<std::endl;
			return 1;
		}
		
		unsigned int article = read_int(conn);
		unsigned char is_comend = conn->read();
		if(is_comend!=protocol::Protocol::COM_END){
			std::cerr<<"Not a delete command"<<std::endl;
			return 1;
		}
		conn->write(protocol::Protocol::ANS_DELETE_ART);
		int deleted = imd->delete_article(group,article);
		if(deleted==0){
			conn->write(protocol::Protocol::ANS_ACK);
		}
		else if(deleted == 1){
			conn->write(protocol::Protocol::ANS_NAK);
			conn->write(protocol::Protocol::ERR_ART_DOES_NOT_EXIST);
		}
		else{
			conn->write(protocol::Protocol::ANS_NAK);
			conn->write(protocol::Protocol::ERR_NG_DOES_NOT_EXIST);
		}
		std::cout<<"  -Done"<<std::endl;
		conn->write(protocol::Protocol::ANS_END);
		std::cout<<"  -Done"<<std::endl;
		return 0;
}

int com_create_art(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
	
	std::cout<<"  -inCreateArt"<<std::endl;
	
	unsigned char is_parnum1 = conn->read();
	if(is_parnum1!=protocol::Protocol::PAR_NUM){
			std::cerr<<"Not a create command"<<std::endl;
			return 1;
		}
	
	unsigned int group = read_int(conn);
	unsigned char is_parstring = conn->read();
	if(is_parstring!=protocol::Protocol::PAR_STRING){
			std::cerr<<"Not a create command"<<std::endl;
			return 1;
		}
	std::vector<char> title_vector;
	
	unsigned int title_n = read_int(conn);
	for(unsigned int i = 0; i<title_n;++i){
		is_parstring=conn->read();
		title_vector.push_back(is_parstring);
	}
	std::string title(title_vector.begin(),title_vector.end());

	unsigned char is_parstring2 = conn->read();
	if(is_parstring2!=protocol::Protocol::PAR_STRING){
			std::cerr<<"Not a create command"<<std::endl;
			return 1;
	}

	std::vector<char> author_vector;
	
	unsigned int author_n = read_int(conn);
	for(unsigned int i = 0; i<author_n;++i){
		is_parstring=conn->read();
		author_vector.push_back(is_parstring);
	}
	std::string author(author_vector.begin(),author_vector.end());
	unsigned char is_parstring3 = conn->read();
	if(is_parstring3!=protocol::Protocol::PAR_STRING){
			std::cerr<<"Not a create command"<<std::endl;
			return 1;
		}
	std::vector<char> content_vector;
	
	unsigned int content_n = read_int(conn);
	for(unsigned int i = 0; i<content_n;++i){
		is_parstring=conn->read();
		content_vector.push_back(is_parstring);
	}
	std::string content(content_vector.begin(),content_vector.end());
	unsigned char is_comend = conn->read();
	if(is_comend!=protocol::Protocol::COM_END){
		std::cerr<<"Not a create command"<<std::endl;
		return 1;
	}	

	bool found = imd->add_article(group,title,author,content);
	conn->write(protocol::Protocol::ANS_CREATE_ART);
	if(found){
		conn->write(protocol::Protocol::ANS_ACK);
	}
	else{
		conn->write(protocol::Protocol::ANS_NAK);
		conn->write(protocol::Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	conn->write(protocol::Protocol::ANS_END);
	std::cout<<"  -Done"<<std::endl;
	return 0;
}

int com_get_art(client_server::Connection* conn) throw(client_server::ConnectionClosedException) {
		std::cout<<"  -inGetArt"<<std::endl;
		unsigned char is_parnum1 = conn->read();
		if(is_parnum1!=protocol::Protocol::PAR_NUM){
			std::cerr<<"Not a get command"<<std::endl;
			return 1;
		}
		unsigned int group = read_int(conn);
		unsigned char is_parnum2 = conn->read();
		if(is_parnum2!=protocol::Protocol::PAR_NUM){
			std::cerr<<"Not a get command"<<std::endl;
			return 1;
		}
		unsigned int article = read_int(conn);
		unsigned char is_comend = conn->read();
		if(is_comend!=protocol::Protocol::COM_END){
			std::cerr<<"Not a get command"<<std::endl;
			return 1;
		}
		conn->write(protocol::Protocol::ANS_GET_ART);
		news_group* ng_pointer = imd->get_news_group(group);
		if(ng_pointer==0){
			conn->write(protocol::Protocol::ANS_NAK);
			conn->write(protocol::Protocol::ERR_NG_DOES_NOT_EXIST);
		}
		else{
			database::article* art_pointer= imd->get_article(group, article);
			if(art_pointer == 0){ 
				conn->write(protocol::Protocol::ANS_NAK);
				conn->write(protocol::Protocol::ERR_ART_DOES_NOT_EXIST);
			}
			else{	
					conn->write(protocol::Protocol::ANS_ACK);
					conn->write(protocol::Protocol::PAR_STRING);
					std::string title = art_pointer->get_title();
					write_int(title.size(), conn);
					for(unsigned int i = 0; i< title.size(); ++i){
						conn->write(title[i]);
					}
					conn->write(protocol::Protocol::PAR_STRING);
					std::string author = art_pointer->get_author();
					write_int(author.size(),conn);
					for(unsigned int i = 0; i< author.size(); ++i){
						conn->write(author[i]);
					}
					conn->write(protocol::Protocol::PAR_STRING);
					std::string content = art_pointer->get_content();	
					write_int(content.size(),conn);
					for(unsigned int i = 0; i< content.size(); ++i){
						conn->write(content[i]);
					}
			}
				
		}

		conn->write(protocol::Protocol::ANS_END);
		std::cout<<"  -Done"<<std::endl;
	return 0;
}

