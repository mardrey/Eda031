#include "on_disc_database.h"
#include <string>
#include "news_group.h"
#include "article.h"
#include "database.h"
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <sstream>

namespace database{
	unsigned int get_unused_index(std::string path){

	return 0;
	}
	bool make_dir(std::string name){
		std::string beg_path = "./";
		std::string local_path = beg_path.append(name);
		int succ = mkdir(local_path.c_str(), S_IRWXU|S_IRGRP|S_IXGRP);
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
		std::stringstream ss1;
		ss1<<path<<"/"<<id;
		std::string ng_path = ss1.str();	
		DIR *dir = opendir(ng_path.c_str());	
		if(dir==NULL){
			std::cerr<<"News group does not exist"<<std::endl;
			return false;
		}
		unsigned int highest = get_unused_index(ng_path);
		std::stringstream ss2;
		ss2<<ng_path<<"/"<<highest;
		std::string art_path = ss2.str();
		std::ofstream ofs;
		ofs.open(art_path.c_str());
		ofs<<title<<"\n\n"<<author<<"\n\n"<<content;
 		ofs.close();
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
