#include <stdlib.h>
#include <iostream>
#include "connection.h"
#include "client_connection_handler.h"
#include <string>

using namespace client;
using namespace client_server;
int main(int argc, char* argv[]){
	if(argc<3){
		std::cerr<<"Insufficient arguments. command is: "<<argv[0]<<"[host] [port]"<<std::endl;
		exit(1);
	}
	std::cout<<"\t\t\t**************************************\t\t\t\n"<<"\t\t\t* Welcome to News Group Reader 2000  * \t\t\t"<<"\n\t\t\t**************************************\t\t\t"<<std::endl;

	std::cout<<"\n\n\t\t\t\tPlease select a command:\n\n1:\tList News Groups\n2:\tCreate News Group\n3:\tDelete News Group\n4:\tCreate Article\n5:\tDelete Article\n6:\tGet Article"<<std::endl;
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
		switch(nbr){
			case 1:
				cch.send_command_list_ng();
			break;
			case 2:{
				std::cout<<"Please write the name of the News Group: "<<std::endl;
				std::string s; 
				std::cin>>s;
				cch.send_command_create_ng(s);
			}
			break;
			case 3:
			
			break;
			case 4:
			
			break;
			case 5:
			
			break;
			case 6:
			
			break;
			default:
				std::cerr<<"Not a valid command"<<std::endl;
			break;
		}
	}

return 0;
}
