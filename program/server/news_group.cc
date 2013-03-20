#include <string>
#include <vector>
#include "article.h"
#include "news_group.h"
namespace database{

news_group::news_group(unsigned int i){
	id = i;
	article_ids=0;
}
news_group::~news_group(){
	
}
unsigned int news_group::get_id(){
	return id;
}

void news_group::new_article(string& content, string& title, string& author){
	article a(content, article_ids, title, author);
	article_ids++;
	add_article(a);
}

void news_group::add_article(article a){
	articles.push_back(a);
}
void news_group::remove_article(article a){
	for(unsigned int i = 0; i<articles.size(); ++i){
		if(articles[i]==a){
			articles.erase(articles.begin()+i);
		}
	}
}
article* news_group::get_article(unsigned int art_id){
	article* dest = 0;
	for(unsigned int i = 0; i<articles.size(); ++i){
		if(articles[i].get_id()==art_id){
			dest=&articles[i];
		}
	}
	return dest;
}
}
