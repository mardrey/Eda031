#include "database.h"
#include <iostream>
#include "in_memory_database.h"
#include "server.h"

//using namespace client_server;
using namespace database;
int main(){	
	in_memory_database db();
	std::cout << "Hejsan svejsans "<< std::endl;
	client_server::Server s(2011);
	/* code */
	return 0;
}
