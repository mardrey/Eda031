#include "on_disc_database.h"
#include <string>
#include "news_group.h"
#include "article.h"
#include "database.h"
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>

namespace database{

	bool make_dir(std::string name){
		std::string path = "./"<<name;
		int succ = mkdir(pat.c_str(), S_IRWXU|S_IRGRP|S_IXGRP);
		if(succ != 0){
			std::cerr<<"Could not create directory "<<name<<std::endl;
			return false; 
		}
		return true;
	}

	on_disc_database::on_disc_database(){
		path = "./database";
		root = opendir(path.c_str());
		if(root == NULL){
			int succ = mkdir(path.c_str(), S_IRWXU|S_IRGRP|S_IXGRP);
			if(succ != 0){
				exit(1);
			}
		}else{
			closedir(root);
		}
	}
	on_disc_database::~on_disc_database(){
		
	}

	void on_disc_database::print_status(int i){

	}

	int on_disc_database::add_news_group(std::string& name){
		root = opendir(path.c_str());
		

		return 0; //Everything went ok
	}

	std::vector<news_group> on_disc_database::list_news_groups(){
		std::vector<news_group> groups;
		return groups;
	}

	bool on_disc_database::delete_news_group(unsigned int id){
		return false;
	}

	bool on_disc_database::add_article(unsigned int id, std::string& title, std::string& author, std::string& content){
		return false;
	}

	int on_disc_database::delete_article(unsigned int group_id, unsigned int article_id){
		return 0;
	}
	article* on_disc_database::get_article(unsigned int group_id, unsigned int article_id){
		return NULL;
	}
	news_group* on_disc_database::get_news_group(unsigned int id){
		return NULL;
	}
}