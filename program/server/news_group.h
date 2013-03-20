#ifndef NEWS_GROUP_H
#define NEWS_GROUP_H

#include <string>
#include <vector>
#include "article.h"

namespace database{
class news_group{
public:
	news_group(std::string& nam, unsigned int i);
	~news_group();
	unsigned int get_id();
	std::string get_name();
	void new_article(std::string& content, std::string& title, std::string& author);
	void remove_article(article a);
	article* get_article_from_id(unsigned int art_id);
	article* get_article_from_name(std::string& comp_title);
private:
	void add_article(article a);
	std::vector<article> articles;
	unsigned int id;
	unsigned int article_ids;
	std::string name;
};
}
#endif
