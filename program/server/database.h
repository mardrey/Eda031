#ifndef DATABASE_H
#define DATABASE_H
#include <string>


namespace database{
class database{
	public:
	database(){}
	~database(){}
	//virtual void add_news_group(std::string& name)=0;
	void print_status(int i){}
	void add_news_group(std::string& name){}
	void list_news_groups(){}
	bool delete_news_group(unsigned int id){
		return false;
	}
	bool add_article(unsigned int id, std::string& title, std::string& author, std::string& content){
		return false;
	}
	int  delete_article(unsigned int group_id, unsigned int article_id){
		return -1;
	}
	private:

};
}

#endif
