#ifndef NEWS_GROUP_H
#define NEWS_GROUP_H

#include <string>
#include <vector>
#include "article.h"

namespace database{
class news_group{
public:
	news_group(std::string& nam, int i);
	~news_group();
	int get_id();
	std::string get_name();
	void new_article(std::string& content, std::string& title, std::string& author);
	bool remove_article(int id);
	std::vector<article>* list_articles();
	article* get_article_from_id(int art_id);
	article* get_article_from_name(std::string& comp_title);
	void add_article(article a);
private:
	std::vector<article> articles;
	int id;
	unsigned int article_ids;
	std::string name;
};
}
#endif
