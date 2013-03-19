#include "database.h"
#include <iostream>

namespace database{
class in_memory_database : database{
	public:
		in_memory_database(){}
		~in_memory_database(){}
		void print_status(int i){
			std::cout<<"status"<<std::endl;

		}
	private:

};
}