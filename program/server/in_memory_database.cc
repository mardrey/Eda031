#include "database.h"
#include "news_group.h"
#include <iostream>
#include <vector>
#include <climits>
namespace database{
class in_memory_database : database{
	public:
		in_memory_database(){
			news_group_ids = 0;	
		}
		~in_memory_database(){}
		void print_status(int i){
			std::cout<<"status "<<i<<std::endl;

		}

		void add_news_group(std::string& name){
			for(unsigned int i = 0; i< ngnames.size(); ++i){
				if(ngnames[i]==name){
					std::cerr<<"name is already in use"<<std::endl;
					throw -2;
				}
			}
			if(news_group_ids<UINT_MAX){			
				news_group group(name,news_group_ids);
				news_group_ids++;
				ngroups.push_back(group);
				ngnames.push_back(name);
			}
			else{
				std::cerr<<"Maximum amount of news groups reached"<<std::endl;
				throw -1;
			}

		}

		void list_news_groups(){
			for(unsigned int i = 0; i< ngroups.size(); ++i){
				std::cout<<"name: "<<ngroups[i].get_name()<<"	id: "<<ngroups[i].get_id()<<std::endl;
			}
		}
	private:
	unsigned int news_group_ids;
	std::vector<std::string> ngnames;
	std::vector<news_group> ngroups;
};
}
