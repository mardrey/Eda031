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

	bool make_dir(std::string name){
		int succ = mkdir(name.c_str(), S_IRWXU|S_IRGRP|S_IXGRP);
		if(succ != 0){
			std::cerr<<"Could not create directory  "<<name<<std::endl;
			return false; 
		}
		return true;
	}

	unsigned int get_unused_index(std::string index_file_path){
		std::ifstream file_exists;
		file_exists.open(index_file_path.c_str());
		if(!file_exists){
			std::ofstream index_file;
			index_file.open(index_file_path.c_str(),std::fstream::app);
			index_file<<"0\n";
			index_file.close();
			return 0;
		}else{
			unsigned int index;
			file_exists >> index;
			while(!file_exists.eof()){
				file_exists >> index;
			}
			file_exists.close();
			index++;
			std::ofstream index_file_out;
			index_file_out.open(index_file_path.c_str(),std::fstream::app);
			index_file_out<<index;
			index_file_out<<"\n";
			index_file_out.close();
			return index;
		}
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
		std::string indices_text = path+"/used_indices.txt";
		unsigned int id = get_unused_index(indices_text);
		std::ostringstream convert;
		convert<<id;
		std::string id_str;
		id_str = convert.str();
		std::string create_dir_name = (path+"/"+id_str+":"+name);
		if(make_dir(create_dir_name)){
			return 0; //Everything went ok
		}
		std::cerr<<"could not create directory for news group"<<std::endl;
		return -1;
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
		DIR *dir = opendir(path);
		struct dirent *entry = readdir(dir);
		bool found = false;
		while(entry!=null && !found){
			if(entry->d_type == DT_DIR){
				std::string d_name = entry->d_name;
				if()
			}
			entry = readdir(dir);
		}
		//ss1<<path<<"/"<<id;
		std::string ng_path = ss1.str();	
		DIR *ng_dir = opendir(ng_path.c_str());	
		if(ng_dir==NULL){
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
