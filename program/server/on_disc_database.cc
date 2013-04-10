#include "on_disc_database.h"
#include <string>
#include "news_group.h"
#include "article.h"
#include "database.h"

namespace database{
	on_disc_database::on_disc_database(){

	}
	on_disc_database::~on_disc_database(){
		
	}

	void on_disc_database::print_status(int i){

	}

	int on_disc_database::add_news_group(std::string& name){

	}

	std::vector<news_group> on_disc_database::list_news_groups(){

	}

	bool on_disc_database::delete_news_group(unsigned int id){

	}

	bool on_disc_database::add_article(unsigned int id, std::string& title, std::string& author, std::string& content){

	}

	int on_disc_database::delete_article(unsigned int group_id, unsigned int article_id){

	}
	article* on_disc_database::get_article(unsigned int group_id, unsigned int article_id){

	}
	news_group* on_disc_database::get_news_group(unsigned int id){

	}
}