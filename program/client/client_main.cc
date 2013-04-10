#include <stdlib.h>
#include <iostream>
#include "connection.h"
#include "client_connection_handler.h"
#include <string>
#include <sstream>
using namespace client;
using namespace client_server;
int main(int argc, char* argv[]){
	if(argc<3){
		std::cerr<<"Insufficient arguments. command is: "<<argv[0]<<"[host] [port]"<<std::endl;
		exit(1);
	}
	std::cout<<"\t\t\t**************************************\t\t\t\n"<<"\t\t\t* Welcome to News Group Reader 2000  * \t\t\t"<<"\n\t\t\t**************************************\t\t\t"<<std::endl;

	std::cout<<"\n\n\t\t\t\tPlease select a command:\n\n1:\tList News Groups\n2:\tCreate News Group\n3:\tDelete News Group\n4:\tCreate Article\n5:\tDelete Article\n6:\tGet Article\n7:\tList Article"<<std::endl;
	std::string command;	
	
	const char* host = argv[1];
	int port = atoi(argv[2]);
	client_server::Connection cp(host,port);
	std::cout<<"host:"<<host<<std::endl;
	std::cout<<"port:"<<port<<std::endl;
	if (!cp.isConnected()) {
		std::cerr << "Connection attempt failed" << std::endl;
		exit(1);
	}

	client_connection_handler cch(&cp);
	while(true){
		std::cout<<"in da loop"<<std::endl;
		std::cin>>command;
		int nbr = atoi(command.c_str());
		std::cout<<"COMMAND: "<<nbr<<std::endl;
		switch(nbr){
			case 1:{
				cch.send_command_list_ng();}
			break;
			case 2:{
				std::cout<<"Please enter the name of the News Group"<<std::endl;
				std::string name;				
				std::cin>>name;
				bool success = cch.send_command_create_ng(name);
				if(success){
					std::cout<<"Group created successfully"<<std::endl;
				}
				else{
					std::cerr<<"Group creation failed"<<std::endl;
				}}
			break;
			case 3:{
				std::cout<<"Please enter the ID number of the News Group"<<std::endl;
				std::string id;				
				std::cin>>id;
				unsigned int id_nbr = atoi(id.c_str()); //if not a number, gives group with id 0
				bool success = cch.send_command_delete_ng(id_nbr);
				if(success){
					std::cout<<"Group deletion successful"<<std::endl;
				}
				else{
					std::cerr<<"Group deletion failed"<<std::endl;
				}}
			break;
			case 4:{
				std::string foo;
				std::string id;	
				std::string title;
				std::string author;
				std::string  content;
				std::getline(std::cin,foo);
				std::cout<<"Please enter the ID number of the News Group"<<std::endl;
							
				std::getline(std::cin,id);
				unsigned int id_nbr = atoi(id.c_str()); //if not a number, gives group with id 0
				std::cout<<"Please enter the title of the article"<<std::endl;
				
				std::getline(std::cin,title);
			
			
				std::cout<<"title: "<<title<<std::endl;
				std::cout<<"Please enter the author of the article"<<std::endl;
				
				std::getline(std::cin,author);
			
				std::cout<<"author: "<<author<<std::endl;
				std::cout<<"Please enter the content of the article"<<std::endl;	
				
				std::getline(std::cin,content);
				
				std::cout<<"content: "<<content<<std::endl;
			
				bool success = cch.send_command_create_art(id_nbr,title,author,content);
				if(success){
					std::cout<<"Article creation successful"<<std::endl;
				}
				else{
					std::cerr<<"Article creation failed"<<std::endl;
				}	
				}
			break;
			case 5:{
				std::cout<<"Please enter the ID number of the News Group"<<std::endl;
				std::string id;				
				std::cin>>id;
				unsigned int id_nbr = atoi(id.c_str()); //if not a number, gives group with id 0
				std::cout<<"Please enter the ID number of the Article"<<std::endl;
				std::string art;				
				std::cin>>art;
				unsigned int art_nbr = atoi(art.c_str()); //if not a number, gives group with id 0
				bool success = cch.send_command_delete_art(id_nbr,art_nbr);
				if(success){
					std::cout<<"Article deletion successful"<<std::endl;
				}
				else{
					std::cerr<<"Article deletion failed"<<std::endl;
				}	
				}
			break;
			case 6:{
				std::cout<<"Please enter the ID number of the News Group"<<std::endl;
				std::string id;				
				std::cin>>id;
				unsigned int id_nbr = atoi(id.c_str()); //if not a number, gives group with id 0
				std::cout<<"Please enter the ID number of the Article"<<std::endl;
				std::string art;				
				std::cin>>art;
				unsigned int art_nbr = atoi(art.c_str()); //if not a number, gives group with id 0
				bool success = cch.send_command_get_art(id_nbr,art_nbr);
				if(success){
					std::cout<<"Article found"<<std::endl;
				}
				else{
					std::cerr<<"Article not found"<<std::endl;
				}	
				}
			break;
			case 7:{
				std::cout<<"Please enter the ID number of the News Group"<<std::endl;
				std::string id;				
				std::cin>>id;
				unsigned int id_nbr = atoi(id.c_str()); //if not a number, gives group with id 0
				bool success = cch.send_command_list_art(id_nbr);
				if(!success){
					std::cerr<<"articles not found"<<std::endl;
				}
			}break;

			default:
				std::cerr<<"Not a valid command"<<std::endl;
			break;
		}
		command = "";
	}

return 0;
}
