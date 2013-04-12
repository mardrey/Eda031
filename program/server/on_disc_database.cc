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

	std::string int_to_string(int i){
		std::stringstream ss_convert;
		ss_convert<<i;
		return (ss_convert.str());
	}

	int get_id_for_file_or_dir(std::string& name, char del){
		std::cout<<"NAME: "<<name<<std::endl;
		std::string name_res;
		std::stringstream iter_stream(name);
		char c;
		iter_stream.get(c);
		while(c != del){
			name_res+=c;
			iter_stream.get(c);
		}
		std::cout<<"CSTRING IS: "<<name_res<<std::endl;
		int id = atoi(name_res.c_str());
		return id;
	}

	std::string get_name_for_file_or_dir(std::string& name, char del){
		std::string name_res;
		std::stringstream iter_stream(name);
		char c;
		iter_stream.get(c);
		int traversed = 1;
		for(unsigned int i = traversed; (i < name.size()&&c != del);++i){
		//	std::cout<<c<<std::endl;
			iter_stream.get(c);
			traversed++;
		}
		name_res = name.substr(traversed,name.size()-1);
		return name_res;
	}

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
				closedir(root);
				return false;
			}
			struct dirent *entry = readdir(root);
			while(entry != NULL){
				std::string nome = entry->d_name;
				if(nome != "." && nome != ".." && nome != "" && nome[0] != '.'){
					std::string name = path + "/" + (nome);
					if(!delete_file_dir_entry(name)){
					closedir(root);
						std::cerr<<"Deletion of "<<(name)<<" was unsuccessful"<<std::endl;
						return false;
					}
				}
				entry = readdir(root);
			}
			closedir(root);
			return (rmdir(path.c_str()) != -1);
		}
	}

	article read_article(std::string& path, unsigned int id){
		std::ifstream ifs;
		ifs.open(path.c_str());
		if(!ifs){
			std::cerr<<"could not open file"<<std::endl;
			std::string f("Fail");
			article fail_art(-1,f,f,f);
			return fail_art;
		}
		unsigned int title_l;
		ifs>>title_l;
		std::string title;
		for(unsigned int i = 0; i < title_l; ++i){
			char c;
			ifs.get(c);
			title+=c;
		}
		unsigned int author_l;
		ifs>>author_l;
		std::string author;
		for(unsigned int i = 0; i < author_l; ++i){
			char c;
			ifs.get(c);
			author+=c;
		}
		unsigned int content_l;
		ifs>>content_l;
		std::string content;
		for(unsigned int i = 0; i < content_l; ++i){
			char c;
			ifs.get(c);
			content+=c;
		}
		article art(id,title,author,content);
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
		closedir(root);
	}

	int on_disc_database::add_news_group(std::string& name){

		root = opendir(path.c_str());
		dirent* entry = readdir(root);
		while(entry != NULL){
			std::string p = entry->d_name;
			std::string filename = get_name_for_file_or_dir(p,':');
			std::cout<<"P: "<<p<<"FN: "<<filename<<std::endl;
			if(name == filename){
				closedir(root);
				std::cerr<<"Group name already in use"<<std::endl;
				return -1;
			}
			entry = readdir(root);
		}
		closedir(root);
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

	bool on_disc_database::delete_news_group(int id){
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
						bool res = delete_file_dir_entry(name);
						closedir(root);
						return res;
					}
				}
			}
			entry = readdir(root);
		}
		closedir(root);
		return false;
	}

	bool on_disc_database::add_article(int id, std::string& title, std::string& author, std::string& content){
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
			closedir(dir);
			return false;
		}
		ss1<<path<<"/"<<d_name;
		std::string ng_path = ss1.str();	
		DIR *ng_dir = opendir(ng_path.c_str());	
		if(ng_dir==NULL){
			closedir(dir);
			closedir(ng_dir);
			std::cerr<<"News group does not exist"<<std::endl;
			return false;
		}
		unsigned int highest = get_unused_index(ng_path+"/used_indices.txt");
		std::stringstream ss2;
		ss2<<ng_path<<"/"<<highest;
		std::string art_path = ss2.str();
		std::ofstream ofs;
		ofs.open((art_path+":"+title+".art").c_str());
		unsigned int title_l = title.size();
		unsigned int author_l = author.size();
		unsigned int content_l = content.size();
		ofs<<title_l<<title<<author_l<<author<<content_l<<content;
 		ofs.close();
		closedir(dir);
		closedir(ng_dir);
 		return true;
	}

	int on_disc_database::delete_article(int group_id, int article_id){

		std::stringstream ss1;
		std::string d_name_ng;
		std::string d_name_art;
		DIR *dir = opendir(path.c_str());
		struct dirent *entry = readdir(dir);
		bool ng_found = false;
		while(entry!=NULL && !ng_found){
			std::string nome = entry->d_name;
			if(entry->d_type == DT_DIR && nome != ".." && nome != "." && nome[0] != '.'){
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
			closedir(dir);
			return -1;
		}
		ss1<<path<<"/"<<d_name_ng;
		std::string ng_path = ss1.str();
		DIR *ng_dir = opendir(ng_path.c_str());	
		if(ng_dir==NULL){
			std::cerr<<"News group does not exist"<<std::endl;
			closedir(dir);
			closedir(ng_dir);
			return -1;
		}
        struct dirent *entry2;
        bool art_found = false;
		while(!art_found){
			entry2 = readdir(ng_dir);
			if(entry2==NULL){
				std::cerr<<"Article does not exist"<<std::endl;
				closedir(dir);
				closedir(ng_dir);
				return 1;
			}
			std::string nome(entry2->d_name);
			if(nome!="."  && (nome!="..") && nome[0] != '.' && nome != "used_indices.txt"){
				d_name_art = entry2->d_name;
				std::stringstream split_stream(d_name_art);
				std::string temp_string;
				getline(split_stream,temp_string,':');
				std::cout<<d_name_art<<std::endl;
				std::cout<<ng_path<<std::endl;
				std::cout<<(ng_path+"/"+d_name_art)<<std::endl;
				int art_id = get_id_for_file_or_dir(d_name_art,':');
				std::cout<<"found id: "<<art_id<<", wanted id: "<<article_id<<std::endl;
				if(art_id==article_id){
					art_found = true;
				}
			}
		}
        if(!art_found){
			std::cerr<<"article was not found"<<std::endl;
				closedir(dir);
				closedir(ng_dir);
			return 1;
		}
        if(remove((ng_path+"/"+d_name_art).c_str())!=0){
        	std::cout<<(ng_path+"/"+d_name_art)<<std::endl;
			std::cerr<<"article deletion unsuccessful"<<std::endl;
			closedir(dir);
			closedir(ng_dir);
			return 1;
		}
		closedir(dir);
		closedir(ng_dir);
		return 0;
	}
	article on_disc_database::get_article(int group_id, int article_id){
		std::string f("Fail");
		article fail_art(-1,f,f,f);	
		std::cout<<"GA0"<<std::endl;
        news_group ng2 = get_news_group(group_id);
        if(ng2.get_name() == "Fail"){
            std::cerr<<"News group does not exist1"<<std::endl;
            return fail_art;
        }
		std::cout<<"GA1"<<std::endl;
        std::string i_s;
        std::string ng2_name;
        i_s = int_to_string(group_id);
        ng2_name = ng2.get_name();
        std::string d_name_ng;
        d_name_ng = (i_s + ":" + ng2_name);
        std::string ng_path = path + "/" + d_name_ng;
        std::cout<<"path: "<<ng_path<<std::endl;
        DIR *ng_dir = opendir(ng_path.c_str());   
        if(ng_dir==NULL){
            std::cerr<<"News group does not exist2"<<std::endl;
            closedir(ng_dir);
            return fail_art;
        }
        std::cout<<"GA2"<<std::endl;
        struct dirent* entry2;
        bool art_found = false;
        std::string d_name_art;
        while(!art_found){
            entry2 = readdir(ng_dir);
            if(entry2==NULL){
            	std::cerr<<"Could not find article"<<std::endl;
            	closedir(ng_dir);
                return fail_art;
            }
            std::string name = entry2->d_name;
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
        std::cout<<"GA3"<<std::endl;
        if(!art_found){
            std::cerr<<"article was not found"<<std::endl;
            if(entry2 != NULL){
       		}
            closedir(ng_dir);
            return fail_art;
        }
        std::cout<<"GA3.5"<<std::endl;
        std::cout<<"GA3.56365"<<std::endl;
        closedir(ng_dir);
        std::cout<<"GA3.6275"<<std::endl;
        std::string art_path = (ng_path+"/"+d_name_art);
        std::cout<<"GA3.625"<<std::endl;
        article art = read_article(art_path,article_id);
        std::cout<<"GA3.75"<<std::endl;
        if(art.get_id() == -1){
        	return art;
        }
        std::cout<<"GA4"<<std::endl;
		return art;


	}
	news_group on_disc_database::get_news_group(int id){
		std::string f("Fail");
		news_group fail_ng(f,-1);
		root = opendir(path.c_str());
		dirent *entry = readdir(root);
		std::string ng_name;
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
					found_ng = true;
				}
			}
			entry = readdir(root);
		}
		news_group ng(ng_name,id);
		if(!found_ng || ng.get_id() == -1){
			std::cout<<"could not find news group"<<std::endl;
			closedir(root);
			return fail_ng;
		}
		std::stringstream int_con;
		int_con<<id;
		std::string id_string;
		id_string = int_con.str();
		std::string ng_path = path+"/"+id_string+":"+ng_name;
		closedir(root);
		root = opendir(ng_path.c_str());
		std::cout<<ng_path<<std::endl;
		if(root == NULL){
			std::cerr<<"could not open news group"<<std::endl;
			closedir(root);
			return fail_ng;
		}
		entry = readdir(root);
		while(entry != NULL){
			std::string name = entry->d_name;
			if(name != "" && name != "." && name != ".." && name[0] != '.' && name != "used_indices.txt"){
				std::stringstream ss(name);
				std::string temp_string;
				getline(ss,temp_string,':');
				unsigned int art_id = atoi(temp_string.c_str());
				std::string art_path;
				art_path = path+"/"+id_string+":"+ng_name+"/"+name;
				article art = read_article(art_path, art_id);
				if(art.get_id() == -1){
					std::cout<<"article is fail"<<std::endl;
					closedir(root);
					return fail_ng;
				}
				ng.add_article(art);
			}
			entry = readdir(root);
		}
		closedir(root);
		return ng;
	}
}
