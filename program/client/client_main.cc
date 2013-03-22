
#include <iostream>
#include "connection.h"
#include "client_connection_handler.h"
#include <string>
int main(int argc, char* argv[]){

	std::cout<<"\t\t\t**************************************\t\t\t\n"<<"\t\t\t* Welcome to News Group Reader 2000  * \t\t\t"<<"\n\t\t\t**************************************\t\t\t"<<std::endl;

	std::cout<<"\n\n\t\t\t\tPlease select a command:\n\n1:\tList News Groups\n2:\tCreate News Group\n3:\tDelete News Group\n4:\tCreate Article\n5:\tDelete Article\n6:\tGet Article"<<std::endl;
	std::string command;	
	while(true){
		//std::cin>>command>>std::endl;
		
	}

	client_server::Connection conn("localhost", 2011);
	if (! conn.isConnected()) {
		std::cerr << "Connection attempt failed" << std::endl;
		return 1;
	}
	client::client_connection_handler cch(&conn);
//	cch.send_command_list_ng();
return 0;
}
