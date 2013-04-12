#include <string>
#include "article.h"

namespace database{


	article::article(int i,std::string titl, std::string aut,std::string cont){
		content = cont;
		id = i;
		title = titl;
		author = aut;

	}
	article::~article(){}

	std::string article::get_content(){
		return content;
	}
	std::string article::get_author(){
		return author;
	}
	std::string article::get_title(){
		return title;
	}

	int article::get_id(){
		return id;
	}

}
