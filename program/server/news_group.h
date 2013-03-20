#ifndef NEWS_GROUP_H
#define NEWS_GROUP_H

#include <string>
#include <vector>
#include "article.h"

namespace database{
class news_group{
public:
	news_group(unsigned int i);
	~news_group();
	unsigned int get_id();
	void add_article(article a);
	void remove_article(article a);
	article* get_article(unsigned int art_id);
private:
	std::vector<article> articles;
	unsigned int id;
};
}
#endif
