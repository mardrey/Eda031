#ifndef DATABASE_H
#define DATABASE_H
#include <string>
#include "article.h"
#include "news_group.h"
namespace database{
class database{
	public:
	database(){}
	~database(){}
	//virtual void add_news_group(std::string& name)=0;
	 void print_status(int i);
	 int add_news_group(std::string& name);
	 void list_news_groups();
	bool delete_news_group(unsigned int id);
	bool add_article(unsigned int id, std::string& title, std::string& author, std::string& content);
	int  delete_article(unsigned int group_id, unsigned int article_id);
	article* get_article(unsigned int group_id, unsigned int article_id);
	news_group* get_news_group(unsigned int id);
	private:

};
}

#endif
