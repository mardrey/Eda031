#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>


namespace database{
class article{
public:
	article(int i, std::string titl, std::string aut, std::string cont);
	~article();
	std::string get_content();
	int get_id();
	std::string get_title();
	std::string get_author();
private:
	std::string content;
	std::string title;
	std::string author;
	int id;
};
}
#endif
