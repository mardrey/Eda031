#include "in_memory_database.h"
#include "database.h"
#include "news_group.h"
#include <iostream>
#include <vector>
#include <climits>
#include <string>
#include <vector>

namespace database{
		in_memory_database::in_memory_database(){
			news_group_ids = 0;	
		}
		in_memory_database::~in_memory_database(){}
		void print_status(int i){
			std::cout<<"status "<<i<<std::endl;

		}

		int in_memory_database::add_news_group(std::string& name){
			for(unsigned int i = 0; i< ngnames.size(); ++i){
				if(ngnames[i]==name){
					std::cerr<<"name is already in use"<<std::endl;
					return -2;
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
				return -1;
			}
			return 0;

		}

		std::vector<news_group>  in_memory_database::list_news_groups(){
			for(unsigned int i = 0; i< ngroups.size(); ++i){
				std::cout<<"name: "<<ngroups[i].get_name()<<"	id: "<<ngroups[i].get_id()<<std::endl;
			}
			return ngroups;

		}

}
