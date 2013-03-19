#include <string>
#include "article.h"
namespace database{


	article::article(std::string cont, unsigned int i,std::string titl, std::string aut){
		content = cont;
		id = i;
		title = titl;
		author = aut;
	}
	article::~article(){}

	std::string article::get_content(){
		return content;
	}
	unsigned int article::get_id(){
		return id;
	}
	bool article::operator==(article other){
		if(id==other.get_id()){
			return true;
		}
		return false;
	}

}
