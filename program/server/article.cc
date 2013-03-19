#include <string>
#include "article.h"
namespace database{


	article::article(std::string s, unsigned int i){
		content = s;
		id = i;
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
