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
#include "article.h"
#include <stdio.h>

	namespace database{

	bool delete_file_dir_entry(std::string& path){
		std::fstream is_file;
		is_file.open(path.c_str());
		if(is_file){
			if(remove(path.c_str()) != 0){
				std::cerr<<"Deletion of file "<<path<<" was unsuccessful"<<std::endl;
				is_file.close();
				return false;
			}else{
				is_file.close();
				return true;
			}
		}
		std::cout<<path<<" was not a file"<<std::endl;
		if(rmdir(path.c_str()) != -1){
			return true;
		}else{
			DIR *root = opendir(path.c_str());
			if(root == NULL){
				std::cerr<<"could not open directory"<<std::endl;
				return false;
			}
			struct dirent *entry = readdir(root);
			while(entry != NULL){
				std::string nome = entry->d_name;
				if(nome != "." && nome != ".." && nome != "" && nome[0] != '.'){
					std::string name = path + "/" + (nome);
					if(!delete_file_dir_entry(name)){
					//	std::cerr<<"Deletion of "<<(name)<<" was unsuccessful"<<std::endl;
						return false;
					}
				}
				entry = readdir(root);
			}
			return (rmdir(path.c_str()) != -1);
		}
	}

	article* read_article(std::string& path, unsigned int id){
		std::ifstream ifs;
		ifs.open(path.c_str());
		if(!ifs){
			std::cerr<<"could not open file"<<std::endl;
			return NULL;
		}
		std::string blank;
		std::string title;
		ifs>>title;
		std::string author;
		ifs>>author;
		std::string content;
		ifs>>content;
		while(!ifs.eof()){
			std::string line;
			ifs>>line;
			content.append(line);
		}
		article* art = new article(id,title,author,content);
		return art;
	}

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
		root = opendir(path.c_str());
		struct dirent *entry = readdir(root);
		std::vector<news_group> groups;
		while(entry != NULL){
			if(entry->d_type == DT_DIR){
				std::string line = entry->d_name;
				std::stringstream split_stream(line);
				std::string temp_string;
				std::string temp_string2;
				getline(split_stream,temp_string,':');
				getline(split_stream,temp_string2);
				if(temp_string != "" && temp_string2 != ""){
				news_group ng(temp_string2,atoi(temp_string.c_str()));
				groups.push_back(ng);
				}

			}
			entry = readdir(root);
		}
		closedir(root);
		return groups;
	}

	bool on_disc_database::delete_news_group(unsigned int id){
		root = opendir(path.c_str());
		struct dirent *entry = readdir(root);
		while(entry != NULL){
			if(entry->d_type == DT_DIR){
				std::string line = entry->d_name;
				std::stringstream split_stream(line);
				std::string temp_string;
				std::string temp_string2;
				getline(split_stream,temp_string,':');
				getline(split_stream,temp_string2);
				if(temp_string != "" && temp_string2 != ""){
					std::string name = (path + "/" + (entry->d_name));
					if(atoi(temp_string.c_str())==id && temp_string != "." && temp_string != ".." && temp_string[0] != '.'){
						std::cout<<"entering delete"<<std::endl;
						bool res = delete_file_dir_entry(name);
						return res;
					}
				}
			}
			entry = readdir(root);
		}
		return false;
	}

	bool on_disc_database::add_article(unsigned int id, std::string& title, std::string& author, std::string& content){
		std::stringstream ss1;
		std::string d_name;
		DIR *dir = opendir(path.c_str());
		struct dirent *entry = readdir(dir);
		bool found = false;
		while(entry!=NULL && !found){
			if(entry->d_type == DT_DIR){
				d_name = entry->d_name;
				std::stringstream split_stream(d_name);
				std::string temp_string;
				getline(split_stream,temp_string,':');
				if(temp_string != "" && temp_string != "." && temp_string != ".." && temp_string[0] != '.'){
					int ng_id = atoi(temp_string.c_str());
					if(ng_id==id){
						found = true;
					}
				}
			}
			entry = readdir(dir);
		}
		if(!found){
			std::cerr<<"News group does not exist"<<std::endl;
			return false;
		}
		ss1<<path<<"/"<<d_name;
		std::string ng_path = ss1.str();	
		DIR *ng_dir = opendir(ng_path.c_str());	
		if(ng_dir==NULL){
			std::cerr<<"News group does not exist"<<std::endl;
			return false;
		}
		unsigned int highest = get_unused_index(ng_path+"/used_indices.txt");
		std::stringstream ss2;
		ss2<<ng_path<<"/"<<highest;
		std::string art_path = ss2.str();
		std::ofstream ofs;
		ofs.open((art_path+":"+title+".art").c_str());
		ofs<<title<<"\n\n"<<author<<"\n\n"<<content;
 		ofs.close();
	}

	int on_disc_database::delete_article(unsigned int group_id, unsigned int article_id){
		std::stringstream ss1;
		std::string d_name_ng;
		std::string d_name_art;
		DIR *dir = opendir(path.c_str());
		struct dirent *entry = readdir(dir);
		bool ng_found = false;
		while(entry!=NULL && !ng_found){
			if(entry->d_type == DT_DIR){
				d_name_ng = entry->d_name;
				std::stringstream split_stream(d_name_ng);
				std::string temp_string;
				getline(split_stream,temp_string,':');
				int ng_id = atoi(temp_string.c_str());
				if(ng_id==group_id){
					ng_found = true;
				}
			}
			entry = readdir(dir);
		}
		if(!ng_found){
			std::cerr<<"News group does not exist"<<std::endl;
			return -1;
		}
		ss1<<path<<"/"<<d_name_ng;
		std::string ng_path = ss1.str();
		DIR *ng_dir = opendir(ng_path.c_str());	
		if(ng_dir==NULL){
			std::cerr<<"News group does not exist"<<std::endl;
			return -1;
		}
        struct dirent *entry2;
        bool art_found = false;
		while(!art_found){
			entry2 = readdir(ng_dir);
			if(entry2==NULL){
				return 1;
			}
			if(entry2->d_name!="."  && (entry2->d_name!="..") ){
				d_name_art = entry2->d_name;
				std::stringstream split_stream(d_name_art);
				std::string temp_string;
				getline(split_stream,temp_string,':');
				int art_id = atoi(temp_string.c_str());
				if(art_id==article_id){
					art_found = true;
				}
			}
		}
        if(!art_found){
			std::cerr<<"article was not found"<<std::endl;
			return 1;
		}
        if(remove((ng_path+"/"+d_name_art).c_str())!=0){
			std::cerr<<"article deletion unsuccessful"<<std::endl;
			return 1;
		}
		
		return 0;
	}
	article* on_disc_database::get_article(unsigned int group_id, unsigned int article_id){


		std::cout<<"in on disc get art"<<std::endl;
		std::stringstream ss1;
        std::string d_name_ng;
        std::string d_name_art;
        DIR *dir = opendir(path.c_str());
        struct dirent *entry = readdir(dir);
        bool ng_found = false;
        while(entry!=NULL && !ng_found){
            if(entry->d_type == DT_DIR){
                d_name_ng = entry->d_name;
                std::stringstream split_stream(d_name_ng);
                std::string temp_string;
                getline(split_stream,temp_string,':');
                int ng_id = atoi(temp_string.c_str());
                if(ng_id==group_id){
                    ng_found = true;
                }
            }
            entry = readdir(dir);
        }
        if(!ng_found){
            std::cerr<<"News group does not exist"<<std::endl;
            return NULL;
        }
        ss1<<path<<"/"<<d_name_ng;
        std::string ng_path = ss1.str();
        DIR *ng_dir = opendir(ng_path.c_str());   
        if(ng_dir==NULL){
            std::cerr<<"News group does not exist"<<std::endl;
            return NULL;
        }
        struct dirent *entry2;
        bool art_found = false;
        while(!art_found){
            entry2 = readdir(ng_dir);
            std::string name = entry2->d_name;
            if(entry2==NULL){
            	std::cerr<<"Could not find article"<<std::endl;
                return NULL;
            }
            if(name!="."  && (name!="..") && (name)[0]!='.' && name != "used_indices.txt"){
                d_name_art = name;
                std::cout<<d_name_art<<std::endl;
                std::stringstream split_stream(d_name_art);
                std::string temp_string;
                getline(split_stream,temp_string,':');
                int art_id = atoi(temp_string.c_str());
                if(art_id==article_id){
                    art_found = true;
                }
            }
        }
        if(!art_found){
            std::cerr<<"article was not found"<<std::endl;
            return NULL;
        }
        std::cout<<"reading file"<<std::endl;
        std::string art_path = (ng_path+"/"+d_name_art);
        article* art = read_article(art_path,article_id);

		return art;


	}
	news_group* on_disc_database::get_news_group(unsigned int id){
		std::cout<<"in get news group"<<std::endl;
		root = opendir(path.c_str());
		dirent *entry = readdir(root);
		std::string ng_name;
		news_group* ng;
		bool found_ng = false;
		while(entry != NULL && !found_ng){
			std::string name = entry->d_name;
			if(name != "" && name != "." && name != ".." && name[0] != '.' && entry->d_type == DT_DIR){
				std::stringstream split_stream(name);
				std::string id_string;
				getline(split_stream,id_string,':');
				int ng_id = atoi(id_string.c_str());
				if(ng_id == id){
					getline(split_stream,ng_name);
					ng = new news_group(ng_name, id);
					found_ng = true;
				}
			}
			entry = readdir(root);
		}
		if(!found_ng || ng == NULL || ng == 0){
			std::cout<<"could not found news group"<<std::endl;
			return NULL;
		}
		std::cout<<"in the middle"<<std::endl;
		std::stringstream int_con;
		int_con<<id;
		std::string id_string;
		std::cout<<"in the middle1.5"<<std::endl;
		id_string = int_con.str();
		std::string ng_path = path+"/"+id_string+":"+ng_name;
		std::cout<<"in the middle2"<<std::endl;
		closedir(root);
		root = opendir(ng_path.c_str());
		std::cout<<ng_path<<std::endl;
		if(root == NULL){
			std::cerr<<"could not open news group"<<std::endl;
			return NULL;
		}
		std::cout<<"in the middle2.25"<<std::endl;
		entry = readdir(root);
		std::cout<<"in the middle2.5"<<std::endl;
		while(entry != NULL){
			std::string name = entry->d_name;
			if(name != "" && name != "." && name != ".." && name[0] != '.' && name != "used_indices.txt"){
				std::cout<<"in the beginning, name: "<<name<<std::endl;
				std::stringstream ss(name);
				std::string temp_string;
				getline(ss,temp_string,':');
				unsigned int art_id = atoi(temp_string.c_str());
				std::string art_path;
				art_path = path+"/"+id_string+":"+ng_name+"/"+name;
				std::cout<<"before read_art"<<std::endl;
				article* art = read_article(art_path, art_id);
				if(art == NULL || art == 0){
					std::cout<<"article is null"<<std::endl;
					return NULL;
				}
				std::cout<<"after read_art: "<<(ng->get_name())<<art->get_title()<<std::endl;

				ng->add_article(*art);
				std::cout<<"after add_art"<<std::endl;
			}
			std::cout<<"new entry time"<<std::endl;
			entry = readdir(root);
		}
		std::cout<<"in the end"<<std::endl;
		return ng;
	}
}
