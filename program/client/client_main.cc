#include <stdlib.h>
#include <iostream>
#include <string>
#include "client_connection_handler.h"
#include "connection.h"
using namespace client;
using namespace client_server;
int main(int argc, char* argv[]){
	if(argc<3){
		std::cerr<<"Insufficient arguments. Input is host port"<<std::endl;
		return 1;
	}
	std::cout<<"\t\t\t**************************************\t\t\t\n"<<"\t\t\t* Welcome to News Group Reader 2000  * \t\t\t"<<"\n\t\t\t**************************************\t\t\t"<<std::endl;

	std::cout<<"\n\n\t\t\t\tPlease select a command:\n\n1:\tList News Groups\n2:\tCreate News Group\n3:\tDelete News Group\n4:\tCreate Article\n5:\tDelete Article\n6:\tGet Article"<<std::endl;
	std::string command;	
	client_server::Connection* cp;
	const char* host = argv[1];
	int port = atoi(argv[2]);
	*cp = client_server::Connection(host,port);
	client_connection_handler cch(cp);
	while(true){
		std::cin>>command;
		int nbr = atoi(command.c_str());
		switch(nbr){
			case 1:
				cch.send_command_list_ng();
			break;
			case 2:
			
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
