#include <string>
#include <vector>
#include "article.h"
#include "news_group.h"
#include <iostream>
#include <climits>
namespace database{

news_group::news_group(std::string& nam, int i){
	id = i;
	article_ids=0;
	name = nam;
}
news_group::~news_group(){
	
}

int news_group::get_id(){
	return id;
}
std::string news_group::get_name(){
	return name;
}

void news_group::new_article(std::string& title, std::string& author, std::string& content){
	
	if(article_ids<UINT_MAX){
		article a(article_ids, title, author,content);
		article_ids++;
		add_article(a);
	}
	else{
		std::cerr<<"Maximum amount of articles reached"<<std::endl;
		throw -1;
	}
}

void news_group::add_article(article a){
	articles.push_back(a);
}
bool news_group::remove_article(int id){
	bool found = false;
	for(unsigned int i = 0; i<articles.size(); ++i){
		if(articles[i].get_id()==id){
			articles.erase(articles.begin()+i);
			found = true;
		}
	}
	return found;
}
article* news_group::get_article_from_id(int art_id){
	article* dest = 0;
	bool found = false;
	for(unsigned int i = 0; i<articles.size()&&!found; ++i){
		if(articles[i].get_id()==art_id){
			dest=&articles[i];
			found=true;
		}
	}
	return dest;
}
article* news_group::get_article_from_name(std::string& comp_title){
	article* dest = 0;
	bool found = false;
	for(unsigned int i = 0; i<articles.size()&&!found; ++i){
		if(articles[i].get_title()==comp_title){
			dest=&articles[i];
			found=true;
		}
	}
	return dest;
}
std::vector<article>* news_group::list_articles(){
	return &articles;
}
}
