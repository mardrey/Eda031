#include "database.h"
#include "news_group.h"
#include <string>
#include <iostream>
#include <climits>
#include <vector>
namespace database{
		database::database(){
			news_group_ids = 0;	
		}
		database::~database(){}
		void database::add_news_group(std::string& name){
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
}
