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
		bool in_memory_database::delete_news_group(unsigned int id){
			bool found = false;			
			for(unsigned int i = 0; i<ngroups.size();++i){
				if(ngroups[i].get_id()==id){
					std::vector<std::string>::iterator pl;
					for(std::vector<std::string>::iterator iter = ngnames.begin(); iter != ngnames.end();iter++){
						if(*iter == ngroups[i].get_name()){
							pl = iter;
						}
					}
					ngnames.erase(pl);
					ngroups.erase(ngroups.begin()+i);
					found = true;
					
				}
			}
			return found;
		}


		bool in_memory_database::add_article(unsigned int id, std::string& title, std::string& author, std::string& content){
			for(unsigned int i = 0; i<ngroups.size();++i){
				if(ngroups[i].get_id()==id){
					ngroups[i].new_article(title,author,content);	
					return true;
				}
			}
			return false;
		}
		// 0 = group and article found, 1 = article found, -1 = niether group nor article found
		int in_memory_database::delete_article(unsigned int group_id, unsigned int article_id){ 
			int found = -1;
			for(unsigned int i = 0; i<ngroups.size();++i){
				if(ngroups[i].get_id()==group_id){
					if(ngroups[i].remove_article(article_id)){
						found = 0;
						return found;
					}
					else{
						found = 1;
						return found;
					}					
				}
			}
			return found;
		}
		std::vector<news_group>  in_memory_database::list_news_groups(){
			for(unsigned int i = 0; i< ngroups.size(); ++i){
				std::cout<<"name: "<<ngroups[i].get_name()<<"	id: "<<ngroups[i].get_id()<<std::endl;
			}
			return ngroups;

		}
		news_group* in_memory_database::get_news_group(int id){		
			for(unsigned int i = 0; i<ngroups.size();++i){
				if(ngroups[i].get_id()==id){
					return &ngroups[i];
				}
			}
			return 0;
		}

}
