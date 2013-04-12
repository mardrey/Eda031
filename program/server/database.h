#ifndef DATABASE_H
#define DATABASE_H
#include <string>
#include "article.h"
#include "news_group.h"
#include <vector>

namespace database{
class database{
	public:
	//database();
	//virtual ~database();
	//virtual void add_news_group(std::string& name)=0;
	virtual int add_news_group(std::string& name) = 0;
	virtual std::vector<news_group> list_news_groups()=0;
	virtual bool delete_news_group(int id)=0;
	virtual bool add_article(int id, std::string& title, std::string& author, std::string& content)=0;
	virtual int  delete_article(int group_id, int article_id)=0;
	virtual article get_article(int group_id, int article_id)=0;
	virtual news_group get_news_group(int id)=0;
	private:

};
}

#endif
